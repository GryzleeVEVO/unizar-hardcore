----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    13:14:28 04/07/2014 
-- Design Name: 
-- Module Name:    UC - Behavioral 
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

ENTITY UC IS
	PORT (
		valid_I_ID : IN STD_LOGIC; --indica si es una instrucción válida
		IR_op_code : IN STD_LOGIC_VECTOR (5 DOWNTO 0);
		Branch : OUT STD_LOGIC;
		RegDst : OUT STD_LOGIC;
		ALUSrc : OUT STD_LOGIC;
		MemWrite : OUT STD_LOGIC;
		MemRead : OUT STD_LOGIC;
		MemtoReg : OUT STD_LOGIC;
		RegWrite : OUT STD_LOGIC;
		-- Nuevas señales
		RTE : OUT STD_LOGIC; -- indica que es una instrucción RTE
		Write_output : OUT STD_LOGIC; --indica que es una instrucción que escribe en la salida
		UNDEF : OUT STD_LOGIC --indica que el código de operación no pertenence a una instrucción conocida
		-- Fin Nuevas señales
	);
END UC;

ARCHITECTURE Behavioral OF UC IS
	CONSTANT NOP_opcode : STD_LOGIC_VECTOR (5 DOWNTO 0) := "000000";
	CONSTANT ARIT_opcode : STD_LOGIC_VECTOR (5 DOWNTO 0) := "000001";
	CONSTANT LW_opcode : STD_LOGIC_VECTOR (5 DOWNTO 0) := "000010";
	CONSTANT SW_opcode : STD_LOGIC_VECTOR (5 DOWNTO 0) := "000011";
	CONSTANT BEQ_opcode : STD_LOGIC_VECTOR (5 DOWNTO 0) := "000100";
	CONSTANT RTE_opcode : STD_LOGIC_VECTOR (5 DOWNTO 0) := "001000";
	CONSTANT WRO_opcode : STD_LOGIC_VECTOR (5 DOWNTO 0) := "100000";
BEGIN
	-- Si IR_op = 0 es nop, IR_op=1 es aritmética, IR_op=2 es LW, IR_op=3 es SW, IR_op= 4 es BEQ
	-- este CASE es en realidad un mux con las entradas fijas.
	UC_mux : PROCESS (IR_op_code)
	BEGIN
		-- Por defecto ponemos todas las seï¿½ales a 0 que es el valor que garantiza que no alteramos nada
		Branch <= '0';
		RegDst <= '0';
		ALUSrc <= '0';
		MemWrite <= '0';
		MemRead <= '0';
		MemtoReg <= '0';
		RegWrite <= '0';
		RTE <= '0';
		write_output <= '0';
		UNDEF <= '0';

		IF valid_I_ID = '1' THEN --si la instrucciï¿½n es vï¿½lida analizamos su cï¿½digo de operaciï¿½n
			CASE IR_op_code IS
					--NOP 
				WHEN NOP_opcode =>
					--ARIT
				WHEN ARIT_opcode =>
					RegDst <= '1';
					RegWrite <= '1';
					--LW
				WHEN LW_opcode =>
					ALUSrc <= '1';
					MemRead <= '1';
					MemtoReg <= '1';
					RegWrite <= '1';
					--SW
				WHEN SW_opcode =>
					ALUSrc <= '1';
					MemWrite <= '1';
					--BEQ
				WHEN BEQ_opcode =>
					Branch <= '1';
					------------------------------------------------
					-- Nuevas
					------------------------------------------------
					--RTE
				WHEN RTE_opcode =>
					RTE <= '1';
					--WRO (write_output)
				WHEN WRO_opcode =>
					write_output <= '1';
					-- OP code undefined
				WHEN OTHERS =>
					UNDEF <= '1';
			END CASE;
		END IF;
	END PROCESS;
END Behavioral;