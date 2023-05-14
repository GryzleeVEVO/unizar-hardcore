
LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.numeric_std.ALL;

ENTITY reg IS
	GENERIC (size : NATURAL := 32); -- por defecto son de 32 bits, pero se puede usar cualquier tama�o
	PORT (
		Din : IN STD_LOGIC_VECTOR (size - 1 DOWNTO 0);
		clk : IN STD_LOGIC;
		reset : IN STD_LOGIC;
		load : IN STD_LOGIC;
		Dout : OUT STD_LOGIC_VECTOR (size - 1 DOWNTO 0));
END REG;

ARCHITECTURE behavioral OF REG IS

	SIGNAL data : STD_LOGIC_VECTOR(size - 1 DOWNTO 0);

BEGIN

	PROCESS (clk)
	BEGIN
		IF (rising_edge(clk)) THEN

			IF (reset = '1') THEN
				data <= (OTHERS => '0');
			ELSIF (load = '1') THEN
				data <= Din;
			END IF;

		END IF;

	END PROCESS;

	Dout <= data;

END behavioral; -- arch