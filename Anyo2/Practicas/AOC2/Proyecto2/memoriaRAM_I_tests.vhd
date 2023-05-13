-- AOC2
-- Proyecto 1
-- Dorian Boleslaw Wozniak (817570@unizar.es)
-- Adrian Arribas Mateo (795593@unizar.es)
-- Memoria de instrucciones

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

ENTITY memoriaRAM_I IS PORT (
    CLK : IN STD_LOGIC;
    ADDR : IN STD_LOGIC_VECTOR (31 DOWNTO 0); --Dir 
    Din : IN STD_LOGIC_VECTOR (31 DOWNTO 0);--entrada de datos para el puerto de escritura
    WE : IN STD_LOGIC; -- write enable	
    RE : IN STD_LOGIC; -- read enable		  
    Dout : OUT STD_LOGIC_VECTOR (31 DOWNTO 0));
END memoriaRAM_I;

--************************************************************************************************************
-- Fichero con la memoria de instrucciones cargada con diversos test
--************************************************************************************************************

ARCHITECTURE Behavioral OF memoriaRAM_I IS
    TYPE RamType IS ARRAY(0 TO 127) OF STD_LOGIC_VECTOR(31 DOWNTO 0);
    --------------------------------------------------------------------------------------------------------------------------------
    -- Instruction Memory Map
    -- From Word 0 to 3: Exception Vector Table: (@ of the exception routines)
    -- 		@0: reset
    -- 		@4: IRQ
    -- 		@8: Data Abort
    -- 		@C: UNDEF
    -- From Word 4  (@010): .CODE (code of the application to execute)
    -- From Word 64 (@100): RTI (code for the IRQ)
    -- From Word 96 (@180): Data abort (code for the Data Abort exception)
    -- From Word 112(@1C0): UNDEF (code for the UNDEF exception)
    --------------------------------------------------------------------------------------------------------------------------------
    --------------------------------------------------------------------------------------------------------------------------------
    -- BANCO DE PRUEBAS PARA EL PROCESADOR BASE: 
    -- Incluye nops para eliminar los riesgos de datos y control 
    -- El c�digo se describe en Codigo_retardado
    --------------------------------------------------------------------------------------------------------------------------------

    --- INICIO CONTENIDOS MEMORIA ---

    ---- Test 4 proyecto 1
    ---- El programa calcula los 15 primeros dígitos de la sucesión de Fibonacci
    ---- Si le llega una IRQ, escribe como salida IRQ! (0x48 0x51 0x50 0x21)
    SIGNAL RAM : RamType := (
        --   +0     	 +4 		  +8 		   +c 		
        X"10210003", X"1021003E", X"1021005D", X"1021006C", -- 0x0
        X"081F0000", X"0806001C", X"08040008", X"08050004", -- 0x10
        X"10A60008", X"08020018", X"80400000", X"08010014", -- 0x20
        X"04411800", X"0C030018", X"0C020014", X"04A42800", -- 0x30
        X"1000FFF7", X"1000FFFF", X"00000000", X"00000000", -- 0x40
        X"00000000", X"00000000", X"00000000", X"00000000", -- 0x50 
        X"00000000", X"00000000", X"00000000", X"00000000", -- 0x60
        X"00000000", X"00000000", X"00000000", X"00000000", -- 0x70
        X"00000000", X"00000000", X"00000000", X"00000000", -- 0x80
        X"00000000", X"00000000", X"00000000", X"00000000", -- 0x90
        X"00000000", X"00000000", X"00000000", X"00000000", -- 0xa0
        X"00000000", X"00000000", X"00000000", X"00000000", -- 0xb0
        X"00000000", X"00000000", X"00000000", X"00000000", -- 0xc0
        X"00000000", X"00000000", X"00000000", X"00000000", -- 0xd0
        X"00000000", X"00000000", X"00000000", X"00000000", -- 0xe0
        X"00000000", X"00000000", X"00000000", X"00000000", -- 0xf0
        X"0FE10000", X"0FE20004", X"0FE30008", X"0FE4000C", -- 0x100
        X"0FE50010", X"0FE60014", X"08010010", X"07E1F800", -- 0x110
        X"08020020", X"80400000", X"08030008", X"0804000C", -- 0x120
        X"04821000", X"04431000", X"80400000", X"04431001", -- 0x130
        X"08050024", X"80400000", X"80A00000", X"07E1F801", -- 0x140
        X"0BE10000", X"0BE20004", X"0BE30008", X"0BE4000C", -- 0x150
        X"0BE50010", X"0BE60014", X"20000000", X"00000000", -- 0x160
        X"00000000", X"00000000", X"00000000", X"00000000", -- 0x170
        X"1000FFFF", X"00000000", X"00000000", X"00000000", -- 0x180
        X"00000000", X"00000000", X"00000000", X"00000000", -- 0x190
        X"00000000", X"00000000", X"00000000", X"00000000", -- 0x1a0
        X"00000000", X"00000000", X"00000000", X"00000000", -- 0x1b0
        X"1000FFFF", X"00000000", X"00000000", X"00000000", -- 0x1c0
        X"00000000", X"00000000", X"00000000", X"00000000", -- 0x1d0
        X"00000000", X"00000000", X"00000000", X"00000000", -- 0x1e0
        X"00000000", X"00000000", X"00000000", X"00000000" -- 0x1f0
    );

    -- Test 1 proyecto 2
    --SIGNAL RAM : RamType := (
    --    --   +0     	 +4 		  +8 		   +c 		
    --    X"10210003", X"1021003E", X"1021005D", X"1021006C", -- 0x0
    --    X"081F0000", X"081E0004", X"08010040", X"08020044", -- 0x10 -- INI/TEST1
    --    X"0C010088", X"0C02008C", X"0C1F00C8", X"0C1E00CC", -- 0x20
    --    X"08010108", X"0802010C", X"08030148", X"0804014C", -- 0x30
    --    X"0C010180", X"0C020184", X"0C0301c0", X"0C0401c4", -- 0x40
    --    X"08010010", X"08220000", X"04221800", X"04032000", -- 0x50 -- TEST2
    --    X"0C810000", X"04632800", X"10A50006", X"00000000", -- 0x60
    --    X"00000000", X"00000000", X"00000000", X"00000000", -- 0x70
    --    X"08A10000", X"08020131", X"0C030200", X"00000000", -- 0x80 -- TEST4
    --    X"00000000", X"00000000", X"ffffffff", X"00000000", -- 0x90
    --    X"00000000", X"00000000", X"00000000", X"00000000", -- 0xa0
    --    X"00000000", X"00000000", X"00000000", X"00000000", -- 0xb0
    --    X"00000000", X"00000000", X"00000000", X"00000000", -- 0xc0
    --    X"00000000", X"00000000", X"00000000", X"00000000", -- 0xd0
    --    X"00000000", X"00000000", X"00000000", X"00000000", -- 0xe0
    --    X"00000000", X"00000000", X"00000000", X"00000000", -- 0xf0
    --    X"0FE10000", X"0FE20004", X"0FE30008", X"0FE4000c", -- 0x100 -- RTE/TEST3
    --    X"08010020", X"07E1F800", X"08020160", X"80400000", -- 0x110
    --    X"08030164", X"80600000", X"07E1F801", X"0BE10000", -- 0x120
    --    X"0BE20004", X"0BE30008", X"0BE4000c", X"20000000", -- 0x130
    --    X"00000000", X"00000000", X"00000000", X"00000000", -- 0x140
    --    X"00000000", X"00000000", X"00000000", X"00000000", -- 0x150
    --    X"00000000", X"00000000", X"00000000", X"00000000", -- 0x160
    --    X"00000000", X"00000000", X"00000000", X"00000000", -- 0x170
    --    X"0BC10000", X"80200000", X"20000000", X"00000000", -- 0x180 -- ABORT
    --    X"00000000", X"00000000", X"00000000", X"00000000", -- 0x190
    --    X"00000000", X"00000000", X"00000000", X"00000000", -- 0x1a0
    --    X"00000000", X"00000000", X"00000000", X"00000000", -- 0x1b0
    --    X"1000FFFF", X"00000000", X"00000000", X"00000000", -- 0x1c0 -- UNDEF
    --    X"00000000", X"00000000", X"00000000", X"00000000", -- 0x1d0
    --    X"00000000", X"00000000", X"00000000", X"00000000", -- 0x1e0
    --    X"00000000", X"00000000", X"00000000", X"00000000" -- 0x1f0
    --);

    --- FIN CONTENIDOS MEMORIA --- 

    SIGNAL dir_7 : STD_LOGIC_VECTOR(6 DOWNTO 0);
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

END Behavioral;