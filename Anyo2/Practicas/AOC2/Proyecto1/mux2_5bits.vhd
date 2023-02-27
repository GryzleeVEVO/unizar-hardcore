----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    15:13:42 04/07/2014 
-- Design Name: 
-- Module Name:    mux2_6bits - Behavioral 
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

ENTITY mux2_5bits IS
    PORT (
        DIn0 : IN STD_LOGIC_VECTOR (4 DOWNTO 0);
        DIn1 : IN STD_LOGIC_VECTOR (4 DOWNTO 0);
        ctrl : IN STD_LOGIC;
        Dout : OUT STD_LOGIC_VECTOR (4 DOWNTO 0));
END mux2_5bits;

ARCHITECTURE Behavioral OF mux2_5bits IS

BEGIN

    Dout <= DIn1 WHEN (ctrl = '1') ELSE
        DIn0;
END Behavioral;