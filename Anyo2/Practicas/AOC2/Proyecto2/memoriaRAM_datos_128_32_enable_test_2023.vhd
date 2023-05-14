----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    14:12:11 04/04/2014 
-- Design Name: 
-- Module Name:    memoriaRAM - Behavioral 
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
----------------------------------------------------------------------------------
LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.ALL;
USE ieee.numeric_std.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;
-- Memoria RAM de 128 oalabras de 32 bits
ENTITY RAM_128_32 IS PORT (
	CLK : IN STD_LOGIC;
	ADDR : IN STD_LOGIC_VECTOR (31 DOWNTO 0); --Dir 
	Din : IN STD_LOGIC_VECTOR (31 DOWNTO 0);--entrada de datos para el puerto de escritura
	WE : IN STD_LOGIC; -- write enable	
	RE : IN STD_LOGIC; -- read enable		  
	enable : IN STD_LOGIC; --solo se lee o escribe si enable est� activado
	Dout : OUT STD_LOGIC_VECTOR (31 DOWNTO 0));
END RAM_128_32;

ARCHITECTURE Behavioral OF RAM_128_32 IS
	TYPE RamType IS ARRAY(0 TO 127) OF STD_LOGIC_VECTOR(31 DOWNTO 0);

	-- Datos test 1
	--SIGNAL RAM : RamType := (
	--	--   +0     	 +4 		  +8 		   +c 		
	--	X"10000080", X"01000000", X"00000000", X"00000000", -- 0x0
	--	X"00000058", X"00000000", X"00000000", X"00000000", -- 0x10
	--	X"00000010", X"00000000", X"00000000", X"00000000", -- 0x20
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0x30
	--	X"00000040", X"00000044", X"00000000", X"00000000", -- 0x40
	--	X"00000000", X"00000000", X"00000044", X"00000000", -- 0x50
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0x60
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0x70
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0x80
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0x90
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0xa0
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0xb0
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0xc0
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0xd0
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0xe0
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0xf0
	--	X"00000000", X"00000000", X"00000108", X"0000010C", -- 0x100
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0x110
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0x120
	--	X"00000000", X"00000000", X"00000138", X"00000000", -- 0x130
	--	X"00000000", X"00000000", X"00000148", X"0000014c", -- 0x140
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0x150
	--	X"0000004f", X"0000004b", X"00000000", X"00000000", -- 0x160
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0x170
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0x180
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0x190
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0x1a0
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0x1b0
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0x1c0
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0x1d0
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0x1e0
	--	X"00000000", X"00000000", X"00000000", X"00000000" -- 0x1f0
	--);

		-- Datos test 2
	--SIGNAL RAM : RamType := (
	--	--   +0     	 +4 		  +8 		   +c 		
	--	X"00000100", X"00000000", X"00000001", X"00000008", -- 0x0
	--	X"00000018", X"00000000", X"00000001", X"0000000f", -- 0x10   
	--	X"00000049", X"00000021", X"00000000", X"00000000", -- 0x20
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0x30  
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0x40
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0x50  
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0x60
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0x70  
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0x80
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0x90  
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0xa0
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0xb0  
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0xc0
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0xd0  
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0xe0
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0xf0  
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0x100
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0x110 
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0x120
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0x130 
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0x140
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0x150 
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0x160
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0x170 
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0x180
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0x190 
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0x1a0
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0x1b0 
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0x1c0
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0x1d0 
	--	X"00000000", X"00000000", X"00000000", X"00000000", -- 0x1e0
	--	X"00000000", X"00000000", X"00000000", X"00000000" -- 0x1f0
	--);


	SIGNAL dir_7 : STD_LOGIC_VECTOR(6 DOWNTO 0);
BEGIN

	dir_7 <= ADDR(8 DOWNTO 2); -- como la memoria es de 128 plalabras no usamos la direcci�n completa sino s�lo 7 bits. Como se direccionan los bytes, pero damos palabras no usamos los 2 bits menos significativos
	PROCESS (CLK)
	BEGIN
		IF (CLK'event AND CLK = '1') THEN
			IF (WE = '1') AND (enable = '1') THEN -- s�lo se escribe si WE vale 1
				RAM(conv_integer(dir_7)) <= Din;
				REPORT "Simulation time : " & TIME'IMAGE(now) & ".  Data written: " & INTEGER'image(to_integer(unsigned(Din))) & ", in ADDR = " & INTEGER'image(to_integer(unsigned(ADDR)));
			END IF;
		END IF;
	END PROCESS;

	Dout <= RAM(conv_integer(dir_7)) WHEN ((RE = '1') AND (enable = '1')) ELSE
		"00000000000000000000000000000000"; --s�lo se lee si RE vale 1

END Behavioral;