LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
-- Este m�dulo monitoriza una entrada del sistema y la escribe a trav�s del bus en una direcci�n de memoria de la Scrtach
-- EN la versi�n actual intenta escribir siempre (IO_M_Req ='1') el dato que recibe (IO_input) que representa una entrada del sistema. 
-- Lo escribe en la palabra 0 de la scratch. De esta forma el procesador puede acceder al valor de la entrada 
ENTITY IO_master IS
	PORT (
		clk : IN STD_LOGIC;
		reset : IN STD_LOGIC;
		IO_M_bus_Grant : IN STD_LOGIC;
		IO_input : IN STD_LOGIC_VECTOR (31 DOWNTO 0);
		bus_TRDY : IN STD_LOGIC; --indica que el esclavo no puede realizar la operaci�n solicitada en este ciclo
		Bus_DevSel : IN STD_LOGIC; --indica que el esclavo ha reconocido que la direcci�n est� dentro de su rango
		IO_M_ERROR : OUT STD_LOGIC; -- Se activa si el esclavo no responde a su direcci�n
		IO_M_Req : OUT STD_LOGIC;
		IO_M_Read : OUT STD_LOGIC;
		IO_M_Write : OUT STD_LOGIC;
		IO_M_bus_Frame : OUT STD_LOGIC;
		IO_M_send_Addr : OUT STD_LOGIC;
		IO_M_send_Data : OUT STD_LOGIC;
		IO_M_last_word : OUT STD_LOGIC;
		IO_M_Addr : OUT STD_LOGIC_VECTOR (31 DOWNTO 0);
		IO_M_Data : OUT STD_LOGIC_VECTOR (31 DOWNTO 0));
END IO_master;
ARCHITECTURE Behavioral OF IO_master IS

	TYPE state_type IS (Inicio, send_data);
	TYPE error_type IS (addr_error, No_error);
	SIGNAL state, next_state : state_type;
	SIGNAL error_state, next_error_state : error_type;
	SIGNAL internal_IO_M_send_Addr : STD_LOGIC;
BEGIN
	-- maquina de estados del m�ster
	---------------------------------------------------------------------------
	-- M�quina de estados para de la memoria scratch
	---------------------------------------------------------------------------

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

	---------------------------------------------------------------------------
	-- 2023
	-- M�quina de estados para el bit de error
	---------------------------------------------------------------------------

	Error_reg : PROCESS (clk)
	BEGIN
		IF (clk'event AND clk = '1') THEN
			IF (reset = '1') THEN
				error_state <= No_error;
			ELSE
				error_state <= next_error_state;
			END IF;
		END IF;
	END PROCESS;
	--Mealy State-Machine - Outputs based on state and inputs
	----------------------------------------------------------------------------
	-- Esta m�quina de estados gestiona la escritura de la entrada IO en la �ltima palabra de la Scratch
	---------------------------------------------------------------------------
	OUTPUT_DECODE : PROCESS (state, IO_M_bus_Grant, bus_TRDY, Bus_DevSel)
	BEGIN
		-- valores por defecto, si no se asigna otro valor en un estado valdr�n lo que se asigna aqu�

		IO_M_Req <= '0'; -- si no lo tenemos pedimos el bus
		IO_M_bus_Frame <= '0';
		IO_M_send_Addr <= '0';
		IO_M_send_Data <= '0';
		IO_M_last_word <= '0';
		IO_M_Read <= '0'; -- nunca lee
		IO_M_Write <= '0';
		IO_M_Addr <= X"10000000"; --Mandamos siempre la misma @ la direcci�n 0 de la scratch. De esta forma el procesador puede leer la entrada externa
		IO_M_Data <= IO_input; -- escribe el dato de la entrada	
		next_state <= Inicio; --Por defecto nos quedamos en inicio
		next_error_state <= error_state; -- por defecto mantenemos el valor de error
		-- Estado inicial: Espera   
		IF (state = Inicio) THEN
			IO_M_Req <= '1';
			IF (IO_M_bus_Grant = '0') THEN -- si no nos dan el bus no hacemos nada
				next_state <= Inicio;
			ELSE -- si nos dan el bus enviamos la direcci�n
				IO_M_send_Addr <= '1';
				IO_M_Write <= '1';
				IF (Bus_DevSel = '1') THEN --si el esclavo reconoce la direcci�n pasamos a la fase de datos
					next_state <= send_data;
					next_error_state <= No_error; -- marcamos que el esclavo ha reconocido la �ltima direcci�n
				ELSE
					next_error_state <= addr_error; -- marcamos que el esclavo no ha reconocido la �ltima direcci�n	
					next_state <= Inicio;-- Si el esclavo no responde, damos por cerrada la transferencia
				END IF;
			END IF;
		ELSIF state = send_data THEN
			IO_M_send_Addr <= '0';
			IO_M_Write <= '1';
			IO_M_bus_Frame <= '1'; --activamos bus_frame para deshabilitar al �rbitro
			IO_M_send_Data <= '1'; --enviamos el dato de la entrada	
			IO_M_last_word <= '1'; --indicamos que es la �ltima palabra			
			IF (bus_TRDY = '1') THEN --si el esclavo est� preparado se realiza la escritura en la scratch
				next_state <= inicio;
			ELSE
				next_state <= send_data;
			END IF;
		END IF;
	END PROCESS;
	IO_M_ERROR <= '1' WHEN (error_state = addr_error) ELSE
		'0';

END Behavioral;