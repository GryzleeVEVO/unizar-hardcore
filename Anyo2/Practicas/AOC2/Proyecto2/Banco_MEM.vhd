----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    15:28:20 04/07/2014 
-- Design Name: 
-- Module Name:    Banco_MEM - Behavioral 
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

ENTITY Banco_MEM IS
	PORT (
		ALU_out_EX : IN STD_LOGIC_VECTOR (31 DOWNTO 0);
		ALU_out_MEM : OUT STD_LOGIC_VECTOR (31 DOWNTO 0); -- instrucci�n leida en IF
		clk : IN STD_LOGIC;
		reset : IN STD_LOGIC;
		load : IN STD_LOGIC;
		MemWrite_EX : IN STD_LOGIC;
		MemRead_EX : IN STD_LOGIC;
		MemtoReg_EX : IN STD_LOGIC;
		RegWrite_EX : IN STD_LOGIC;
		MemWrite_MEM : OUT STD_LOGIC;
		MemRead_MEM : OUT STD_LOGIC;
		MemtoReg_MEM : OUT STD_LOGIC;
		RegWrite_MEM : OUT STD_LOGIC;
		BusB_EX : IN STD_LOGIC_VECTOR (31 DOWNTO 0); -- para los store
		BusB_MEM : OUT STD_LOGIC_VECTOR (31 DOWNTO 0); -- para los store
		RW_EX : IN STD_LOGIC_VECTOR (4 DOWNTO 0); -- registro destino de la escritura
		RW_MEM : OUT STD_LOGIC_VECTOR (4 DOWNTO 0);
		-- Nuevo excepci�n
		PC_exception_EX : IN STD_LOGIC_VECTOR (31 DOWNTO 0);
		PC_exception_MEM : OUT STD_LOGIC_VECTOR (31 DOWNTO 0);
		--bits de validez
		valid_I_EX : IN STD_LOGIC;
		valid_I_MEM : OUT STD_LOGIC);
END Banco_MEM;

ARCHITECTURE Behavioral OF Banco_MEM IS

BEGIN
	SYNC_PROC : PROCESS (clk)
	BEGIN
		IF (clk'event AND clk = '1') THEN
			IF (reset = '1') THEN
				ALU_out_MEM <= x"00000000";
				BUSB_MEM <= x"00000000";
				RW_MEM <= "00000";
				MemWrite_MEM <= '0';
				MemRead_MEM <= '0';
				MemtoReg_MEM <= '0';
				RegWrite_MEM <= '0';
				valid_I_MEM <= '0';
				--nuevo excepciones
				PC_exception_MEM <= x"00000000";
			ELSE
				IF (load = '1') THEN
					ALU_out_MEM <= ALU_out_EX;
					BUSB_MEM <= BUSB_EX;
					RW_MEM <= RW_EX;
					MemWrite_MEM <= MemWrite_EX;
					MemRead_MEM <= MemRead_EX;
					MemtoReg_MEM <= MemtoReg_EX;
					RegWrite_MEM <= RegWrite_EX;
					valid_I_MEM <= valid_I_EX;
					--nuevo excepciones
					PC_exception_MEM <= PC_exception_EX;
				END IF;
			END IF;
		END IF;
	END PROCESS;

END Behavioral;