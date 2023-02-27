LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
-- Unidad de anticipación incompleta. Ahora mismo selecciona siempre la entrada 0
-- Entradas: 
-- Reg_Rs_EX
-- Reg_Rt_EX
-- RegWrite_MEM
-- RW_MEM
-- RegWrite_WB
-- RW_WB
-- Salidas:
-- MUX_ctrl_A
-- MUX_ctrl_B
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
	-- Dise�o incompleto. Os lo ponemos c�mo ejemplo. Deb�is completarlo vosotros
	-- Activamos la se�al corto_A_Mem, cuand detectamos que el operando almacenado en A (Rs) es el mismo en el que va a escribir la instrucci�n que est� en la etapa Mem
	-- Importante: s�lo activamos el corto si la instrucci�n de la etapa MEM en v�lida
	Corto_A_Mem <= '1' WHEN ((Reg_Rs_EX = RW_MEM) AND (RegWrite_MEM = '1') AND (valid_I_MEM = '1')) ELSE
		'0';
	-- Resto de cortos:
	Corto_B_Mem <= '0';
	Corto_A_WB <= '0';
	Corto_B_WB <= '0';
	-- Con las se�ales anteriores se elige la entrada de los muxes:
	-- entrada 00: se corresponde al dato del banco de registros
	-- entrada 01: dato de la etapa Mem
	-- entrada 10: dato de la etapa WB
	-- Ponemos un ejemplo para el Corto_A_Mem. Deb�is completarlo
	MUX_ctrl_A <= "01" WHEN (Corto_A_Mem = '1') ELSE
		"00";
	MUX_ctrl_B <= "00";
END Behavioral;