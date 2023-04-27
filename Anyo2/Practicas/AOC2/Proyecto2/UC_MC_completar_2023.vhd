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
		single_word_transfer_addr,
		single_word_transfer_data,
		block_transfer_addr,
		block_transfer_data,
		Send_Addr_Word,
		Send_ADDR_CB,
		fallo,
		CopyBack,
		bajar_Frame
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
		'1' WHEN (error_state = memory_error) ELSE
		'0';

	OUTPUT_DECODE : PROCESS (state, hit, last_word_block, bus_TRDY, RE, WE, Bus_DevSel, Bus_grant, via_2_rpl, hit0, hit1, addr_non_cacheable, internal_addr, unaligned)
	BEGIN
		-- valores por defecto, si no se asigna otro valor en un estado valdr�n lo que se asigna aqu�
		MC_WE0 <= '0';
		MC_WE1 <= '0';
		MC_bus_Rd_Wr <= '0';
		MC_tags_WE <= '0';
		ready <= '0';
		mux_origen <= '0';
		MC_send_addr_ctrl <= '0';
		MC_send_data <= '0';
		next_state <= state; -- por defecto se mantiene el estado
		count_enable <= '0';
		Frame <= '0';
		block_addr <= '0';
		inc_m <= '0';
		inc_w <= '0';
		Bus_req <= '0';
		one_word <= '0';
		mux_output <= "00";
		last_word <= '0';
		next_error_state <= error_state; -- por defecto se mantiene el estado
		load_addr_error <= '0';

		-- Estado1
		--  | - Trans1
		--  | - Trasn2
		-- 	...
		-- Estado2
		-- ...
		IF (state = Inicio) THEN

			-- No actuar
			IF (RE = '0' AND WE = '0') THEN
				next_state <= Inicio;
				ready <= '1';

			-- Dirección mal alineada -> Error de dirección
			ELSIF (state = Inicio) AND ((RE = '1') OR (WE = '1')) AND (unaligned = '1') THEN
				next_state <= Inicio;
				ready <= '1';
				next_error_state <= memory_error;
				load_addr_error <= '1';
			
			-- MIPS pide dirección del Data Abort -> Bajar error
			ELSIF (state = Inicio AND RE = '1' AND internal_addr = '1') THEN 
				next_state <= Inicio;
				ready <= '1';
				mux_output <= "10"; 
				next_error_state <= No_error;
			
			-- Acierto lectura
			ELSIF (state = Inicio AND RE = '1' AND hit = '1') THEN 
				next_state <= Inicio;
				ready <= '1';
				mux_output <= "00";

			-- Escritura
			ELSIF (state = Inicio AND ((WE = '1') OR (RE = '1'))) THEN
				Bus_Req <= '1';
				ready <= '0';

			END IF;
		END IF;

	END PROCESS;
END Behavioral;