----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    15:10:56 04/04/2014 
-- Design Name: 
-- Module Name:    bits_shifter - Behavioral 
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

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

ENTITY two_bits_shifter IS
    PORT (
        Din : IN STD_LOGIC_VECTOR (31 DOWNTO 0);
        Dout : OUT STD_LOGIC_VECTOR (31 DOWNTO 0));
END two_bits_shifter;

ARCHITECTURE Behavioral OF two_bits_shifter IS

BEGIN
    Dout(1 DOWNTO 0) <= "00";
    Dout(31 DOWNTO 2) <= Din(29 DOWNTO 0);

END Behavioral;