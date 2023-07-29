----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    15:07:44 04/04/2014 
-- Design Name: 
-- Module Name:    Ext_signo - Behavioral 
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

ENTITY Ext_signo IS
    PORT (
        inm : IN STD_LOGIC_VECTOR (15 DOWNTO 0);
        inm_ext : OUT STD_LOGIC_VECTOR (31 DOWNTO 0));
END Ext_signo;

ARCHITECTURE Behavioral OF Ext_signo IS

BEGIN

    inm_ext(15 DOWNTO 0) <= inm;
    inm_ext(31 DOWNTO 16) <= "0000000000000000" WHEN inm(15) = '0' ELSE
    "1111111111111111";

END Behavioral;