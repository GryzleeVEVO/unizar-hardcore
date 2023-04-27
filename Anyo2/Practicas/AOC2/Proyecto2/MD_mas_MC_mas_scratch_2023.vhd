----------------------------------------------------------------------------------
--
-- Description: Este m�dulo sustituye a la memoria de datos del mips. Incluye un memoria cache que se conecta a trav�s de un bus a memoria principal
-- el interfaz a�ade una se�al nueva (Mem_ready) que indica si la MC podr� ralizar la operaci�n en el ciclo actual
----------------------------------------------------------------------------------
LIBRARY IEEE;
USE ieee.std_logic_1164.ALL;
USE ieee.numeric_std.ALL;
USE IEEE.std_logic_arith.ALL;
USE IEEE.std_logic_unsigned.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;
-- Memoria RAM de 128 palabras de 32 bits
ENTITY MD_mas_MC IS PORT (
	CLK : IN STD_LOGIC;
	reset : IN STD_LOGIC;
	ADDR : IN STD_LOGIC_VECTOR (31 DOWNTO 0); --Dir solicitada por el Mips
	Din : IN STD_LOGIC_VECTOR (31 DOWNTO 0);--entrada de datos desde el Mips
	WE : IN STD_LOGIC; -- write enable	del MIPS
	RE : IN STD_LOGIC; -- read enable del MIPS	
	IO_input : IN STD_LOGIC_VECTOR (31 DOWNTO 0); --dato que viene de una entrada del sistema
	Mem_ready : OUT STD_LOGIC; -- indica si podemos hacer la operaci�n solicitada en el ciclo actual
	Data_abort : OUT STD_LOGIC; --indica que el �ltimo acceso a memoria ha sido un error
	Dout : OUT STD_LOGIC_VECTOR (31 DOWNTO 0) --dato que se env�a al Mips
);
END MD_mas_MC;

ARCHITECTURE Behavioral OF MD_mas_MC IS
	-- Memoria de datos con su controlador de bus
	COMPONENT MD_cont IS PORT (
		CLK : IN STD_LOGIC;
		reset : IN STD_LOGIC;
		Bus_Frame : IN STD_LOGIC; -- indica que el master quiere m�s datos
		bus_last_word : IN STD_LOGIC; --indica que es el �ltimo dato de la transferencia
		bus_Read : IN STD_LOGIC;
		bus_Write : IN STD_LOGIC;
		Bus_Addr : IN STD_LOGIC_VECTOR (31 DOWNTO 0); --Direcciones 
		Bus_Data : IN STD_LOGIC_VECTOR (31 DOWNTO 0); --Datos 
		MD_Bus_DEVsel : OUT STD_LOGIC; -- para avisar de que se ha reconocido que la direcci�n pertenece a este m�dulo
		MD_Bus_TRDY : OUT STD_LOGIC; -- para avisar de que se va a realizar la operaci�n solicitada en el ciclo actual
		MD_send_data : OUT STD_LOGIC; -- para enviar los datos al bus
		MD_Dout : OUT STD_LOGIC_VECTOR (31 DOWNTO 0) -- salida de datos
		);
	END COMPONENT;

	-- MemoriaCache de datos
	COMPONENT MC_datos IS PORT (
		CLK : IN STD_LOGIC;
		reset : IN STD_LOGIC;
		--Interfaz con el MIPS
		ADDR : IN STD_LOGIC_VECTOR (31 DOWNTO 0); --Dir 
		Din : IN STD_LOGIC_VECTOR (31 DOWNTO 0);
		RE : IN STD_LOGIC; -- read enable		
		WE : IN STD_LOGIC;
		ready : OUT STD_LOGIC; -- indica si podemos hacer la operaci�n solicitada en el ciclo actual
		Dout : OUT STD_LOGIC_VECTOR (31 DOWNTO 0); --dato que se env�a al Mips
		-- Nueva se�al de error
		Mem_ERROR : OUT STD_LOGIC; -- Se activa si en la ultima transferencia el esclavo no respondi� a su direcci�n
		--Interfaz con el bus
		MC_Bus_Req : OUT STD_LOGIC; --indica que la MC quiere usar el bus;
		MC_Bus_Grant : IN STD_LOGIC; --indica que el �rbitro permite usar el bus a la MC;
		MC_Bus_Din : IN STD_LOGIC_VECTOR (31 DOWNTO 0);--para leer datos del bus
		Bus_TRDY : IN STD_LOGIC; --indica que el esclavo (la memoriade datos) puede realizar la operaci�n solicitada en este ciclo
		Bus_DevSel : IN STD_LOGIC; --indica que la memoria ha reconocido que la direcci�n est� dentro de su rango
		MC_send_addr_ctrl : OUT STD_LOGIC; --ordena que se env�en la direcci�n y las se�ales de control al bus
		MC_send_data : OUT STD_LOGIC; --ordena que se env�en los datos
		MC_frame : OUT STD_LOGIC; --indica que la operaci�n no ha terminado
		MC_last_word : OUT STD_LOGIC; --indica que es el �ltimo dato de la transferencia
		MC_Bus_ADDR : OUT STD_LOGIC_VECTOR (31 DOWNTO 0); --Dir 
		MC_Bus_data_out : OUT STD_LOGIC_VECTOR (31 DOWNTO 0);--para enviar datos por el bus
		MC_bus_Rd_Wr : OUT STD_LOGIC --'0' para lectura,  '1' para escritura
		);
	END COMPONENT;
	-- Memoria scratch (Memoria r�pida que contesta en el ciclo en el que se le pide algo)
	-- S�lo tiene acceso palabra a palabra
	COMPONENT MD_scratch IS PORT (
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
	END COMPONENT;

	COMPONENT Arbitro IS
		PORT (
			clk : IN STD_LOGIC;
			reset : IN STD_LOGIC;
			last_word : IN STD_LOGIC; -- Cuando termina una transferencia cambiamos las prioridades
			bus_frame : IN STD_LOGIC;-- para saber que hay una transferenica en curso
			Bus_TRDY : IN STD_LOGIC; --para saber que la ultima transferencia va a realizarse este ciclo
			Req0 : IN STD_LOGIC; -- hay dos solicitudes
			Req1 : IN STD_LOGIC;
			Grant0 : OUT STD_LOGIC;
			Grant1 : OUT STD_LOGIC);
	END COMPONENT;

	COMPONENT IO_Master IS
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
	END COMPONENT;

	COMPONENT counter IS
		GENERIC (size : INTEGER := 10);
		PORT (
			clk : IN STD_LOGIC;
			reset : IN STD_LOGIC;
			count_enable : IN STD_LOGIC;
			count : OUT STD_LOGIC_VECTOR (size - 1 DOWNTO 0));
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

	--se�ales del bus
	SIGNAL Bus_Data_Addr : STD_LOGIC_VECTOR(31 DOWNTO 0);
	SIGNAL Bus_TRDY, Bus_Devsel, bus_Read, bus_Write, Bus_Frame, Bus_last_word : STD_LOGIC;
	--se�ales de MC
	SIGNAL MC_Bus_Din, MC_Bus_ADDR, MC_Bus_data_out : STD_LOGIC_VECTOR (31 DOWNTO 0);
	SIGNAL MC_send_addr_ctrl, MC_send_data, MC_frame, MC_bus_Rd_Wr, MC_last_word : STD_LOGIC;
	--se�ales de MD_scratch
	SIGNAL MD_scratch_Dout : STD_LOGIC_VECTOR(31 DOWNTO 0);
	SIGNAL MD_scratch_Bus_DEVsel, MD_scratch_send_data, MD_scratch_Bus_TRDY : STD_LOGIC;
	--se�ales de MD
	SIGNAL MD_Dout : STD_LOGIC_VECTOR(31 DOWNTO 0);
	SIGNAL MD_Bus_DEVsel, MD_send_data, MD_Bus_TRDY : STD_LOGIC;
	-- se�ales para el arbitraje
	SIGNAL MC_Bus_Grant, MC_Bus_Req : STD_LOGIC;
	SIGNAL IO_M_bus_Grant, IO_M_Req : STD_LOGIC;-- se�ales para simular otros dispositivos que solicitan el bus
	--se�ales del m�ster de IO
	SIGNAL IO_M_Addr, IO_M_Data : STD_LOGIC_VECTOR(31 DOWNTO 0);
	SIGNAL IO_M_read, IO_M_write, IO_M_last_word, IO_M_bus_Frame, IO_M_send_Addr, IO_M_send_Data, IO_M_ERROR : STD_LOGIC;
	--se�ales de monitorizaci�n
	SIGNAL IO_M_count : STD_LOGIC_VECTOR (7 DOWNTO 0);
	-- se�ales de error
	SIGNAL Mem_ERROR : STD_LOGIC;
BEGIN
	------------------------------------------------------------------------------------------------
	--   MC de datos
	------------------------------------------------------------------------------------------------

	MC : MC_datos PORT MAP(
		clk => clk, reset => reset, ADDR => ADDR, Din => Din, RE => RE, WE => WE, ready => Mem_ready, Dout => Dout,
		MC_Bus_Din => MC_Bus_Din, Bus_TRDY => Bus_TRDY, Bus_DevSel => Bus_DevSel, MC_send_addr_ctrl => MC_send_addr_ctrl,
		MC_send_data => MC_send_data, MC_frame => MC_frame, MC_Bus_ADDR => MC_Bus_ADDR, MC_Bus_data_out => MC_Bus_data_out,
		MC_Bus_Req => MC_Bus_Req, MC_Bus_Grant => MC_Bus_Grant, MC_bus_Rd_Wr => MC_bus_Rd_Wr, MC_last_word => MC_last_word,
		Mem_ERROR => Mem_ERROR);

	------------------------------------------------------------------------------------------------	
	-- Controlador de MD
	------------------------------------------------------------------------------------------------
	controlador_MD : MD_cont PORT MAP(
		CLK => CLK,
		reset => reset,
		Bus_Frame => Bus_Frame,
		bus_last_word => bus_last_word,
		bus_Read => bus_Read,
		bus_Write => bus_Write,
		Bus_Addr => Bus_Data_Addr,
		Bus_data => Bus_Data_Addr,
		MD_Bus_DEVsel => MD_Bus_DEVsel,
		MD_Bus_TRDY => MD_Bus_TRDY,
		MD_send_data => MD_send_data,
		MD_Dout => MD_Dout
	);

	------------------------------------------------------------------------------------------------	
	-- Memoria Scratch de datos
	------------------------------------------------------------------------------------------------
	M_scratch : MD_scratch PORT MAP(
		CLK => CLK,
		reset => reset,
		Bus_Frame => Bus_Frame,
		bus_Read => bus_Read,
		bus_Write => bus_Write,
		Bus_Addr => Bus_Data_Addr,
		Bus_data => Bus_Data_Addr,
		MD_Bus_DEVsel => MD_scratch_Bus_DEVsel,
		MD_Bus_TRDY => MD_scratch_Bus_TRDY,
		MD_send_data => MD_Scratch_send_data,
		MD_Dout => MD_Scratch_Dout
	);

	------------------------------------------------------------------------------------------------	 
	MC_Bus_Din <= Bus_Data_Addr;
	------------------------------------------------------------------------------------------------	 
	------------------------------------------------------------------------------------------------
	--   	BUS: l�neas compartidas y buffers triestado (cuando no se manda nada queda en estado "Z" de alta impedancia)
	--		o OR cableada (cuando no se env�a nada, el estado por defecto es "0")
	--		MC actua de m�ster, MD de slave. Y simulamos que hay otros dispositivos haciendo cosas en el bus con "others". 
	--		Estos otros dispositivos intentan usar el bus todo el tiempo, pero el �rbitro va asignanado prioridades con un round-robin
	------------------------------------------------------------------------------------------------
	-- Data: Tres fuentes de datos: MC, MD, y "others" 
	Bus_Data_Addr <= MC_Bus_data_out WHEN MC_send_data = '1' ELSE
		MD_Dout WHEN MD_send_data = '1' ELSE
		MD_Scratch_Dout WHEN MD_Scratch_send_data = '1' ELSE
		IO_M_Data WHEN IO_M_send_Data = '1' ELSE
		"ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ";
	-- Addr: la direcci�n la env�a MC o "others"
	Bus_Data_Addr <= MC_Bus_ADDR WHEN (MC_send_addr_ctrl = '1') ELSE
		IO_M_Addr WHEN (IO_M_send_Addr = '1') ELSE
		"ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ";
	Bus_Data_Addr <= x"00000000" WHEN ((MC_send_data = '0') AND (MD_send_data = '0') AND (MD_Scratch_send_data = '0') AND (IO_M_send_Data = '0') AND (MC_send_addr_ctrl = '0') AND (IO_M_send_Addr = '0')) ELSE
		"ZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZZ";
	-- Ponemos un 00000000 como valor por defecto para que el simulador no saque sin parar mensajes diciendo que hay se�ales con valor 'Z'						
	--Control
	--********************************************************************
	bus_Read <= NOT(MC_bus_Rd_Wr) WHEN (MC_send_addr_ctrl = '1') ELSE
		IO_M_read WHEN (IO_M_send_Addr = '1') ELSE--en este ejemplo "others" nunca lee
		'0';
	bus_Write <= MC_bus_Rd_Wr WHEN (MC_send_addr_ctrl = '1') ELSE
		IO_M_write WHEN (IO_M_send_Addr = '1') ELSE--"others" escribe en una direcci�n fuera de rango. Es s�lo para que se vea cuando usa el bus
		'0';

	Bus_Frame <= MC_frame OR IO_M_bus_Frame; --el bus est� ocupado si cualquiera de los dos m�sters lo est� usando

	Bus_last_word <= MC_last_word WHEN (MC_frame = '1') ELSE
		IO_M_last_word WHEN (IO_M_bus_Frame = '1') ELSE
		'0';

	-- Se�ales de las memorias	
	Bus_DevSel <= MD_Bus_DEVsel OR MD_scratch_Bus_DEVsel;
	Bus_TRDY <= MD_Bus_TRDY OR MD_scratch_Bus_TRDY;

	-- �rbitraje

	Arbitraje : arbitro PORT MAP(
		clk => clk, reset => reset, Req0 => MC_Bus_Req, Req1 => IO_M_Req, Grant0 => MC_Bus_Grant, Grant1 => IO_M_bus_Grant,
		Bus_Frame => Bus_Frame, Bus_TRDY => Bus_TRDY, last_word => Bus_last_word);
	------------------------------------------------------------------------------------------------	
	-- este contador nos dice cuantos ciclos han podido usar el m�ster de IO. 
	-- Su objetivo es ver si liberamos el bus en cuanto se puede o si lo retenemos m�s de la cuenta

	cont_IO : counter GENERIC MAP(size => 8)
	PORT MAP(clk => clk, reset => reset, count_enable => IO_M_bus_Grant, count => IO_M_count);
	------------------------------------------------------------------------------------------------	
	-- Modulo_IO: una y otra vez escribe lo que haya en la entrada IO_M_input en la �ltima palabra de la Scratch. Es una forma de hacer visible al procesador una entrada externa
	-- Lo m�s habitual ser� que tuviese un registro direccionable, y que actuase como esclavo en el bus, en lugar de como m�ster. Pero lo hemos hecho as� para que haya dos m�sters que compitan por el bus
	Modulo_IO : IO_Master PORT MAP(
		clk => clk, reset => reset,
		IO_M_Req => IO_M_Req,
		IO_M_bus_Grant => IO_M_bus_Grant,
		IO_M_bus_Frame => IO_M_bus_Frame,
		IO_input => io_input,
		IO_M_read => IO_M_read,
		IO_M_write => IO_M_write,
		IO_M_Addr => IO_M_Addr,
		IO_M_Data => IO_M_Data,
		IO_M_ERROR => IO_M_ERROR,
		bus_trdy => bus_trdy,
		Bus_DevSel => Bus_DevSel,
		IO_M_send_Addr => IO_M_send_Addr,
		IO_M_send_Data => IO_M_send_Data,
		IO_M_last_word => IO_M_last_word);
	------------------------------------------------------------------------------------------------	
	-- Data abort 
	Data_abort <= Mem_Error; --Si la memoria da un error avisamos al procesador activando data_abort

END Behavioral;