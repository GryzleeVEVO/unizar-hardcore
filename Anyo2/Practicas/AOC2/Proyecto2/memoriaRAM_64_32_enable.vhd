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
ENTITY RAM_64_32 IS PORT (
	CLK : IN STD_LOGIC;
	ADDR : IN STD_LOGIC_VECTOR (31 DOWNTO 0); --Dir 
	Din : IN STD_LOGIC_VECTOR (31 DOWNTO 0);--entrada de datos para el puerto de escritura
	WE : IN STD_LOGIC; -- write enable	
	RE : IN STD_LOGIC; -- read enable		  
	enable : IN STD_LOGIC; --solo se lee o escribe si enable est� activado
	Dout : OUT STD_LOGIC_VECTOR (31 DOWNTO 0));
END RAM_64_32;

ARCHITECTURE Behavioral OF RAM_64_32 IS
	TYPE RamType IS ARRAY(0 TO 63) OF STD_LOGIC_VECTOR(31 DOWNTO 0);
	SIGNAL RAM : RamType := (X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", -- posiciones 0,1,2,3,4,5,6,7
	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", --posicones 8,9,...
	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000",
	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000",
	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000",
	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000",
	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000",
	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000");
	SIGNAL dir_7 : STD_LOGIC_VECTOR(6 DOWNTO 0);
BEGIN

	dir_7 <= ADDR(8 DOWNTO 2); -- como la memoria es de 128 plalabras no usamos la direcci�n completa sino s�lo 7 bits. Como se direccionan los bytes, pero damos palabras no usamos los 2 bits menos significativos
	PROCESS (CLK)
	BEGIN
		IF (CLK'event AND CLK = '1') THEN
			IF (WE = '1') AND (enable = '1') THEN -- s�lo se escribe si WE vale 1
				RAM(conv_integer(dir_7)) <= Din;
				REPORT "Simulation time : " & TIME'IMAGE(now) & ".  Data written: " & INTEGER'image(to_integer(unsigned(Din))) & ", in MD_Scratch in ADDR = " & INTEGER'image(to_integer(unsigned(ADDR)));
			END IF;
		END IF;
	END PROCESS;

	Dout <= RAM(conv_integer(dir_7)) WHEN ((RE = '1') AND (enable = '1')) ELSE
		"00000000000000000000000000000000"; --s�lo se lee si RE vale 1

END Behavioral;