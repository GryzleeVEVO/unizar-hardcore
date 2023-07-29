----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    15:16:09 05/14/2014 
-- Design Name: 
-- Module Name:    Contador de dos bits 
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
ENTITY counter_2bits IS
   PORT (
      clk : IN STD_LOGIC;
      reset : IN STD_LOGIC;
      count_enable : IN STD_LOGIC;
      count : OUT STD_LOGIC_VECTOR (1 DOWNTO 0));
END counter_2bits;

ARCHITECTURE Behavioral OF counter_2bits IS
   SIGNAL int_count : STD_LOGIC_VECTOR (1 DOWNTO 0);
BEGIN
   PROCESS (clk)
   BEGIN
      IF clk = '1' AND clk'event THEN
         IF reset = '1' THEN
            int_count <= (OTHERS => '0'); -- pone todo a 0
         ELSIF count_enable = '1' THEN
            int_count <= int_count + "01"; --si enable vale uno y load vale 0 el contador cuenta.
         END IF;

      END IF;
   END PROCESS;
   count <= int_count;
END Behavioral;