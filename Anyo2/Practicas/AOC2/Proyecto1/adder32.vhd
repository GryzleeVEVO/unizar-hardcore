----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    15:14:14 04/04/2014 
-- Design Name: 
-- Module Name:    adder32 - Behavioral 
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

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

ENTITY adder32 IS
    PORT (
        Din0 : IN STD_LOGIC_VECTOR (31 DOWNTO 0);
        Din1 : IN STD_LOGIC_VECTOR (31 DOWNTO 0);
        Dout : OUT STD_LOGIC_VECTOR (31 DOWNTO 0));
END adder32;

ARCHITECTURE Behavioral OF adder32 IS

BEGIN
    Dout <= Din0 + Din1;

END Behavioral;