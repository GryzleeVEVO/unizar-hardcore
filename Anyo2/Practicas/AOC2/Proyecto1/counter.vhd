----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    12:32:21 07/23/2014 
-- Design Name: 
-- Module Name:    contado5 - Behavioral 
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
USE IEEE.std_logic_arith.ALL;
USE IEEE.std_logic_unsigned.ALL;
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
USE IEEE.NUMERIC_STD.ALL;
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

ENTITY counter IS
   GENERIC (
      size : INTEGER := 10
   );
   PORT (
      clk : IN STD_LOGIC;
      reset : IN STD_LOGIC;
      count_enable : IN STD_LOGIC;
      count : OUT STD_LOGIC_VECTOR (size - 1 DOWNTO 0));
END counter;

ARCHITECTURE Behavioral OF counter IS
   SIGNAL count_int : STD_LOGIC_VECTOR (size - 1 DOWNTO 0);
BEGIN
   PROCESS (clk)
   BEGIN
      IF clk = '1' AND clk'event THEN
         IF reset = '1' THEN
            count_int <= (OTHERS => '0');
         ELSIF count_enable = '1' THEN
            count_int <= count_int + 1;
         END IF;
      END IF;
   END PROCESS;
   count <= count_int;
END Behavioral;