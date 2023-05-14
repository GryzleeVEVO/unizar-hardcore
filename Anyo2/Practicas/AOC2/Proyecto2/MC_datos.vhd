----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    10:38:16 04/08/2014 
-- Design Name: 
-- Module Name:    
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: La memoria cache est� compuesta de 8 bloques de 4 datos con: asociatividad 2, escritura directa, y la politica convencional en fallo de escritura (fetch on write miss). 
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
----------------------------------------------------------------------------------
LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.ALL;
USE ieee.numeric_std.ALL; -- se usa para convertir std_logic a enteros

ENTITY MC_datos IS PORT (
	CLK : IN STD_LOGIC;
	reset : IN STD_LOGIC;
	--Interfaz con el MIPS
	--entradas
	ADDR : IN STD_LOGIC_VECTOR (31 DOWNTO 0); --Dir 
	Din : IN STD_LOGIC_VECTOR (31 DOWNTO 0);
	RE : IN STD_LOGIC; -- read enable		
	WE : IN STD_LOGIC;
	--salidas
	ready : OUT STD_LOGIC; -- indica si podemos hacer la operaci�n solicitada en el ciclo actual
	Dout : OUT STD_LOGIC_VECTOR (31 DOWNTO 0); --dato que se env�a al Mips
	-- Nueva se�al de error
	Mem_ERROR : OUT STD_LOGIC; -- Se activa si en la ultima transferencia el esclavo no respondi� a su direcci�n
	--Interfaz con el bus
	--entradas
	MC_Bus_Din : IN STD_LOGIC_VECTOR (31 DOWNTO 0);--para leer datos del bus
	Bus_TRDY : IN STD_LOGIC; --indica que el esclavo (la memoriade datos)  puede realizar la operaci�n solicitada en este ciclo
	Bus_DevSel : IN STD_LOGIC; --indica que la memoria ha reconocido que la direcci�n est� dentro de su rango
	MC_Bus_Grant : IN STD_LOGIC; --indica que el �rbitro permite usar el bus a la MC;
	--salidas
	MC_send_addr_ctrl : OUT STD_LOGIC; --ordena que se env�en la direcci�n y las se�ales de control al bus
	MC_send_data : OUT STD_LOGIC; --ordena que se env�en los datos
	MC_frame : OUT STD_LOGIC; --indica que la operaci�n no ha terminado
	MC_Bus_ADDR : OUT STD_LOGIC_VECTOR (31 DOWNTO 0); --Dir 
	MC_Bus_data_out : OUT STD_LOGIC_VECTOR (31 DOWNTO 0);--para enviar datos por el bus
	MC_bus_Rd_Wr : OUT STD_LOGIC; --'0' para lectura,  '1' para escritura
	MC_Bus_Req : OUT STD_LOGIC; --indica que la MC quiere usar el bus;
	MC_last_word : OUT STD_LOGIC --indica que es el �ltimo dato de la transferencia
);
END MC_datos;

ARCHITECTURE Behavioral OF MC_datos IS

	COMPONENT UC_MC IS
		PORT (
			clk : IN STD_LOGIC;
			reset : IN STD_LOGIC;
			RE : IN STD_LOGIC; --RE y WE son las ordenes del MIPs
			WE : IN STD_LOGIC;
			hit0 : IN STD_LOGIC; --se activa si hay acierto en la via 0
			hit1 : IN STD_LOGIC; --se activa si hay acierto en la via 1
			bus_TRDY : IN STD_LOGIC; --indica que la memoria puede realizar la operaci�n solicitada en este ciclo
			Bus_DevSel : IN STD_LOGIC; --indica que la memoria ha reconocido que la direcci�n est� dentro de su rango
			via_2_rpl : IN STD_LOGIC; --indica que via se va a reemplazar
			Bus_grant : IN STD_LOGIC; --indica la concesi�n del uso del bus
			addr_non_cacheable : IN STD_LOGIC; --indica que la direcci�n no debe almacenarse en MC. En este caso porque pertenece a la scratch
			-- Nueva se�al que indica que la direcci�n solicitada es de un registro de MC
			internal_addr : IN STD_LOGIC;
			-- Nueva se�al para la gesti�n de errores
			unaligned : IN STD_LOGIC; --indica que la direcci�n solicitada por el MIPS no est� alineada
			--salidas
			Mem_ERROR : OUT STD_LOGIC; -- Se activa si en la ultima transferencia el esclavo no respondi� a su direcci�n
			load_addr_error : OUT STD_LOGIC; --para controlar el registro que guarda la direcci�n que caus� error
			--
			MC_WE0 : OUT STD_LOGIC;
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
			inc_m : OUT STD_LOGIC; -- indica que ha habido un fallo
			inc_w : OUT STD_LOGIC; -- indica que ha habido una escritura
			mux_output : OUT STD_LOGIC_VECTOR(1 DOWNTO 0); -- para elegir si le mandamos al procesador la salida de MC (valor 0),los datos que hay en el bus (valor 1), o un registro interno( valor 2)
			last_word : OUT STD_LOGIC; --indica que es el �ltimo dato de la transferencia
			Bus_req : OUT STD_LOGIC --indica la petici�n al �rbitro del uso del bus
		);
	END COMPONENT;

	COMPONENT reg IS
		GENERIC (size : NATURAL := 32); -- por defecto son de 32 bits, pero se puede usar cualquier tama�o
		PORT (
			Din : IN STD_LOGIC_VECTOR (size - 1 DOWNTO 0);
			clk : IN STD_LOGIC;
			reset : IN STD_LOGIC;
			load : IN STD_LOGIC;
			Dout : OUT STD_LOGIC_VECTOR (size - 1 DOWNTO 0));
	END COMPONENT;

	COMPONENT counter IS
		GENERIC (size : INTEGER := 10);
		PORT (
			clk : IN STD_LOGIC;
			reset : IN STD_LOGIC;
			count_enable : IN STD_LOGIC;
			count : OUT STD_LOGIC_VECTOR (size - 1 DOWNTO 0));
	END COMPONENT;

	COMPONENT Via IS
		GENERIC (num_via : INTEGER); -- se usa para los mensajes. Hay que poner el n�mero correcto al instanciarla
		PORT (
			CLK : IN STD_LOGIC;
			reset : IN STD_LOGIC;
			Dir_word : IN STD_LOGIC_VECTOR(1 DOWNTO 0); -- se usa para elegir la palabra a la que se accede en un conjunto la cache de datos. 
			Dir_cjto : IN STD_LOGIC_VECTOR(1 DOWNTO 0); -- se usa para elegir el conjunto
			Tag : IN STD_LOGIC_VECTOR(25 DOWNTO 0);
			Din : IN STD_LOGIC_VECTOR (31 DOWNTO 0);
			RE : IN STD_LOGIC; -- read enable		
			WE : IN STD_LOGIC; -- write enable	
			Tags_WE : IN STD_LOGIC; -- write enable para la memoria de etiquetas 
			hit : OUT STD_LOGIC; -- indica si es acierto
			Dout : OUT STD_LOGIC_VECTOR (31 DOWNTO 0));
	END COMPONENT;

	COMPONENT FIFO_reg IS
		PORT (
			clk : IN STD_LOGIC;
			reset : IN STD_LOGIC;
			cjto : IN STD_LOGIC_VECTOR (1 DOWNTO 0); --Dir del cjto reemplazado
			new_block : IN STD_LOGIC; -- Indica que hay un reemplazo y por tanto hay que actualizar la info del fifo del conjunto correspondiente				        
			via_2_rpl : OUT STD_LOGIC
		);
	END COMPONENT;

	SIGNAL dir_cjto : STD_LOGIC_VECTOR(1 DOWNTO 0); -- se usa para elegir el cjto al que se accede en la cache de datos. 
	SIGNAL dir_word : STD_LOGIC_VECTOR(1 DOWNTO 0); -- se usa para elegir la dato solicitada de un determinado bloque. 
	SIGNAL internal_MC_bus_Rd_Wr, mux_origen, MC_Tags_WE, block_addr, new_block : STD_LOGIC;
	SIGNAL via_2_rpl, Tags_WE_via0, Tags_WE_via1, hit0, hit1, WE_via0, WE_via1 : STD_LOGIC;
	SIGNAL palabra_UC : STD_LOGIC_VECTOR(1 DOWNTO 0); --se usa al traer un bloque nuevo a la MC (va cambiando de valor para traer todas las palabras)
	SIGNAL MC_Din, MC_Dout, Dout_via1, Dout_via0, Addr_Error, Internal_MC_Bus_ADDR : STD_LOGIC_VECTOR (31 DOWNTO 0);
	SIGNAL Tag : STD_LOGIC_VECTOR(25 DOWNTO 0);
	SIGNAL m_count, w_count, Mem_stalls : STD_LOGIC_VECTOR(7 DOWNTO 0);
	SIGNAL inc_m, inc_w, inc_mem_stall : STD_LOGIC;
	SIGNAL addr_non_cacheable, internal_addr, load_addr_error, unaligned, Mem_ready : STD_LOGIC;
	SIGNAL mux_output : STD_LOGIC_VECTOR(1 DOWNTO 0);
BEGIN
	-------------------------------------------------------------------------------------------------- 
	-----MC_data: memoria RAM que almacena los 8 bloques de 4 datos que puede guardar la Cache
	-- dir palabra puede venir de la entrada (cuando se busca un dato solicitado por el Mips) o de la Unidad de control, UC, (cuando se est� escribiendo un bloque nuevo 
	-------------------------------------------------------------------------------------------------- 
	-- la regi�n que empieza por "00010000000000000000000" est� definida como no cacheable.
	-- Las direcciones en esa regi�n se env�an a la MD_scratch y cuando responda se reenvia el resultado al procesador. 
	-- Nunca se debe guardar nada de ese intervalo en MC

	addr_non_cacheable <= '1' WHEN Addr(31 DOWNTO 8) = x"100000" ELSE
		'0';
	unaligned <= '1' WHEN Addr(1 DOWNTO 0) /= "00" ELSE
		'0';
	tag <= ADDR(31 DOWNTO 6);
	dir_word <= ADDR(3 DOWNTO 2) WHEN (mux_origen = '0') ELSE
		palabra_UC;
	dir_cjto <= ADDR(5 DOWNTO 4); -- es emplazamiento asociativo
	-- la entrada de datos de la MC puede venir del Mips (acceso normal) o del bus (gesti�n de fallos)
	MC_Din <= Din WHEN (mux_origen = '0') ELSE
		MC_bus_Din;

	Via_0 : Via GENERIC MAP(num_via => 0)PORT MAP(clk => clk, reset => reset, RE => RE, WE => WE_via0, Tags_WE => Tags_WE_via0, hit => hit0, Dir_cjto => Dir_cjto, Dir_word => Dir_word, Tag => Tag, Din => MC_Din, Dout => Dout_via0);

	Via_1 : Via GENERIC MAP(num_via => 1)PORT MAP(clk => clk, reset => reset, RE => RE, WE => WE_via1, Tags_WE => Tags_WE_via1, hit => hit1, Dir_cjto => Dir_cjto, Dir_word => Dir_word, Tag => Tag, Din => MC_Din, Dout => Dout_via1);

	MC_Dout <= Dout_via1 WHEN (hit1 = '1') ELSE
		Dout_via0;

	new_block <= MC_Tags_WE; -- la info para el fifo se actualiza cada vez que se escribe una nueva etiqueta

	Info_FIFO : FIFO_reg PORT MAP(clk => clk, reset => reset, cjto => dir_cjto, new_block => new_block, via_2_rpl => via_2_rpl);

	-- se elige en qu� via se escribe la nueva etiqueta seg�n indique via_2_rpl
	Tags_WE_via0 <= MC_Tags_WE AND NOT(via_2_rpl);
	Tags_WE_via1 <= MC_Tags_WE AND via_2_rpl;
	-------------------------------------------------------------------------------------------------- 
	-----MC_UC: unidad de control
	-------------------------------------------------------------------------------------------------- 
	Unidad_Control : UC_MC PORT MAP(
		clk => clk, reset => reset, RE => RE, WE => WE, hit0 => hit0, hit1 => hit1, bus_TRDY => bus_TRDY,
		bus_DevSel => bus_DevSel, MC_WE0 => WE_via0, MC_WE1 => WE_via1, MC_bus_Rd_Wr => internal_MC_bus_Rd_Wr,
		MC_tags_WE => MC_tags_WE, palabra => palabra_UC, mux_origen => mux_origen, ready => Mem_ready, MC_send_addr_ctrl => MC_send_addr_ctrl,
		block_addr => block_addr, MC_send_data => MC_send_data, Frame => MC_Frame, via_2_rpl => via_2_rpl, last_word => MC_last_word,
		addr_non_cacheable => addr_non_cacheable, mux_output => mux_output, Bus_grant => MC_Bus_grant, Bus_req => MC_Bus_req,
		internal_addr => internal_addr, unaligned => unaligned, Mem_ERROR => Mem_ERROR, inc_m => inc_m, inc_w => inc_w, load_addr_error => load_addr_error);
	--------------------------------------------------------------------------------------------------
	----------- Contadores de eventos
	-------------------------------------------------------------------------------------------------- 
	cont_m : counter GENERIC MAP(size => 8)
	PORT MAP(clk => clk, reset => reset, count_enable => inc_m, count => m_count);
	cont_w : counter GENERIC MAP(size => 8)
	PORT MAP(clk => clk, reset => reset, count_enable => inc_w, count => w_count);
	inc_mem_stall <= NOT(Mem_ready);
	cont_Mem_stall : counter GENERIC MAP(size => 8)
	PORT MAP(clk => clk, reset => reset, count_enable => inc_mem_stall, count => Mem_stalls);
	--------------------------------------------------------------------------------------------------
	----------- Salidas para el bus
	-------------------------------------------------------------------------------------------------- 
	MC_bus_Rd_Wr <= internal_MC_bus_Rd_Wr;
	--Si es escritura se manda la direcci�n de la palabra y si es un fallo la direcci�n del bloque que caus� el fallo
	Internal_MC_Bus_ADDR <= ADDR(31 DOWNTO 2) & "00" WHEN block_addr = '0' ELSE
		ADDR(31 DOWNTO 4) & "0000";
	-- se usa la se�al "internal" para poder leerla, porque MC_Bus_ADDR es de salida y no se puede leer
	MC_Bus_ADDR <= Internal_MC_Bus_ADDR;

	MC_Bus_data_out <= Din; -- se usa para mandar el dato a escribir

	--------------------------------------------------------------------------------------------------
	-- Registro Addr Error
	-- Cuando se produce un error en el acceso a memoria (porque la direcci�n solicitada no corresponde a nadie) se guarda la direcci�n en este registro
	-- Su direcci�n asociada es "01000000"
	--------------------------------------------------------------------------------------------------
	ADDR_Error_Reg : reg GENERIC MAP(size => 32)
	PORT MAP(Din => Internal_MC_Bus_ADDR, clk => clk, reset => reset, load => load_addr_error, Dout => Addr_Error);
	--------------------------------------------------------------------------------------------------
	-- Decodificador para detectar si la se�al es interna. Es decir si pertenece a un registro de MC
	Internal_addr <= '1' WHEN (ADDR(31 DOWNTO 0) = x"01000000") ELSE
		'0';

	--------------------------------------------------------------------------------------------------
	----------- Salidas para el Mips
	-------------------------------------------------------------------------------------------------- 
	Dout <= MC_Dout WHEN mux_output = "00" ELSE
		MC_bus_Din WHEN mux_output = "01" ELSE -- se usa para mandar el dato que ha llegado por el bus directamente al Mips
		Addr_Error WHEN mux_output = "10" ELSE -- se usa para mandarle al Mips el contenido del registro Addr_Error
		x"00000000";

	ready <= Mem_ready;

END Behavioral;