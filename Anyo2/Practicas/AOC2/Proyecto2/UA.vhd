-- AOC2
-- Proyecto 1
-- Dorian Boleslaw Wozniak (817570@unizar.es)
-- Adrian Arribas Mateo (795593@unizar.es)
-- Unidad de anticipación


LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;

ENTITY UA IS
	PORT (
		valid_I_MEM : IN STD_LOGIC; --indica si es una instrucci�n de MEM es v�lida
		valid_I_WB : IN STD_LOGIC; --indica si es una instrucci�n de WB es v�lida
		Reg_Rs_EX : IN STD_LOGIC_VECTOR(4 DOWNTO 0);
		Reg_Rt_EX : IN STD_LOGIC_VECTOR(4 DOWNTO 0);
		RegWrite_MEM : IN STD_LOGIC;
		RW_MEM : IN STD_LOGIC_VECTOR(4 DOWNTO 0);
		RegWrite_WB : IN STD_LOGIC;
		RW_WB : IN STD_LOGIC_VECTOR(4 DOWNTO 0);
		MUX_ctrl_A : OUT STD_LOGIC_VECTOR(1 DOWNTO 0);
		MUX_ctrl_B : OUT STD_LOGIC_VECTOR(1 DOWNTO 0)
	);
END UA;

ARCHITECTURE Behavioral OF UA IS
	SIGNAL Corto_A_Mem, Corto_B_Mem, Corto_A_WB, Corto_B_WB : STD_LOGIC;
BEGIN

	-- Un corto se puede activar cuando:
	-- 		- La instrucci�n de donde se anticipa es v�lida
	--		- El registro de escritura seleccionado es utilizado como operando por EX
	--		- Se realiza una escritura (puede ser que RW no quede utilizada)
	Corto_A_Mem <=
		'1' WHEN ((Reg_Rs_EX = RW_MEM) AND (RegWrite_MEM = '1') AND (valid_I_MEM = '1')) ELSE
		'0';

	Corto_B_Mem <=
		'1' WHEN ((Reg_Rt_EX = RW_MEM) AND (RegWrite_MEM = '1') AND (valid_I_MEM = '1')) ELSE
		'0';

	Corto_A_WB <=
		'1' WHEN ((Reg_Rs_EX = RW_WB) AND (RegWrite_WB = '1') AND (valid_I_WB = '1')) ELSE
		'0';

	Corto_B_WB <=
		'1' WHEN ((Reg_Rt_EX = RW_WB) AND (RegWrite_WB = '1') AND (valid_I_WB = '1')) ELSE
		'0';

	-- Los cortos en etapa de memoria tienen prioridad sobre WB por ser mas recientes
	MUX_ctrl_A <=
		"10" WHEN (Corto_A_Mem = '0' AND Corto_A_WB = '1') ELSE
		"01" WHEN (Corto_A_Mem = '1') ELSE
		"00";

	MUX_ctrl_B <=
		"10" WHEN (Corto_B_Mem = '0' AND Corto_B_WB = '1') ELSE
		"01" WHEN (Corto_B_Mem = '1') ELSE
		"00";
END Behavioral;