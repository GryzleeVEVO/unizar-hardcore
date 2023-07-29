----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    22:25:11 04/07/2014 
-- Design Name: 
-- Module Name:    Banco_WB - Behavioral 
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

ENTITY Banco_WB IS
   PORT (
      ALU_out_MEM : IN STD_LOGIC_VECTOR (31 DOWNTO 0);
      ALU_out_WB : OUT STD_LOGIC_VECTOR (31 DOWNTO 0);
      MEM_out : IN STD_LOGIC_VECTOR (31 DOWNTO 0);
      MDR : OUT STD_LOGIC_VECTOR (31 DOWNTO 0); --memory data register
      clk : IN STD_LOGIC;
      reset : IN STD_LOGIC;
      load : IN STD_LOGIC;
      MemtoReg_MEM : IN STD_LOGIC;
      RegWrite_MEM : IN STD_LOGIC;
      MemtoReg_WB : OUT STD_LOGIC;
      RegWrite_WB : OUT STD_LOGIC;
      RW_MEM : IN STD_LOGIC_VECTOR (4 DOWNTO 0); -- registro destino de la escritura
      RW_WB : OUT STD_LOGIC_VECTOR (4 DOWNTO 0); -- PC+4 en la etapa IDend Banco_WB;
      --bits de validez
      valid_I_WB_in : IN STD_LOGIC;
      valid_I_WB : OUT STD_LOGIC);
END Banco_WB;
ARCHITECTURE Behavioral OF Banco_WB IS

BEGIN
   SYNC_PROC : PROCESS (clk)
   BEGIN
      IF (clk'event AND clk = '1') THEN
         IF (reset = '1') THEN
            ALU_out_WB <= "00000000000000000000000000000000";
            MDR <= "00000000000000000000000000000000";
            RW_WB <= "00000";
            MemtoReg_WB <= '0';
            RegWrite_WB <= '0';
            valid_I_WB <= '0';
         ELSE
            IF (load = '1') THEN
               ALU_out_WB <= ALU_out_MEM;
               MDR <= Mem_out;
               RW_WB <= RW_MEM;
               MemtoReg_WB <= MemtoReg_MEM;
               RegWrite_WB <= RegWrite_MEM;
               valid_I_WB <= valid_I_WB_in;
            END IF;
         END IF;
      END IF;
   END PROCESS;

END Behavioral;