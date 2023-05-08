----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    14:12:11 04/04/2014 
-- Design Name: 
-- Module Name:    DMA - Behavioral 
-- Project Name: 
-- Target Devices: 
-- Tool versions: 
-- Description: 
--
-- Dependencies: 
--
-- Revision: 
-- Revision 0.01 - File Created
-- Additional Comments: 
--
LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.std_logic_arith.ALL;
USE IEEE.std_logic_unsigned.ALL;
ENTITY MD_scratch IS PORT (
	CLK : IN STD_LOGIC;
	reset : IN STD_LOGIC;
	Bus_Frame : IN STD_LOGIC; -- indica que el master quiere m�s datos
	bus_Read : IN STD_LOGIC;
	bus_Write : IN STD_LOGIC;
	Bus_Addr : IN STD_LOGIC_VECTOR (31 DOWNTO 0); --Direcciones 
	Bus_Data : IN STD_LOGIC_VECTOR (31 DOWNTO 0); --Datos  
	MD_Bus_DEVsel : OUT STD_LOGIC; -- para avisar de que se ha reconocido que la direcci�n pertenece a este m�dulo
	MD_Bus_TRDY : OUT STD_LOGIC; -- para avisar de que se va a realizar la operaci�n solicitada en el ciclo actual
	MD_send_data : OUT STD_LOGIC; -- para enviar los datos al bus
	MD_Dout : OUT STD_LOGIC_VECTOR (31 DOWNTO 0) -- salida de datos
);
END MD_scratch;

ARCHITECTURE Behavioral OF MD_scratch IS
	-- misma memoria que en el proyecto anterior
	COMPONENT RAM_64_32 IS PORT (
		CLK : IN STD_LOGIC;
		enable : IN STD_LOGIC; --solo se lee o escribe si enable est� activado
		ADDR : IN STD_LOGIC_VECTOR (31 DOWNTO 0); --Dir 
		Din : IN STD_LOGIC_VECTOR (31 DOWNTO 0);--entrada de datos para el puerto de escritura
		WE : IN STD_LOGIC; -- write enable	
		RE : IN STD_LOGIC; -- read enable		  
		Dout : OUT STD_LOGIC_VECTOR (31 DOWNTO 0));
	END COMPONENT;

	COMPONENT reg32 IS
		PORT (
			Din : IN STD_LOGIC_VECTOR (31 DOWNTO 0);
			clk : IN STD_LOGIC;
			reset : IN STD_LOGIC;
			load : IN STD_LOGIC;
			Dout : OUT STD_LOGIC_VECTOR (31 DOWNTO 0));
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

	SIGNAL MD_Scratch_enable, load_reg, load_addr, addr_in_range, BUS_RE, BUS_WE, Scratch_RE, Scratch_WE : STD_LOGIC;
	SIGNAL addr_reg : STD_LOGIC_VECTOR (6 DOWNTO 0);
	SIGNAL MD_Scratch_Dout_int, MD_scratch_addr : STD_LOGIC_VECTOR (31 DOWNTO 0);
	TYPE state_type IS (Inicio, Acceso, Envio);
	SIGNAL state, next_state : state_type;
	SIGNAL load_control, Internal_read, Internal_write : STD_LOGIC; --signals to store inputs bus_read, and bus_write
BEGIN
	---------------------------------------------------------------------------
	-- Decodificador: identifica cuando la direcci�n pertenece a la MD: (X"10000000"-X"100000FF")
	---------------------------------------------------------------------------
	-- Se activa cuando el bus quiere realizar una operaci�n (bus_read o bus_write = '1') y la direcci�n est� en el rango
	addr_in_range <= '1' WHEN (Bus_Addr(31 DOWNTO 8) = "000100000000000000000000") AND ((bus_Read = '1') OR (bus_Write = '1'))ELSE
		'0';
	---------------------------------------------------------------------------
	-- Registro que almacena las se�ales de control del bus
	---------------------------------------------------------------------------

	Read_Write_register : PROCESS (clk)
	BEGIN
		IF (clk'event AND clk = '1') THEN
			IF (reset = '1') THEN
				Internal_read <= '0';
				Internal_write <= '0';
			ELSIF load_control = '1' THEN
				Internal_read <= bus_Read;
				Internal_write <= bus_write;
			END IF;
		END IF;
	END PROCESS;

	---------------------------------------------------------------------------
	-- Memoria de datos de 64 palabras de 32 bits
	---------------------------------------------------------------------------

	MD_scratch_64_32 : RAM_64_32 PORT MAP(CLK => CLK, enable => MD_Scratch_enable, ADDR => MD_scratch_addr, Din => Bus_Data, WE => Scratch_WE, RE => Scratch_RE, Dout => MD_Scratch_Dout_int);

	--La salida de la scratch se guarda en un registro
	output_reg : reg GENERIC MAP(size => 32)
	PORT MAP(Din => MD_Scratch_Dout_int, clk => clk, reset => reset, load => load_reg, Dout => MD_Dout);
	-- registro para almacenar la @(es de 7 bits aunque para esta memoria basta con 6)
	reg_addr : reg GENERIC MAP(size => 7)
	PORT MAP(Din => Bus_Addr(8 DOWNTO 2), CLK => CLK, reset => reset, load => load_addr, Dout => addr_reg);
	-- @ para la Scratch (en esta memoria son s�lo 6 bits)
	-- s�lo asignamos los bits que se usan. El resto se quedan a 0.
	MD_scratch_addr(7 DOWNTO 2) <= addr_reg(5 DOWNTO 0);
	MD_scratch_addr(1 DOWNTO 0) <= "00";
	MD_scratch_addr(31 DOWNTO 8) <= "000000000000000000000000";
	---------------------------------------------------------------------------
	-- M�quina de estados para de la memoria scratch
	---------------------------------------------------------------------------

	SYNC_PROC : PROCESS (clk)
	BEGIN
		IF (clk'event AND clk = '1') THEN
			IF (reset = '1') THEN
				state <= Inicio;
			ELSE
				state <= next_state;
			END IF;
		END IF;
	END PROCESS;
	--Mealy State-Machine - Outputs based on state and inputs
	----------------------------------------------------------------------------
	-- Esta m�quina de estados gestiona load_reg, load_control, MD_Bus_TRDY, y MD_send_data
	---------------------------------------------------------------------------
	OUTPUT_DECODE : PROCESS (state, addr_in_range, Internal_read, Internal_write)
	BEGIN
		-- valores por defecto, si no se asigna otro valor en un estado valdr�n lo que se asigna aqu�

		MD_Bus_DEVsel <= '0';
		MD_Scratch_enable <= '0';
		MD_Bus_TRDY <= '0';
		MD_send_data <= '0';
		load_reg <= '0';
		load_addr <= '0';
		load_control <= '0';
		Scratch_RE <= '0';
		Scratch_WE <= '0';
		next_state <= Inicio; --Por defecto nos quedamos en inicio
		-- Estado inicial: Espera   
		CASE state IS
			WHEN Inicio =>
				-- si la direcci�n no est� en rango no hay que hacer nada
				IF (addr_in_range = '0') THEN
					next_state <= Inicio;
				ELSIF (addr_in_range = '1') THEN -- si detectamos que la direcci�n nos pertenece la guardamos
					next_state <= Acceso;
					MD_Bus_DEVsel <= '1';
					load_addr <= '1';
					load_control <= '1';
				END IF;
			WHEN Acceso =>

				MD_Scratch_enable <= '1';
				IF (Internal_write = '1') THEN -- la escritura se hace en el ciclo actual
					MD_Bus_TRDY <= '1';
					next_state <= Inicio;
					Scratch_WE <= '1';
				ELSIF (Internal_read = '1') THEN
					next_state <= Envio;
					load_reg <= '1'; --La salida de la memoria se carga en el registro para enviarla en el ciclo siguiente
					Scratch_RE <= '1';
				ELSE --No deber�a pasar, pero si no hay que hacer nada nos volvemos a Inicio
					next_state <= Inicio;
				END IF;
			WHEN Envio =>
				MD_Bus_TRDY <= '1';
				MD_send_data <= '1';
				next_state <= Inicio;
			WHEN OTHERS => next_state <= Inicio;--No deberia pasar nunca
		END CASE;
	END PROCESS;

END Behavioral;