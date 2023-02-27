----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    12:10:07 04/03/2014 
-- Design Name: 
-- Module Name:    ALU - Behavioral 
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

ENTITY ALU IS
    PORT (
        DA : IN STD_LOGIC_VECTOR (31 DOWNTO 0); --entrada 1
        DB : IN STD_LOGIC_VECTOR (31 DOWNTO 0); --entrada 2
        ALUctrl : IN STD_LOGIC_VECTOR (2 DOWNTO 0); -- funci�n a realizar: 0 suma, 1 resta, 2 AND, 3 OR. El resto se dejan por si queremos a�adir operaciones
        Dout : OUT STD_LOGIC_VECTOR (31 DOWNTO 0)); -- salida
END ALU;

ARCHITECTURE Behavioral OF ALU IS
    SIGNAL Dout_internal : STD_LOGIC_VECTOR (31 DOWNTO 0);
BEGIN
    Dout_internal <= DA + DB WHEN (ALUctrl = "000")
        ELSE
        DA - DB WHEN (ALUctrl = "001")
        ELSE
        DA AND DB WHEN (ALUctrl = "010")
        ELSE
        DA OR DB WHEN (ALUctrl = "011")
        ELSE
        "00000000000000000000000000000000";
    Dout <= Dout_internal;

END Behavioral;