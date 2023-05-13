---------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    13:38:18 05/15/2014 
-- Design Name: 
-- Module Name:    UC_slave - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: 
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: la UC incluye un contador de 2 bits para llevar la cuenta de las transferencias de bloque y una m�quina de estados
--
----------------------------------------------------------------------------------
LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

ENTITY UC_MC IS
    PORT (
        clk : IN STD_LOGIC;
        reset : IN STD_LOGIC;
        RE : IN STD_LOGIC; --RE y WE son las ordenes del MIPs
        WE : IN STD_LOGIC;
        hit0 : IN STD_LOGIC; --se activa si hay acierto en la via 0
        hit1 : IN STD_LOGIC; --se activa si hay acierto en la via 1
        addr_non_cacheable : IN STD_LOGIC; --indica que la direcci�n no debe almacenarse en MC. En este caso porque pertenece a la scratch
        bus_TRDY : IN STD_LOGIC; --indica que el esclavo no puede realizar la operaci�n solicitada en este ciclo
        Bus_DevSel : IN STD_LOGIC; --indica que el esclavo ha reconocido que la direcci�n est� dentro de su rango
        via_2_rpl : IN STD_LOGIC; --indica que via se va a reemplazar
        Bus_grant : IN STD_LOGIC; --indica la concesi�n del uso del bus
        Bus_req : OUT STD_LOGIC; --indica la petici�n al �rbitro del uso del bus
        -- Nueva se�al que indica que la direcci�n solicitada es de un registro de MC
        internal_addr : IN STD_LOGIC;
        -- Nueva se�al que indica que la direcci�n que env�a el MIPS no est� alineada
        unaligned : IN STD_LOGIC;
        -- Nueva se�al de error
        Mem_ERROR : OUT STD_LOGIC; -- Se activa si en la ultima transferencia el esclavo no respondi� a su direcci�n o la direcci�n solicitada no est� alineada
        load_addr_error : OUT STD_LOGIC; --para controlar el registro que guarda la direcci�n que caus� error
        --Interfaz con el bus
        MC_WE0 : OUT STD_LOGIC; -- write enable de la VIA 0 y 1
        MC_WE1 : OUT STD_LOGIC;
        MC_bus_Rd_Wr : OUT STD_LOGIC; --1 para escritura en Memoria y 0 para lectura
        MC_tags_WE : OUT STD_LOGIC; -- para escribir la etiqueta en la memoria de etiquetas
        palabra : OUT STD_LOGIC_VECTOR (1 DOWNTO 0);--indica la palabra actual dentro de una transferencia de bloque (1�, 2�...)
        mux_origen : OUT STD_LOGIC; -- Se utiliza para elegir si el origen de la direcci�n y el dato es el Mips (cuando vale 0) o la UC y el bus (cuando vale 1)
        ready : OUT STD_LOGIC; -- indica si podemos procesar la orden actual del MIPS en este ciclo. En caso contrario habr� que detener el MIPs
        block_addr : OUT STD_LOGIC; -- indica si la direcci�n a enviar es la de bloque (rm) o la de palabra (w)
        MC_send_addr_ctrl : OUT STD_LOGIC; --ordena que se env�en la direcci�n y las se�ales de control al bus
        MC_send_data : OUT STD_LOGIC; --ordena que se env�en los datos
        Frame : OUT STD_LOGIC; --indica que la operaci�n no ha terminado
        last_word : OUT STD_LOGIC; --indica que es el �ltimo dato de la transferencia
        mux_output : OUT STD_LOGIC_VECTOR(1 DOWNTO 0); -- para elegir si le mandamos al procesador la salida de MC (valor 0),los datos que hay en el bus (valor 1), o un registro interno( valor 2)
        inc_m : OUT STD_LOGIC; -- indica que ha habido un fallo
        inc_w : OUT STD_LOGIC -- indica que ha habido una escritura			
    );
END UC_MC;

ARCHITECTURE Behavioral OF UC_MC IS

    COMPONENT counter IS
        GENERIC (
            size : INTEGER := 10
        );
        PORT (
            clk : IN STD_LOGIC;
            reset : IN STD_LOGIC;
            count_enable : IN STD_LOGIC;
            count : OUT STD_LOGIC_VECTOR (size - 1 DOWNTO 0)
        );
    END COMPONENT;

    -- Estados UC principal
    TYPE state_type IS (
        Inicio,
        Fetch_Block,
        Get_Word,
        Put_Word
    );

    -- Estados UC error
    TYPE error_type IS (
        memory_error,
        No_error
    );

    SIGNAL state, next_state : state_type;
    SIGNAL error_state, next_error_state : error_type;
    SIGNAL last_word_block : STD_LOGIC; --se activa cuando se est� pidiendo la �ltima palabra de un bloque
    SIGNAL one_word : STD_LOGIC; --se activa cuando s�lo se quiere transferir una palabra
    SIGNAL count_enable : STD_LOGIC; -- se activa si se ha recibido una palabra de un bloque para que se incremente el contador de palabras
    SIGNAL hit : STD_LOGIC;
    SIGNAL palabra_UC : STD_LOGIC_VECTOR (1 DOWNTO 0);

BEGIN
    -- Ha habido acierto
    hit <= hit0 OR hit1;

    -- Cuenta palabras durante una transferencia
    word_counter : counter GENERIC MAP(size => 2)
    PORT MAP(
        clk,
        reset,
        count_enable,
        palabra_UC
    );

    -- Última palabra del bloque
    last_word_block <=
        '1' WHEN palabra_UC = "11" ELSE -- Si (palabra 0x......c)
        '0';

    -- Palabra actual
    palabra <= palabra_UC;

    -- Registro estado
    State_reg : PROCESS (clk)
    BEGIN
        IF (clk'event AND clk = '1') THEN
            IF (reset = '1') THEN
                state <= Inicio;
            ELSE
                state <= next_state;
            END IF;
        END IF;
    END PROCESS;

    -- Registro estado gestión errores
    error_reg : PROCESS (clk)
    BEGIN
        IF (clk'event AND clk = '1') THEN
            IF (reset = '1') THEN
                error_state <= No_error;
            ELSE
                error_state <= next_error_state;
            END IF;
        END IF;
    END PROCESS;

    -- Error memoria
    Mem_ERROR <=
        '1' WHEN (error_state = memory_error OR next_error_state = memory_error) ELSE
        '0';

    OUTPUT_DECODE : PROCESS (
        state, -- Estado actual

        hit0, -- Hit en cache
        hit1,
        hit,

        RE, -- Órden lectura/escritura MIPS
        WE,

        via_2_rpl, -- Vía a reemplazar
        addr_non_cacheable, -- @ no cacheable (MD_scratch) 
        last_word_block, -- Última palabra del bloque

        unaligned, -- @ mal alineada
        internal_addr, -- Pide @ error de acceso

        Bus_grant, -- Árbitro concede bus
        Bus_DevSel, -- @ pertenece a algún slave
        bus_TRDY -- Slave listo
        ) BEGIN
        next_state <= state; -- Estado siguiente (por defecto el actual)
        next_error_state <= error_state; -- Siguiente estado (error)

        ready <= '0'; -- Dato solicitado por MIPS listo

        MC_WE0 <= '0'; -- Escritura en cache
        MC_WE1 <= '0';
        MC_tags_WE <= '0'; --Escribir tag en cache

        mux_origen <= '0'; -- Origen D_in (MIPS = 0, bus = 1)
        mux_output <= "00"; -- Origen D_out (Cache = 0, Bus = 1, Error = 2)

        Bus_req <= '0'; -- Pedir acceso a bus

        MC_send_addr_ctrl <= '0'; -- Enviar @ y control
        block_addr <= '0'; -- Enviar palabra = 0, bloque = 1
        MC_bus_Rd_Wr <= '0'; -- Órden lectura/escritura del slave

        Frame <= '0'; -- Activar ráfaga (Solo 1 palabra para MD_scratch)
        MC_send_data <= '0'; -- Enviar datos

        inc_m <= '0'; -- Fallo en búsqueda
        inc_w <= '0'; -- Escritura en cache solicitadas por MIPS

        count_enable <= '0'; -- Contar palabra
        one_word <= '0'; -- Enviar solo una palabra (NO SE USA)
        last_word <= '0'; -- Última palabra a enviar

        load_addr_error <= '0'; -- Cargar @ error en registro

        CASE state IS
            WHEN Inicio =>
                -- Contiene todas las acciones que no sean transferencias
                IF (RE = '0' AND WE = '0') THEN
                    -- Nada que hacer
                    ready <= '1';

                ELSIF ((RE = '1' OR WE = '1') AND unaligned = '1') THEN
                    -- Fallo de alineación de @
                    next_error_state <= memory_error;
                    load_addr_error <= '1';
                    ready <= '1';

                ELSIF (RE = '1' AND internal_addr = '1') THEN
                    -- MIPS solicita @ de último error
                    next_error_state <= No_error;
                    mux_output <= "10";
                    ready <= '1';

                ELSIF (RE = '1' AND hit = '1') THEN
                    -- Acierto de lectura
                    mux_output <= "00";
                    ready <= '1';

                ELSE
                    -- Pide bus
                    Bus_req <= '1';

                    IF (Bus_grant = '1') THEN
                        -- Recibe bus
                        MC_send_addr_ctrl <= '1';

                        IF (hit = '0' AND addr_non_cacheable = '0') THEN
                            -- Fallo de MC
                            next_state <= Fetch_block;
                            inc_m <= '1';
                            MC_bus_Rd_Wr <= '0';
                            block_addr <= '1';

                        ELSIF (WE = '1') THEN
                            -- Escritura en MD_Scratch/acierto de lectura
                            next_state <= Put_word;
                            inc_w <= '1';
                            MC_bus_Rd_Wr <= '1';
                            block_addr <= '0';

                            IF (addr_non_cacheable = '0' AND hit = '1') THEN
                                -- Si acierto de lectura, escribe dato de MIPS en MC
                                mux_origen <= '0';
                                MC_WE0 <= hit0;
                                MC_WE1 <= hit1;
                            END IF;

                        ELSIF (RE = '1' AND addr_non_cacheable = '1') THEN
                            -- Lectura de MD_Scratch
                            next_state <= Get_Word;
                            MC_bus_Rd_Wr <= '0';
                            block_addr <= '0';

                        END IF;

                        IF (Bus_DevSel = '0') THEN
                            -- Esclavo no contesta
                            next_state <= Inicio;
                            next_error_state <= memory_error;
                            load_addr_error <= '1';
                            ready <= '1';
                        END IF;
                    END IF;
                END IF;

            WHEN Fetch_block =>
                -- Transferencia de un bloque entero de MD a MC
                Frame <= '1';

                IF (bus_TRDY = '1') THEN
                    -- Dispositivo listo

                    IF (last_word_block = '1') THEN
                        -- Última palabra, notificar al bus 
                        last_word <= '1';
                        next_state <= Inicio;
                        MC_tags_WE <= '1';
                    END IF;

                    mux_origen <= '1';
                    MC_WE0 <= NOT via_2_rpl;
                    MC_WE1 <= via_2_rpl;

                    count_enable <= '1';

                END IF;

            WHEN Get_Word =>
                -- Transferencia de una palabra de MD_Scratch a MIPS
                Frame <= '1';

                IF (bus_TRDY = '1') THEN
                    -- Dispositivo listo    
                    next_state <= Inicio;
                    last_word <= '1';

                    mux_output <= "01";

                    ready <= '1';
                END IF;

            WHEN Put_Word =>
                -- Transferencia de una palabra de MIPS a MD o MD_Scratch
                Frame <= '1';

                IF (bus_TRDY = '1') THEN
                    -- Dispositivo listo
                    next_state <= Inicio;
                    last_word <= '1';

                    mux_origen <= '0';
                    MC_send_data <= '1';

                    ready <= '1';
                END IF;

            WHEN OTHERS =>
                -- No debería ocurrir
                next_state <= Inicio;
        END CASE;
    END PROCESS;
END Behavioral;