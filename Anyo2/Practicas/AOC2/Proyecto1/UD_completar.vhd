LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;

ENTITY UD IS
	PORT (
		valid_I_ID : IN STD_LOGIC; --indica si es una instrucción de ID es válida
		valid_I_EX : IN STD_LOGIC; --indica si es una instrucción de EX es válida
		valid_I_MEM : IN STD_LOGIC; --indica si es una instrucción de MEM es válida
		Reg_Rs_ID : IN STD_LOGIC_VECTOR (4 DOWNTO 0); --registros Rs y Rt en la etapa ID
		Reg_Rt_ID : IN STD_LOGIC_VECTOR (4 DOWNTO 0);
		MemRead_EX : IN STD_LOGIC; -- información sobre la instrucción en EX (destino, si lee de memoria y si escribe en registro)
		RegWrite_EX : IN STD_LOGIC;
		RW_EX : IN STD_LOGIC_VECTOR (4 DOWNTO 0);
		RegWrite_Mem : IN STD_LOGIC;-- informacion sobre la instruccion en Mem (destino y si escribe en registro)
		RW_Mem : IN STD_LOGIC_VECTOR (4 DOWNTO 0);
		IR_op_code : IN STD_LOGIC_VECTOR (5 DOWNTO 0); -- código de operación de la instrucción en IEEE
		salto_tomado : IN STD_LOGIC; -- 1 cuando se produce un salto 0 en caso contrario
		--Nuevo
		Mem_ready : IN STD_LOGIC; -- 1 cuando la memoria puede realizar la operaciï¿½n solicitada en el ciclo actual
		parar_EX : OUT STD_LOGIC; -- Indica que las etapas EX y previas deben parar
		Kill_IF : OUT STD_LOGIC; -- Indica que la instrucción en IF no debe ejecutarse (fallo en la predicciï¿½n de salto tomado)
		Parar_ID : OUT STD_LOGIC -- Indica que las etapas ID y previas deben parar
	);
END UD;
ARCHITECTURE Behavioral OF UD IS
	SIGNAL dep_rs_EX, dep_rs_Mem, dep_rt_EX, dep_rt_Mem, ld_uso_rs, ld_uso_rt, WRO_rs, BEQ_rs, BEQ_rt, riesgo_datos_ID, parar_EX_internal : STD_LOGIC;
	CONSTANT NOP : STD_LOGIC_VECTOR (5 DOWNTO 0) := "000000";
	CONSTANT LW : STD_LOGIC_VECTOR (5 DOWNTO 0) := "000010";
	CONSTANT BEQ : STD_LOGIC_VECTOR (5 DOWNTO 0) := "000100";
	CONSTANT RTE_opcode : STD_LOGIC_VECTOR (5 DOWNTO 0) := "001000";
	CONSTANT WRO_opcode : STD_LOGIC_VECTOR (5 DOWNTO 0) := "100000";
BEGIN
	-- Limpia la instrucción en fetch. Sólo si la instrucción es válida,
	-- tiene los operandos disponibles y decide saltar
	Kill_IF <= NOT (ld_uso_rs OR ld_uso_rt) AND valid_I_ID AND salto_tomado;

	--  ***** Detección de dependencias de datos *****

	-- Hay dependencia si
	--		- La instrucción en EX/MEM es válida
	--		- Requiere del mismo registro que en RW
	--		- Va a escribir en RW
	--		- NOP nunca genera dependencia, RTE es incondicional
	
	dep_rs_EX <=
		'1' WHEN ((valid_I_EX = '1') AND (Reg_Rs_ID = RW_EX) AND (RegWrite_EX = '1') AND (IR_op_code /= NOP) AND (IR_op_code /= RTE_opcode)) ELSE
		'0';

	dep_rs_Mem <=
		'1' WHEN((valid_I_MEM = '1') AND (Reg_Rs_ID = RW_MEM) AND (RegWrite_EX = '1') AND (IR_op_code /= NOP) AND (IR_op_code /= RTE_opcode)) ELSE
		'0';

	dep_rt_EX <=
		'1' WHEN ((valid_I_EX = '1') AND (Reg_Rt_ID = RW_EX) AND (RegWrite_EX = '1') AND (IR_op_code /= NOP) AND (IR_op_code /= RTE_opcode)) ELSE
		'0';

	dep_rt_Mem <=
		'1' WHEN((valid_I_MEM = '1') AND (Reg_Rt_ID = RW_MEM) AND (RegWrite_EX = '1') AND (IR_op_code /= NOP) AND (IR_op_code /= RTE_opcode)) ELSE
		'0';

	-- ***** Riesgos de datos *****

	-- Si hay una instrucción que en etapa EX que requerirá un operando de
	-- un LW, si el LW está en MEM aún no habrá sacado dicho dato y hay que parar 
	ld_uso_rs <= dep_rs_EX AND MemRead_EX;
	ld_uso_rt <= dep_rt_EX AND MemRead_EX;

	-- Al no poder anticipar en ID, cualquier dependencia de BEQ es una parada 
	BEQ_rs <= (dep_rs_EX OR dep_rs_Mem) AND IR_op_code = BEQ;
	BEQ_rt <= (dep_rt_EX OR dep_rt_Mem) AND IR_op_code = BEQ;

	-- 3) WRO: Dependencia similar al BEQ, pero hay que tener en cuenta que WRO sí lo usa Rs
	WRO_rs <= (dep_rs_EX OR dep_rs_Mem) AND IR_op_code = WRO;

	-- Hay riesgo de datos o de control
	riesgo_datos_ID <= BEQ_rt OR BEQ_rs OR ld_uso_rs OR ld_uso_rt OR WRO_rs;

	-- Parada de ID
	Parar_ID <= parar_EX_internal OR (valid_I_ID AND riesgo_datos_ID);

	-- Nunca se detiene la etapa de ejecución en este proyecto
	parar_EX_internal <= '0';
	parar_EX <= parar_EX_internal;
	-------------------------------------------------------------------------------------------------------------------------------
END Behavioral;