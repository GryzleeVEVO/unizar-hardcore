----------------------------------------------------------------------------------
-- Description: Mips segmentado tal y como lo hemos estudiado en clase. Sus caracter�sticas son:
-- Saltos 1-retardados
-- instrucciones aritm�ticas, LW, SW y BEQ
-- MI y MD de 128 palabras de 32 bits
-- Excepciones: IRQ, ABORT y UNDEF
-- L�nea de IRQ
-- Nuevas instrucciones: RTE y WRO
-- Hay funcionalidad incompleta en este archivo, y en UD y UA. Buscar la etiqueta: completar
----------------------------------------------------------------------------------
LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;

ENTITY MIPs_segmentado IS
	PORT (
		clk : IN STD_LOGIC;
		reset : IN STD_LOGIC;
		IRQ : IN STD_LOGIC;
		IO_input : IN STD_LOGIC_VECTOR (31 DOWNTO 0); -- 32 bits de entrada para el MIPS para IO. En el primer proyecto no se usa. En el segundo s�
		IO_output : OUT STD_LOGIC_VECTOR (31 DOWNTO 0)); -- 32 bits de salida para el MIPS para IO
END MIPs_segmentado;

ARCHITECTURE Behavioral OF MIPs_segmentado IS

	COMPONENT reg IS
		GENERIC (size : NATURAL := 32); -- por defecto son de 32 bits, pero se puede usar cualquier tama�o
		PORT (
			Din : IN STD_LOGIC_VECTOR (size - 1 DOWNTO 0);
			clk : IN STD_LOGIC;
			reset : IN STD_LOGIC;
			load : IN STD_LOGIC;
			Dout : OUT STD_LOGIC_VECTOR (size - 1 DOWNTO 0));
	END COMPONENT;
	---------------------------------------------------------------

	COMPONENT adder32 IS
		PORT (
			Din0 : IN STD_LOGIC_VECTOR (31 DOWNTO 0);
			Din1 : IN STD_LOGIC_VECTOR (31 DOWNTO 0);
			Dout : OUT STD_LOGIC_VECTOR (31 DOWNTO 0));
	END COMPONENT;

	COMPONENT mux2_1 IS
		PORT (
			DIn0 : IN STD_LOGIC_VECTOR (31 DOWNTO 0);
			DIn1 : IN STD_LOGIC_VECTOR (31 DOWNTO 0);
			ctrl : IN STD_LOGIC;
			Dout : OUT STD_LOGIC_VECTOR (31 DOWNTO 0));
	END COMPONENT;

	COMPONENT Data_Memory_Subsystem IS PORT (
		CLK : IN STD_LOGIC;
		reset : IN STD_LOGIC;
		ADDR : IN STD_LOGIC_VECTOR (31 DOWNTO 0); --Dir solicitada por el Mips
		Din : IN STD_LOGIC_VECTOR (31 DOWNTO 0);--entrada de datos desde el Mips
		WE : IN STD_LOGIC; -- write enable	del MIPS
		RE : IN STD_LOGIC; -- read enable del MIPS	
		IO_input : IN STD_LOGIC_VECTOR (31 DOWNTO 0); --dato que viene de una entrada del sistema
		Mem_ready : OUT STD_LOGIC; -- indica si podemos hacer la operaci�n solicitada en el ciclo actual
		Data_abort : OUT STD_LOGIC; --indica que el �ltimo acceso a memoria ha sido un error
		Dout : OUT STD_LOGIC_VECTOR (31 DOWNTO 0) --dato que se env�a al Mips
		);
	END COMPONENT;
	COMPONENT memoriaRAM_I IS PORT (
		CLK : IN STD_LOGIC;
		ADDR : IN STD_LOGIC_VECTOR (31 DOWNTO 0); --Dir 
		Din : IN STD_LOGIC_VECTOR (31 DOWNTO 0);--entrada de datos para el puerto de escritura
		WE : IN STD_LOGIC; -- write enable	
		RE : IN STD_LOGIC; -- read enable		  
		Dout : OUT STD_LOGIC_VECTOR (31 DOWNTO 0));
	END COMPONENT;

	COMPONENT Banco_ID IS
		PORT (
			IR_in : IN STD_LOGIC_VECTOR (31 DOWNTO 0); -- instrucci�n leida en IF
			PC4_in : IN STD_LOGIC_VECTOR (31 DOWNTO 0); -- PC+4 sumado en IF
			clk : IN STD_LOGIC;
			reset : IN STD_LOGIC;
			load : IN STD_LOGIC;
			IR_ID : OUT STD_LOGIC_VECTOR (31 DOWNTO 0); -- instrucci�n en la etapa ID
			PC4_ID : OUT STD_LOGIC_VECTOR (31 DOWNTO 0);
			--nuevo para excepciones
			PC_exception : IN STD_LOGIC_VECTOR (31 DOWNTO 0); -- PC al que se volver� si justo esta instrucci�n est� en MEM cuando llega una excepci�n. 
			PC_exception_ID : OUT STD_LOGIC_VECTOR (31 DOWNTO 0);-- PC+4 en la etapa ID
			--bits de validez
			valid_I_IF : IN STD_LOGIC;
			valid_I_ID : OUT STD_LOGIC);
	END COMPONENT;

	COMPONENT BReg
		PORT (
			clk : IN STD_LOGIC;
			reset : IN STD_LOGIC;
			RA : IN STD_LOGIC_VECTOR(4 DOWNTO 0);
			RB : IN STD_LOGIC_VECTOR(4 DOWNTO 0);
			RW : IN STD_LOGIC_VECTOR(4 DOWNTO 0);
			BusW : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
			RegWrite : IN STD_LOGIC;
			BusA : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
			BusB : OUT STD_LOGIC_VECTOR(31 DOWNTO 0)
		);
	END COMPONENT;

	COMPONENT Ext_signo IS
		PORT (
			inm : IN STD_LOGIC_VECTOR (15 DOWNTO 0);
			inm_ext : OUT STD_LOGIC_VECTOR (31 DOWNTO 0));
	END COMPONENT;

	COMPONENT two_bits_shifter IS
		PORT (
			Din : IN STD_LOGIC_VECTOR (31 DOWNTO 0);
			Dout : OUT STD_LOGIC_VECTOR (31 DOWNTO 0));
	END COMPONENT;

	COMPONENT UC IS
		PORT (
			valid_I_ID : IN STD_LOGIC; --indica si es una instrucci�n v�lida			
			IR_op_code : IN STD_LOGIC_VECTOR (5 DOWNTO 0);
			Branch : OUT STD_LOGIC;
			RegDst : OUT STD_LOGIC;
			ALUSrc : OUT STD_LOGIC;
			MemWrite : OUT STD_LOGIC;
			MemRead : OUT STD_LOGIC;
			MemtoReg : OUT STD_LOGIC;
			RegWrite : OUT STD_LOGIC;
			-- Nuevas se�ales
			RTE : OUT STD_LOGIC; -- indica que es una instrucci�n RTE
			Write_output : OUT STD_LOGIC; --indica que es una instrucci�n que escribe en la salida
			UNDEF : OUT STD_LOGIC --indica que el c�digo de operaci�n no pertenence a una instrucci�n conocida
			-- Fin Nuevas se�ales
		);
	END COMPONENT;
	-- Unidad de detecci�n de riesgos
	COMPONENT UD IS
		PORT (
			valid_I_ID : IN STD_LOGIC; --indica si es una instrucci�n v�lida
			valid_I_EX : IN STD_LOGIC; --indica si es una instrucci�n de EX es v�lida
			valid_I_MEM : IN STD_LOGIC; --indica si es una instrucci�n de MEM es v�lida
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
			Mem_ready : IN STD_LOGIC; -- 1 cuando la memoria puede realizar la operaci�n solicitada en el ciclo actual
			parar_EX : OUT STD_LOGIC; -- Indica que las etapas MEM y previas deben parar
			Kill_IF : OUT STD_LOGIC; -- Indica que la instrucción en IF no debe ejecutarse (fallo en la predicción de salto tomado)
			Parar_ID : OUT STD_LOGIC); -- Indica que las etapas ID y previas deben parar
	END COMPONENT;

	COMPONENT Banco_EX
		PORT (
			clk : IN STD_LOGIC;
			reset : IN STD_LOGIC;
			load : IN STD_LOGIC;
			busA : IN STD_LOGIC_VECTOR (31 DOWNTO 0);
			busB : IN STD_LOGIC_VECTOR (31 DOWNTO 0);
			busA_EX : OUT STD_LOGIC_VECTOR (31 DOWNTO 0);
			busB_EX : OUT STD_LOGIC_VECTOR (31 DOWNTO 0);
			RegDst_ID : IN STD_LOGIC;
			ALUSrc_ID : IN STD_LOGIC;
			MemWrite_ID : IN STD_LOGIC;
			MemRead_ID : IN STD_LOGIC;
			MemtoReg_ID : IN STD_LOGIC;
			RegWrite_ID : IN STD_LOGIC;
			inm_ext : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
			inm_ext_EX : OUT STD_LOGIC_VECTOR(31 DOWNTO 0);
			RegDst_EX : OUT STD_LOGIC;
			ALUSrc_EX : OUT STD_LOGIC;
			MemWrite_EX : OUT STD_LOGIC;
			MemRead_EX : OUT STD_LOGIC;
			MemtoReg_EX : OUT STD_LOGIC;
			RegWrite_EX : OUT STD_LOGIC;
			-- Nuevo
			Reg_Rs_ID : IN STD_LOGIC_VECTOR(4 DOWNTO 0);
			Reg_Rs_EX : OUT STD_LOGIC_VECTOR(4 DOWNTO 0);
			--Fin nuevo
			ALUctrl_ID : IN STD_LOGIC_VECTOR (2 DOWNTO 0);
			ALUctrl_EX : OUT STD_LOGIC_VECTOR (2 DOWNTO 0);
			Reg_Rt_ID : IN STD_LOGIC_VECTOR (4 DOWNTO 0);
			Reg_Rd_ID : IN STD_LOGIC_VECTOR (4 DOWNTO 0);
			Reg_Rt_EX : OUT STD_LOGIC_VECTOR (4 DOWNTO 0);
			Reg_Rd_EX : OUT STD_LOGIC_VECTOR (4 DOWNTO 0);
			-- Nuevo excepci�n
			PC_exception_ID : IN STD_LOGIC_VECTOR (31 DOWNTO 0);
			PC_exception_EX : OUT STD_LOGIC_VECTOR (31 DOWNTO 0);
			--bits de validez
			valid_I_EX_in : IN STD_LOGIC;
			valid_I_EX : OUT STD_LOGIC);
	END COMPONENT;

	-- Unidad de anticipaci�n de operandos
	COMPONENT UA
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
	END COMPONENT;
	-- Mux 4 a 1
	COMPONENT mux4_1_32bits IS
		PORT (
			DIn0 : IN STD_LOGIC_VECTOR (31 DOWNTO 0);
			DIn1 : IN STD_LOGIC_VECTOR (31 DOWNTO 0);
			DIn2 : IN STD_LOGIC_VECTOR (31 DOWNTO 0);
			DIn3 : IN STD_LOGIC_VECTOR (31 DOWNTO 0);
			ctrl : IN STD_LOGIC_VECTOR(1 DOWNTO 0);
			Dout : OUT STD_LOGIC_VECTOR (31 DOWNTO 0));
	END COMPONENT;

	COMPONENT ALU
		PORT (
			DA : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
			DB : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
			ALUctrl : IN STD_LOGIC_VECTOR(2 DOWNTO 0);
			Dout : OUT STD_LOGIC_VECTOR(31 DOWNTO 0)
		);
	END COMPONENT;

	COMPONENT mux2_5bits IS
		PORT (
			DIn0 : IN STD_LOGIC_VECTOR (4 DOWNTO 0);
			DIn1 : IN STD_LOGIC_VECTOR (4 DOWNTO 0);
			ctrl : IN STD_LOGIC;
			Dout : OUT STD_LOGIC_VECTOR (4 DOWNTO 0));
	END COMPONENT;

	COMPONENT Banco_MEM
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
	END COMPONENT;

	COMPONENT Banco_WB
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
	END COMPONENT;

	COMPONENT counter
		GENERIC (
			size : INTEGER := 10);
		PORT (
			clk : IN STD_LOGIC;
			reset : IN STD_LOGIC;
			count_enable : IN STD_LOGIC;
			count : OUT STD_LOGIC_VECTOR (size - 1 DOWNTO 0));
	END COMPONENT;
	-- Se�ales internas MIPS	
	CONSTANT ARIT : STD_LOGIC_VECTOR (5 DOWNTO 0) := "000001";
	SIGNAL load_PC, RegWrite_ID, RegWrite_EX, RegWrite_MEM, RegWrite_WB, RegWrite, Z, Branch_ID, RegDst_ID, RegDst_EX, ALUSrc_ID, ALUSrc_EX : STD_LOGIC;
	SIGNAL MemtoReg_ID, MemtoReg_EX, MemtoReg_MEM, MemtoReg_WB, MemWrite_ID, MemWrite_EX, MemWrite_MEM, MemRead_ID, MemRead_EX, MemRead_MEM, WE, RE : STD_LOGIC;
	SIGNAL PC_in, PC_out, four, PC4, Dirsalto_ID, IR_in, IR_ID, PC4_ID, inm_ext_EX, ALU_Src_out : STD_LOGIC_VECTOR(31 DOWNTO 0);
	SIGNAL BusW, BusA, BusB, BusA_EX, BusB_EX, BusB_MEM, inm_ext, inm_ext_x4, ALU_out_EX, ALU_out_MEM, ALU_out_WB, Mem_out, MDR : STD_LOGIC_VECTOR(31 DOWNTO 0);
	SIGNAL RW_EX, RW_MEM, RW_WB, Reg_Rs_ID, Reg_Rs_EX, Reg_Rt_ID, Reg_Rd_EX, Reg_Rt_EX : STD_LOGIC_VECTOR(4 DOWNTO 0);
	SIGNAL ALUctrl_ID, ALUctrl_EX : STD_LOGIC_VECTOR(2 DOWNTO 0);
	SIGNAL ALU_INT_out, Mux_A_out, Mux_B_out : STD_LOGIC_VECTOR(31 DOWNTO 0);
	SIGNAL IR_op_code : STD_LOGIC_VECTOR(5 DOWNTO 0);
	SIGNAL MUX_ctrl_A, MUX_ctrl_B : STD_LOGIC_VECTOR(1 DOWNTO 0);
	SIGNAL salto_tomado : STD_LOGIC;
	--Se�ales soluci�n
	SIGNAL parar_ID, parar_EX, RegWrite_EX_mux_out, Kill_IF, reset_ID, load_ID, load_EX, load_Mem : STD_LOGIC;
	SIGNAL Write_output, write_output_UC : STD_LOGIC;
	-- Soporte Excepciones--
	SIGNAL MIPS_status, status_input : STD_LOGIC_VECTOR(1 DOWNTO 0);
	SIGNAL PC_exception, PC_exception_MEM, PC_exception_EX, PC_exception_ID, Return_I, Exception_LR_output : STD_LOGIC_VECTOR(31 DOWNTO 0);
	SIGNAL Exception_accepted, RTE_ID, update_status, reset_EX, reset_MEM : STD_LOGIC;
	SIGNAL Data_Abort, Undef : STD_LOGIC;
	-- Bit validez etapas
	SIGNAL valid_I_IF, valid_I_ID, valid_I_EX_in, valid_I_EX, valid_I_MEM, valid_I_WB_in, valid_I_WB : STD_LOGIC;
	-- contadores
	SIGNAL cycles : STD_LOGIC_VECTOR(15 DOWNTO 0);
	SIGNAL Ins, data_stalls, control_stalls, Exceptions, Exception_cycles : STD_LOGIC_VECTOR(7 DOWNTO 0);
	SIGNAL inc_cycles, inc_I, inc_data_stalls, inc_control_stalls, inc_Exceptions, inc_Exception_cycles : STD_LOGIC;
	--interfaz con memoria
	SIGNAL Mem_ready : STD_LOGIC;
BEGIN
	-- ****************************************************************************************************
	-- Gesti�n de Excepciones: 
	--		* IRQ: es una entrada del MIPs 
	--		* Data_abort: la genera el controlador de memoria cuando recibe una direcci�n no alienada, o fuera del rango de la memoria
	--		* UNDEF: la genera la unidad de control cuando le llega una instrucci�n v�lida con un c�digo de operaci�n desconocido
	-- ****************************************************************************************************
	-------------------------------------------------------------------------------------------------------------------------------
	-- Status_register	 
	-- el registro tiene como entradas y salidas vectores de se�ales cuya longitud se indica en size. En este caso es un vector de tama�o 2
	-- El bit m�s significativo permite deshabilitar (valor 1) o habilitar las excepciones (valor 0)
	-- El bit menos significativo informa si estamos en modo Excepci�n o estamos en modo normal

	status_reg : reg GENERIC MAP(size => 2)
	PORT MAP(Din => status_input, clk => clk, reset => reset, load => update_status, Dout => MIPS_status);
	------------------------------------------------------------------------------------
	-- Completar: falta la l�gica que detecta cu�ndo se va a procesaruna excepci�n: cuando se recibe una de las se�ales (IRQ, Data_abort y Undef) y las excepciones est�n habilitadas (MIPS_status(1)='1')
	Exception_accepted <= '0';
	------------------------------------------------------------------------------------
	-- Completar: falta la l�gica que gestiona update_status. Dise�adla.
	update_status <= Exception_accepted OR RTE_ID;
	-- Fin completar;
	------------------------------------------------------------------------------------

	-- multiplexor para elegir la entrada del registro de estado
	-- En este procesador s�lo hay dos opciones ya que al entrar en modo excepci�n se deshabilitan las excepciones:
	-- 		* "11" al entrar en una IRQ (Excepciones deshabilitadas y modo Excepci�n)
	--		* "00" en el resto de casos
	-- Podr�a hacerse con un bit, pero usamos dos para permitir ampliaciones)
	status_input <= "11" WHEN (Exception_accepted = '1') ELSE
		"00";

	------------------------------------------------------------------------------------
	-- Al procesar una excepci�n las instrucciones que est�n en Mem y WB contin�an su ejecuci�n. El resto se matan
	-- Para retornar se debe eligir la siguiente instrucci�n v�lida. Para ello tenemos sus direcciones almacenadas en:
	-- PC_exception_EX y PC_exception_ID, y sus bits de validez en valid_I_EX y valid_I_ID
	-- Si no hay v�lidas se almacena el valor del PC.
	Return_I <= PC_exception_EX WHEN (valid_I_EX = '1') ELSE
		PC_exception_ID WHEN (valid_I_ID = '1') ELSE
		PC_out;
	------------------------------------------------------------------------------------	
	-- Exception_LR: almacena la direcci�n a la que hay que retornar tras una excepci�n	 
	-- Vamos a guardar la direcci�n seleccionada en el MUX de arriba
	Exception_LR : reg GENERIC MAP(size => 32)
	PORT MAP(Din => Return_I, clk => clk, reset => reset, load => Exception_accepted, Dout => Exception_LR_output);
	-- ****************************************************************************************************
	pc : reg GENERIC MAP(size => 32)
	PORT MAP(Din => PC_in, clk => clk, reset => reset, load => load_PC, Dout => PC_out);

	------------------------------------------------------------------------------------
	-- Completar:
	-- load_pc vale '1' porque en la versi�n actual el procesador no para nunca
	-- Si queremos detener una instrucci�n en la etapa fetch habr� que ponerlo a '0'
	-- Si paramos en ID, tambi�n hay que parar en IF. Parar_ID nos avisa de esto.
	-- Importante: Puede ser que Parar_ID se active, y a la vez se procese una excepci�n: �hay que actualizar el pc?
	load_PC <= '1';
	-- Fin completar;
	------------------------------------------------------------------------------------
	------------------------------------------------------------------------------------
	-- la x en x"00000004" indica que est� en hexadecimal
	adder_4 : adder32 PORT MAP(Din0 => PC_out, Din1 => x"00000004", Dout => PC4);
	------------------------------------------------------------------------------------
	-- Completar: sustituir los (--completar) por la condici�n correcta y descomentar el c�digo. Ver ejemplo para el data abort.
	-- Este c�digo es el mux de entrada al PC: elige entre PC+4, la direcci�n de salto generada en ID, la direcci�n de la rutina de tratamiento de la excepci�n, o la direcci�n de retorno de la excepci�n
	-- El orden asigna prioridad si se cumplen dos o m�s condiciones			

	PC_in <= x"00000008" WHEN (Exception_accepted = '1') AND (Data_abort = '1') ELSE -- Si llega un data abort y est� habilitado saltamos a la direcci�n 0x00000008
		--x"0000000C" 		when (--completar) else -- Si llega un UNDEF saltamos a la direcci�n 0x0000000C
		--x"00000004" 		when (--completar) else -- Si llega un data abort saltamos a la direcci�n 0x00000008
		--Exception_LR_output when (--completar) else 	--@ retorno. Si es una RTE volvemos a la @ que ten�amos almacenada en el Exception_LR		
		Dirsalto_ID WHEN (salto_tomado = '1') ELSE --@ Salto 
		PC4; -- PC+4
	------------------------------------------------------------------------------------
	-- Memoria de instrucciones. Tiene el puerto de escritura inhabilitado (WE='0')
	Mem_I : memoriaRAM_I PORT MAP(CLK => CLK, ADDR => PC_out, Din => x"00000000", WE => '0', RE => '1', Dout => IR_in);
	------------------------------------------------------------------------------------
	-- Reset del banco ID: reseteamos el banco si hay una excepci�n aceptada, ya que en ese caso se matan las intrucciones en IF, ID y EX 
	reset_ID <= (reset OR Exception_accepted);

	--------------------------------------------------------------
	-- anulaci�n de la instrucci�n. Si en ID se detecta que la instrucci�n de IF no debe ejecutarse se desactiva la se�al valid_I_IF
	valid_I_IF <= NOT(kill_IF);
	-----------------------------------------------------------------
	-- Para detener la etapa ID 
	load_ID <= NOT(parar_ID);

	Banco_IF_ID : Banco_ID PORT MAP(
		IR_in => IR_in, PC4_in => PC4, clk => clk, reset => reset_ID, load => load_ID, IR_ID => IR_ID, PC4_ID => PC4_ID,
		--Nuevo
		valid_I_IF => valid_I_IF, valid_I_ID => valid_I_ID,
		PC_exception => PC_out, PC_exception_ID => PC_exception_ID);
	--
	------------------------------------------Etapa ID-------------------------------------------------------------------
	Reg_Rs_ID <= IR_ID(25 DOWNTO 21);
	Reg_Rt_ID <= IR_ID(20 DOWNTO 16);
	--------------------------------------------------
	-- BANCOS DE REGISTROS

	-- s�lo se escribe si la instrucci�n en WB es v�lida
	RegWrite <= RegWrite_WB AND valid_I_WB;

	INT_Register_bank : BReg PORT MAP(clk => clk, reset => reset, RA => Reg_Rs_ID, RB => Reg_Rt_ID, RW => RW_WB, BusW => BusW, RegWrite => RegWrite, BusA => BusA, BusB => BusB);

	-------------------------------------------------------------------------------------
	sign_ext : Ext_signo PORT MAP(inm => IR_ID(15 DOWNTO 0), inm_ext => inm_ext);

	two_bits_shift : two_bits_shifter PORT MAP(Din => inm_ext, Dout => inm_ext_x4);

	adder_dir : adder32 PORT MAP(Din0 => inm_ext_x4, Din1 => PC4_ID, Dout => Dirsalto_ID);

	Z <= '1' WHEN (busA = busB) ELSE
		'0';

	-------------------------------------------------------------------------------------
	IR_op_code <= IR_ID(31 DOWNTO 26);

	-- Si la Instrucci�n en ID no es v�lida, todas las se�ales son 0
	UC_seg : UC PORT MAP(
		valid_I_ID => valid_I_ID, IR_op_code => IR_op_code, Branch => Branch_ID, RegDst => RegDst_ID, ALUSrc => ALUSrc_ID, MemWrite => MemWrite_ID,
		MemRead => MemRead_ID, MemtoReg => MemtoReg_ID, RegWrite => RegWrite_ID,
		--Se�ales nuevas
		-- RTE
		RTE => RTE_ID,
		-- Write_output
		undef => undef,
		write_output => write_output_UC);

	------------------------------------------------------------------------------------
	-- Completar:
	-- Write_output da la orden de escribir en el registro de salida. �Cuidado, no se deben escribir datos equivocados!
	-- IMPORTANTE: si hay dependencias de datos no hay que escribir en la salida
	-- A�adid lo necesario para evitar escrituras incorrectas
	Write_output <= write_output_UC;
	-- Fin completar;
	------------------------------------------------------------------------------------							
	-- Salto tomado se debe activar cada vez que la instrucci�n en D produzca un salto en la ejecuci�n.
	-- Eso incluye los saltos tomados en los BEQs (Z AND Branch_ID) y los saltos debidos a una RTE
	-- IMPORTANTE: si la instrucci�n no es v�lida no se salta
	salto_tomado <= ((Z AND Branch_ID) OR RTE_ID) AND valid_I_ID;

	------------------------Unidad de detenci�n-----------------------------------
	-- Deb�is completar la unidad para que genere las siguientes se�ales correctamente:
	-- Kill_IF: mata la instrucci�n que se est� leyendo en la etapa IF (para que no se ejecute)
	-- parar_ID: detiene la ejecuci�n de las etpas ID e IF cuando hay riesgos
	-- parar_EX: se utiliza para parar las etapas IF, ID y EX cuando la memoria no puede realizar la operaci�n solicitada en el ciclo actual (es decir cuando Mem_ready es 0). En el primer proyecto no hace flata parar.
	-- IMPORTANTE: para detectar un riesgo, primero comprobar que las instrucciones implicadas son v�lidas. �Las instrucciones invalidas no generan riesgos porque no son instrucciones que se vayan a ejecutar
	-------------------------------------------------------------------------------------

	Unidad_detencion_riesgos : UD PORT MAP(
		valid_I_ID => valid_I_ID, valid_I_EX => valid_I_EX, valid_I_MEM => valid_I_MEM, Reg_Rs_ID => Reg_Rs_ID, Reg_Rt_ID => Reg_Rt_ID, MemRead_EX => MemRead_EX, RW_EX => RW_EX, RegWrite_EX => RegWrite_EX,
		RW_Mem => RW_Mem, RegWrite_Mem => RegWrite_Mem, IR_op_code => IR_op_code, salto_tomado => salto_tomado,
		kill_IF => kill_IF, parar_ID => parar_ID,
		Mem_ready => Mem_ready, parar_EX => parar_EX);

	-- Si nos paran en ID marcamos como invalida la instrucci�n que mandamos a la etapa EX
	-- La instrucci�n de EX ser� v�lida el pr�ximo ciclo, si lo es la de ID y no hay detenci�n

	valid_I_EX_in <= valid_I_ID AND NOT(parar_ID);

	-------------------------------------------------------------------------------------
	-- si la operaci�n es aritm�tica (es decir: IR_op_code= "000001") miro el campo funct
	-- como s�lo hay 4 operaciones en la alu, basta con los bits menos significativos del campo func de la instrucci�n	
	-- si no es aritm�tica le damos el valor de la suma (000)
	ALUctrl_ID <= IR_ID(2 DOWNTO 0) WHEN IR_op_code = ARIT ELSE
		"000";
	-- Reset del banco EX: reseteamos el banco si hay una excepci�n aceptada, ya que en ese caso se matan las intrucciones en IF, ID y EX
	reset_EX <= (reset OR Exception_accepted);
	-- Banco ID/EX parte de enteros
	load_EX <= NOT(parar_EX);
	Banco_ID_EX : Banco_EX PORT MAP(
		clk => clk, reset => reset_EX, load => load_EX, busA => busA, busB => busB, busA_EX => busA_EX, busB_EX => busB_EX,
		RegDst_ID => RegDst_ID, ALUSrc_ID => ALUSrc_ID, MemWrite_ID => MemWrite_ID, MemRead_ID => MemRead_ID,
		MemtoReg_ID => MemtoReg_ID, RegWrite_ID => RegWrite_ID, RegDst_EX => RegDst_EX, ALUSrc_EX => ALUSrc_EX,
		MemWrite_EX => MemWrite_EX, MemRead_EX => MemRead_EX, MemtoReg_EX => MemtoReg_EX, RegWrite_EX => RegWrite_EX,
		-- Nuevo (para la anticipaci�n)
		Reg_Rs_ID => Reg_Rs_ID,
		Reg_Rs_EX => Reg_Rs_EX,
		--Fin nuevo
		ALUctrl_ID => ALUctrl_ID, ALUctrl_EX => ALUctrl_EX, inm_ext => inm_ext, inm_ext_EX => inm_ext_EX,
		Reg_Rt_ID => IR_ID(20 DOWNTO 16), Reg_Rd_ID => IR_ID(15 DOWNTO 11), Reg_Rt_EX => Reg_Rt_EX, Reg_Rd_EX => Reg_Rd_EX,
		--Nuevo
		valid_I_EX_in => valid_I_EX_in, valid_I_EX => valid_I_EX,
		PC_exception_ID => PC_exception_ID, PC_exception_EX => PC_exception_EX); --Sol: para llevar el PC a la etapa MEM		

	------------------------------------------Etapa EX-------------------------------------------------------------------
	---------------------------------------------------------------------------------
	-- Unidad de anticipaci�n de enteros incompleta. Deb�is dise�adla teniendo en cuenta que instrucciones lee y escribe cada instrucci�n
	-- Entradas: Reg_Rs_EX, Reg_Rt_EX, RegWrite_MEM, RW_MEM, RegWrite_WB, RW_WB
	-- Salidas: MUX_ctrl_A, MUX_ctrl_B
	Unidad_Ant_INT : UA PORT MAP(
		valid_I_MEM => valid_I_MEM, valid_I_WB => valid_I_WB, Reg_Rs_EX => Reg_Rs_EX, Reg_Rt_EX => Reg_Rt_EX, RegWrite_MEM => RegWrite_MEM,
		RW_MEM => RW_MEM, RegWrite_WB => RegWrite_WB, RW_WB => RW_WB, MUX_ctrl_A => MUX_ctrl_A, MUX_ctrl_B => MUX_ctrl_B);
	---------------------------------------------------------------------------------
	-- Muxes para la anticipaci�n
	Mux_A : mux4_1_32bits PORT MAP(DIn0 => BusA_EX, DIn1 => ALU_out_MEM, DIn2 => busW, DIn3 => x"00000000", ctrl => MUX_ctrl_A, Dout => Mux_A_out);
	Mux_B : mux4_1_32bits PORT MAP(DIn0 => BusB_EX, DIn1 => ALU_out_MEM, DIn2 => busW, DIn3 => x"00000000", ctrl => MUX_ctrl_B, Dout => Mux_B_out);

	----------------------------------------------------------------------------------
	muxALU_src : mux2_1 PORT MAP(Din0 => Mux_B_out, DIn1 => inm_ext_EX, ctrl => ALUSrc_EX, Dout => ALU_Src_out);

	ALU_MIPs : ALU PORT MAP(DA => Mux_A_out, DB => ALU_Src_out, ALUctrl => ALUctrl_EX, Dout => ALU_out_EX);
	mux_dst : mux2_5bits PORT MAP(Din0 => Reg_Rt_EX, DIn1 => Reg_Rd_EX, ctrl => RegDst_EX, Dout => RW_EX);

	-- No reseteamos el banco si hay una excepci�n porque podr�a llegar a mitad de una transferencia y corromper la MC 
	reset_MEM <= (reset);
	--si paramos en EX no hay que cargar una instrucci�n nueva en la etap MEM
	load_MEM <= NOT(parar_EX);
	Banco_EX_MEM : Banco_MEM PORT MAP(
		ALU_out_EX => ALU_out_EX, ALU_out_MEM => ALU_out_MEM, clk => clk, reset => reset_MEM, load => load_MEM, MemWrite_EX => MemWrite_EX,
		MemRead_EX => MemRead_EX, MemtoReg_EX => MemtoReg_EX, RegWrite_EX => RegWrite_EX, MemWrite_MEM => MemWrite_MEM, MemRead_MEM => MemRead_MEM,
		MemtoReg_MEM => MemtoReg_MEM, RegWrite_MEM => RegWrite_MEM,
		--sol:
		BusB_EX => Mux_B_out, -- antes pon�a BusB_EX, pero entonces los sw no pod�an hacer cortos en rt
		--fin sol
		BusB_MEM => BusB_MEM, RW_EX => RW_EX, RW_MEM => RW_MEM,
		-- Nuevo
		valid_I_EX => valid_I_EX, valid_I_MEM => valid_I_MEM,
		PC_exception_EX => PC_exception_EX, PC_exception_MEM => PC_exception_MEM); --Sol: para llevar el PC a la etapa MEM	
	--
	------------------------------------------Etapa MEM-------------------------------------------------------------------
	--

	WE <= MemWrite_MEM AND valid_I_MEM; --s�lo se escribe si es una instrucci�n v�lida
	RE <= MemRead_MEM AND valid_I_MEM; --s�lo se lee si es una instrucci�n v�lida

	Mem_D : Data_Memory_Subsystem PORT MAP(CLK => CLK, ADDR => ALU_out_MEM, Din => BusB_MEM, WE => MemWrite_MEM, RE => MemRead_MEM, reset => reset, IO_input => IO_input, Mem_ready => Mem_ready, Dout => Mem_out, Data_abort => Data_abort);
	-- parar_EX indica que hay que detener la etapa de memoria (se usa m�s adelante cuando la jerarqu�a de memoria sea m�s compleja)
	-- La instrucci�n en WB ser� v�lida el pr�ximo ciclo si la instrucci�n en Mem es v�lida y no hay que parar 
	valid_I_WB_in <= valid_I_MEM AND NOT(parar_EX);

	Banco_MEM_WB : Banco_WB PORT MAP(
		ALU_out_MEM => ALU_out_MEM, ALU_out_WB => ALU_out_WB, Mem_out => Mem_out, MDR => MDR, clk => clk, reset => reset, load => '1',
		MemtoReg_MEM => MemtoReg_MEM, RegWrite_MEM => RegWrite_MEM, MemtoReg_WB => MemtoReg_WB, RegWrite_WB => RegWrite_WB,
		RW_MEM => RW_MEM, RW_WB => RW_WB,
		-- Nuevo
		valid_I_WB_in => valid_I_WB_in, valid_I_WB => valid_I_WB);

	--
	------------------------------------------Etapa WB-------------------------------------------------------------------
	--											
	mux_busW : mux2_1 PORT MAP(Din0 => ALU_out_WB, DIn1 => MDR, ctrl => MemtoReg_WB, Dout => busW);

	-- *********************************************************************************************	-----------
	-- IO_output son 32 pines de salida para que el MIPS pueda comunicarse con el exterior
	-- En la etapa ID de la instrucci�n WRO Rs el contenido de Rs se carga en el registro de salida que se puede ver desde el exterior
	output_reg : reg GENERIC MAP(size => 32)
	PORT MAP(Din => BusA, clk => clk, reset => reset, load => write_output, Dout => IO_output);
	--------------------------------------------------------------------------------------------------
	----------- Contadores de eventos
	-------------------------------------------------------------------------------------------------- 
	-- Contador de ciclos totales
	cont_cycles : counter GENERIC MAP(size => 16)
	PORT MAP(clk => clk, reset => reset, count_enable => inc_cycles, count => cycles);
	-- Contador de Instrucciones ejecutadas
	cont_I : counter GENERIC MAP(size => 8)
	PORT MAP(clk => clk, reset => reset, count_enable => inc_I, count => Ins);
	-- Contador de detenciones por riesgos de datos						
	cont_data_stalls : counter GENERIC MAP(size => 8)
	PORT MAP(clk => clk, reset => reset, count_enable => inc_data_stalls, count => data_stalls);
	-- Contador de detenciones por riesgos de control							
	cont_control_stalls : counter GENERIC MAP(size => 8)
	PORT MAP(clk => clk, reset => reset, count_enable => inc_control_stalls, count => control_stalls);
	-- Contador de n�mero de excepciones							
	cont_Exceptions : counter GENERIC MAP(size => 8)
	PORT MAP(clk => clk, reset => reset, count_enable => inc_Exceptions, count => Exceptions);
	-- Contador de ciclos ejecutando excepciones						
	cont_Exceptions_cycles : counter GENERIC MAP(size => 8)
	PORT MAP(clk => clk, reset => reset, count_enable => inc_Exception_cycles, count => Exception_cycles);
	------------------------------------------------------------------------------------
	-- Completar:
	inc_cycles <= '1';--Done
	inc_I <= '0'; --completar
	inc_data_stalls <= '0'; --completar
	inc_control_stalls <= '0'; --completar
	inc_Exceptions <= '0';--completar
	inc_Exception_cycles <= '0'; --completar	
	-- Fin completar;
	------------------------------------------------------------------------------------			
END Behavioral;