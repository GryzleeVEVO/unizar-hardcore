-- AOC2
-- Proyecto 1
-- Dorian Boleslaw Wozniak (817570@unizar.es)
-- Adrian Arribas Mateo (795593@unizar.es)
-- Memoria de datos

LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;
-- Memoria RAM de 128 oalabras de 32 bits
ENTITY Data_Memory_Subsystem IS PORT (
	CLK : IN STD_LOGIC;
	reset : IN STD_LOGIC;
	ADDR : IN STD_LOGIC_VECTOR (31 DOWNTO 0); --Dir solicitada por el Mips
	Din : IN STD_LOGIC_VECTOR (31 DOWNTO 0);--entrada de datos desde el Mips
	WE : IN STD_LOGIC; -- write enable	del MIPS
	RE : IN STD_LOGIC; -- read enable del MIPS	
	IO_input : IN STD_LOGIC_VECTOR (31 DOWNTO 0); --dato que viene de una entrada del sistema. En esta memoria no se usa
	Mem_ready : OUT STD_LOGIC; -- indica si podemos hacer la operaci�n solicitada en el ciclo actual. En esta memoria vale siempre '1'.
	Data_abort : OUT STD_LOGIC; --indica que el �ltimo acceso a memoria ha sido un error
	Dout : OUT STD_LOGIC_VECTOR (31 DOWNTO 0) --dato que se env�a al Mips
);
END Data_Memory_Subsystem;

ARCHITECTURE Behavioral OF Data_Memory_Subsystem IS
	TYPE RamType IS ARRAY(0 TO 127) OF STD_LOGIC_VECTOR(31 DOWNTO 0);
	--- INICIO CONTENIDOS MEMORIA ---

	-- Datos Codigo retardado / IRQ / Test 1
	--SIGNAL RAM : RamType := (
	--	--   +0           +4           +8           +c          +10          +14          +18          +1c
	--	X"00000100", X"00000001", X"00000008", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", -- 0x0  
	--	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", -- 0x20 
	--	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", -- 0x40 
	--	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", -- 0x60 
	--	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", -- 0x80 
	--	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", -- 0xa0 
	--	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", -- 0xc0 
	--	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", -- 0xe0 
	--	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", -- 0x100
	--	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", -- 0x120
	--	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", -- 0x140
	--	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", -- 0x160
	--	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", -- 0x180
	--	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", -- 0x1a0
	--	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", -- 0x1c0
	--	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000"  -- 0x1e0
	--);

	-- Datos Test 2/3
	--SIGNAL RAM : RamType := (
	--	--   +0           +4           +8           +c          +10          +14          +18          +1c
	--	X"00000000", X"00000001", X"00000002", X"00000004", X"00000008", X"00000010", X"00000000", X"00000000", -- 0x0  
	--	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", -- 0x20 
	--	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", -- 0x40 
	--	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", -- 0x60 
	--	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", -- 0x80 
	--	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", -- 0xa0 
	--	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", -- 0xc0 
	--	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", -- 0xe0 
	--	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", -- 0x100
	--	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", -- 0x120
	--	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", -- 0x140
	--	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", -- 0x160
	--	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", -- 0x180
	--	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", -- 0x1a0
	--	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", -- 0x1c0
	--	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000" -- 0x1e0
	--);

	-- Datos Test 4
	--SIGNAL RAM : RamType := (
	--	--   +0           +4           +8           +c          +10          +14          +18          +1c
	--	X"00000100", X"00000000", X"00000001", X"00000008", X"00000018", X"00000000", X"00000001", X"0000000f", -- 0x0  
	--	X"00000049", X"00000021", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", -- 0x20 
	--	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", -- 0x40 
	--	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", -- 0x60 
	--	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", -- 0x80 
	--	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", -- 0xa0 
	--	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", -- 0xc0 
	--	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", -- 0xe0 
	--	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", -- 0x100
	--	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", -- 0x120
	--	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", -- 0x140
	--	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", -- 0x160
	--	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", -- 0x180
	--	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", -- 0x1a0
	--	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", -- 0x1c0
	--	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000" -- 0x1e0
	--);
	--- FIN CONTENIDOS MEMORIA ---

	SIGNAL dir_7 : STD_LOGIC_VECTOR(6 DOWNTO 0);
	SIGNAL unaligned, out_of_range : STD_LOGIC;
BEGIN

	dir_7 <= ADDR(8 DOWNTO 2); -- como la memoria es de 128 plalabras no usamos la direcci�n completa sino s�lo 7 bits. Como se direccionan los bytes, pero damos palabras no usamos los 2 bits menos significativos
	PROCESS (CLK)
	BEGIN
		IF (CLK'event AND CLK = '1') THEN
			IF (WE = '1') THEN -- s�lo se escribe si WE vale 1
				RAM(conv_integer(dir_7)) <= Din;
			END IF;
		END IF;
	END PROCESS;

	Dout <= RAM(conv_integer(dir_7)) WHEN (RE = '1') ELSE
		"00000000000000000000000000000000"; --s�lo se lee si RE vale 1
	--------------------------------------------------------------------------------------------------------------------
	-- MEMORY READY
	--------------------------------------------------------------------------------------------------------------------
	-- esta memoria est� siempre disponible, y no genera ning�n retardo adicional, por eso Mem_ready es un '1' siempre.
	Mem_ready <= '1';
	--------------------------------------------------------------------------------------------------------------------
	-- DATA ABORT
	--------------------------------------------------------------------------------------------------------------------
	-- out_of_range se activa si la direcci�n est� fuera del rango. La memoria es 128 palabras empezando en la direcci�n 0. Si pedimos una direcci�n mayor saltar� el abort
	out_of_range <= '0' WHEN (ADDR(31 DOWNTO 9) = "00000000000000000000000") ELSE
		'1';
	-- S�lo vamos a permitir direcciones alineadas. Como leemos palabras de 4 bytes estas deben estar en direcciones m�ltiplos de 4. Es decir, acaban en "00"
	unaligned <= '0' WHEN (ADDR(1 DOWNTO 0) = "00") ELSE
		'1';
	-- Hay un data abort cuando se accede a una direcci�n que no existe, o se realiza un acceso no alineado.
	-- �Pero s�lo si se est� haciendo un acceso a memoria! Si WE y RE valen cero no se est� accediendo a memoria, por tanto da igual el valor de la direcci�n
	Data_abort <= (out_of_range OR unaligned) AND (WE OR RE);

END Behavioral;