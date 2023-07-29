LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
--Mux 4 a 1
ENTITY mux4_1_32bits IS
	PORT (
		DIn0 : IN STD_LOGIC_VECTOR (31 DOWNTO 0);
		DIn1 : IN STD_LOGIC_VECTOR (31 DOWNTO 0);
		DIn2 : IN STD_LOGIC_VECTOR (31 DOWNTO 0);
		DIn3 : IN STD_LOGIC_VECTOR (31 DOWNTO 0);
		ctrl : IN STD_LOGIC_VECTOR(1 DOWNTO 0);
		Dout : OUT STD_LOGIC_VECTOR (31 DOWNTO 0));
END mux4_1_32bits;
ARCHITECTURE Behavioral OF mux4_1_32bits IS
BEGIN
	Dout <= DIn0 WHEN ctrl = "00" ELSE
		DIn1 WHEN ctrl = "01" ELSE
		DIn2 WHEN ctrl = "10" ELSE
		DIn3;
END Behavioral;