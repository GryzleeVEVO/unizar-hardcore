----------------------------------------------------------------------------------
-- Description: Mips segmentado tal y como lo hemos estudiado en clase. Sus características son:
-- Saltos 1-retardados
-- instrucciones aritméticas, LW, SW y BEQ
-- MI y MD de 128 palabras de 32 bits
-- Excepciones: IRQ, ABORT y UNDEF
-- Línea de IRQ
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
        IO_input : IN STD_LOGIC_VECTOR (31 DOWNTO 0); -- 32 bits de entrada para el MIPS para IO. En el primer proyecto no se usa. En el segundo si
        IO_output : OUT STD_LOGIC_VECTOR (31 DOWNTO 0)); -- 32 bits de salida para el MIPS para IO
END MIPs_segmentado;

ARCHITECTURE Behavioral OF MIPs_segmentado IS

    -- ***** COMPONENTES UTILIZADOS *****

    -- Registro
    COMPONENT reg IS
        GENERIC (size : NATURAL := 32); -- por defecto son de 32 bits, pero se puede usar cualquier tamaño
        PORT (
            Din : IN STD_LOGIC_VECTOR (size - 1 DOWNTO 0);
            clk : IN STD_LOGIC;
            reset : IN STD_LOGIC;
            load : IN STD_LOGIC;
            Dout : OUT STD_LOGIC_VECTOR (size - 1 DOWNTO 0));
    END COMPONENT;

    -- Sumador
    COMPONENT adder32 IS
        PORT (
            Din0 : IN STD_LOGIC_VECTOR (31 DOWNTO 0);
            Din1 : IN STD_LOGIC_VECTOR (31 DOWNTO 0);
            Dout : OUT STD_LOGIC_VECTOR (31 DOWNTO 0));
    END COMPONENT;

    -- Multiplexor 2x1
    COMPONENT mux2_1 IS
        PORT (
            DIn0 : IN STD_LOGIC_VECTOR (31 DOWNTO 0);
            DIn1 : IN STD_LOGIC_VECTOR (31 DOWNTO 0);
            ctrl : IN STD_LOGIC;
            Dout : OUT STD_LOGIC_VECTOR (31 DOWNTO 0));
    END COMPONENT;

    -- Memoria de datos
    COMPONENT Data_Memory_Subsystem IS PORT (
        CLK : IN STD_LOGIC;
        reset : IN STD_LOGIC;
        ADDR : IN STD_LOGIC_VECTOR (31 DOWNTO 0); -- Dir solicitada por el Mips
        Din : IN STD_LOGIC_VECTOR (31 DOWNTO 0); -- entrada de datos desde el Mips
        WE : IN STD_LOGIC; -- write enable	del MIPS
        RE : IN STD_LOGIC; -- read enable del MIPS	
        IO_input : IN STD_LOGIC_VECTOR (31 DOWNTO 0); -- dato que viene de una entrada del sistema
        Mem_ready : OUT STD_LOGIC; -- indica si podemos hacer la operación solicitada en el ciclo actual
        Data_abort : OUT STD_LOGIC; -- indica que el último acceso a memoria ha sido un error
        Dout : OUT STD_LOGIC_VECTOR (31 DOWNTO 0) -- dato que se envía al Mips
        );
    END COMPONENT;

    -- Memoria de instrucciones
    COMPONENT memoriaRAM_I IS PORT (
        CLK : IN STD_LOGIC;
        ADDR : IN STD_LOGIC_VECTOR (31 DOWNTO 0); -- Dir 
        Din : IN STD_LOGIC_VECTOR (31 DOWNTO 0);-- entrada de datos para el puerto de escritura
        WE : IN STD_LOGIC; -- write enable	
        RE : IN STD_LOGIC; -- read enable		  
        Dout : OUT STD_LOGIC_VECTOR (31 DOWNTO 0));
    END COMPONENT;

    -- Bancos IF -> ID
    COMPONENT Banco_ID IS
        PORT (
            IR_in : IN STD_LOGIC_VECTOR (31 DOWNTO 0); -- instrucción leida en IF
            PC4_in : IN STD_LOGIC_VECTOR (31 DOWNTO 0); -- PC+4 sumado en IF
            clk : IN STD_LOGIC;
            reset : IN STD_LOGIC;
            load : IN STD_LOGIC;
            IR_ID : OUT STD_LOGIC_VECTOR (31 DOWNTO 0); -- instrucción en la etapa ID
            PC4_ID : OUT STD_LOGIC_VECTOR (31 DOWNTO 0);
            -- nuevo para excepciones
            PC_exception : IN STD_LOGIC_VECTOR (31 DOWNTO 0); -- PC al que se volverá si justo esta instrucción está en MEM cuando llega una excepción. 
            PC_exception_ID : OUT STD_LOGIC_VECTOR (31 DOWNTO 0);-- PC+4 en la etapa ID
            -- bits de validez
            valid_I_IF : IN STD_LOGIC;
            valid_I_ID : OUT STD_LOGIC);
    END COMPONENT;

    -- Banco de registros
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

    -- Extensor de signo
    COMPONENT Ext_signo IS
        PORT (
            inm : IN STD_LOGIC_VECTOR (15 DOWNTO 0);
            inm_ext : OUT STD_LOGIC_VECTOR (31 DOWNTO 0));
    END COMPONENT;

    -- Shifter LSL2 
    COMPONENT two_bits_shifter IS
        PORT (
            Din : IN STD_LOGIC_VECTOR (31 DOWNTO 0);
            Dout : OUT STD_LOGIC_VECTOR (31 DOWNTO 0));
    END COMPONENT;

    -- Unidad de control
    COMPONENT UC IS
        PORT (
            valid_I_ID : IN STD_LOGIC; -- indica si es una instrucción válida			
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
            Write_output : OUT STD_LOGIC; -- indica que es una instrucción que escribe en la salida
            UNDEF : OUT STD_LOGIC -- indica que el código de operación no pertenence a una instrucción conocida
            -- Fin Nuevas señales
        );
    END COMPONENT;

    -- Unidad de detección de riesgos
    COMPONENT UD IS
        PORT (
            valid_I_ID : IN STD_LOGIC; -- indica si es una instrucción válida
            valid_I_EX : IN STD_LOGIC; -- indica si es una instrucción de EX es válida
            valid_I_MEM : IN STD_LOGIC; -- indica si es una instrucción de MEM es válida
            Reg_Rs_ID : IN STD_LOGIC_VECTOR (4 DOWNTO 0); -- registros Rs y Rt en la etapa ID
            Reg_Rt_ID : IN STD_LOGIC_VECTOR (4 DOWNTO 0);
            MemRead_EX : IN STD_LOGIC; -- información sobre la instrucción en EX (destino, si lee de memoria y si escribe en registro)
            RegWrite_EX : IN STD_LOGIC;
            RW_EX : IN STD_LOGIC_VECTOR (4 DOWNTO 0);
            RegWrite_Mem : IN STD_LOGIC;-- informacion sobre la instruccion en Mem (destino y si escribe en registro)
            RW_Mem : IN STD_LOGIC_VECTOR (4 DOWNTO 0);
            IR_op_code : IN STD_LOGIC_VECTOR (5 DOWNTO 0); -- código de operación de la instrucción en IEEE
            salto_tomado : IN STD_LOGIC; -- 1 cuando se produce un salto 0 en caso contrario
            -- Nuevo
            Mem_ready : IN STD_LOGIC; -- 1 cuando la memoria puede realizar la operación solicitada en el ciclo actual
            parar_EX : OUT STD_LOGIC; -- Indica que las etapas MEM y previas deben parar
            Kill_IF : OUT STD_LOGIC; -- Indica que la instrucción en IF no debe ejecutarse (fallo en la predicción de salto tomado)
            Parar_ID : OUT STD_LOGIC); -- Indica que las etapas ID y previas deben parar
    END COMPONENT;

    -- Bancos ID -> EX
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
            -- Fin nuevo
            ALUctrl_ID : IN STD_LOGIC_VECTOR (2 DOWNTO 0);
            ALUctrl_EX : OUT STD_LOGIC_VECTOR (2 DOWNTO 0);
            Reg_Rt_ID : IN STD_LOGIC_VECTOR (4 DOWNTO 0);
            Reg_Rd_ID : IN STD_LOGIC_VECTOR (4 DOWNTO 0);
            Reg_Rt_EX : OUT STD_LOGIC_VECTOR (4 DOWNTO 0);
            Reg_Rd_EX : OUT STD_LOGIC_VECTOR (4 DOWNTO 0);
            -- Nuevo excepción
            PC_exception_ID : IN STD_LOGIC_VECTOR (31 DOWNTO 0);
            PC_exception_EX : OUT STD_LOGIC_VECTOR (31 DOWNTO 0);
            -- bits de validez
            valid_I_EX_in : IN STD_LOGIC;
            valid_I_EX : OUT STD_LOGIC);
    END COMPONENT;

    -- Unidad de anticipación de operandos
    COMPONENT UA
        PORT (
            valid_I_MEM : IN STD_LOGIC; -- indica si es una instrucción de MEM es válida
            valid_I_WB : IN STD_LOGIC; -- indica si es una instrucción de WB es válida
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

    -- Mux 4x1
    COMPONENT mux4_1_32bits IS
        PORT (
            DIn0 : IN STD_LOGIC_VECTOR (31 DOWNTO 0);
            DIn1 : IN STD_LOGIC_VECTOR (31 DOWNTO 0);
            DIn2 : IN STD_LOGIC_VECTOR (31 DOWNTO 0);
            DIn3 : IN STD_LOGIC_VECTOR (31 DOWNTO 0);
            ctrl : IN STD_LOGIC_VECTOR(1 DOWNTO 0);
            Dout : OUT STD_LOGIC_VECTOR (31 DOWNTO 0));
    END COMPONENT;

    -- ALU
    COMPONENT ALU
        PORT (
            DA : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
            DB : IN STD_LOGIC_VECTOR(31 DOWNTO 0);
            ALUctrl : IN STD_LOGIC_VECTOR(2 DOWNTO 0);
            Dout : OUT STD_LOGIC_VECTOR(31 DOWNTO 0)
        );
    END COMPONENT;

    -- Mux 2x1 5b
    COMPONENT mux2_5bits IS
        PORT (
            DIn0 : IN STD_LOGIC_VECTOR (4 DOWNTO 0);
            DIn1 : IN STD_LOGIC_VECTOR (4 DOWNTO 0);
            ctrl : IN STD_LOGIC;
            Dout : OUT STD_LOGIC_VECTOR (4 DOWNTO 0));
    END COMPONENT;

    -- Bancos EX -> MEM
    COMPONENT Banco_MEM
        PORT (
            ALU_out_EX : IN STD_LOGIC_VECTOR (31 DOWNTO 0);
            ALU_out_MEM : OUT STD_LOGIC_VECTOR (31 DOWNTO 0); -- instrucción leida en IF
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
            -- Nuevo excepción
            PC_exception_EX : IN STD_LOGIC_VECTOR (31 DOWNTO 0);
            PC_exception_MEM : OUT STD_LOGIC_VECTOR (31 DOWNTO 0);
            -- bits de validez
            valid_I_EX : IN STD_LOGIC;
            valid_I_MEM : OUT STD_LOGIC);
    END COMPONENT;

    -- Bancos MEM -> WB
    COMPONENT Banco_WB
        PORT (
            ALU_out_MEM : IN STD_LOGIC_VECTOR (31 DOWNTO 0);
            ALU_out_WB : OUT STD_LOGIC_VECTOR (31 DOWNTO 0);
            MEM_out : IN STD_LOGIC_VECTOR (31 DOWNTO 0);
            MDR : OUT STD_LOGIC_VECTOR (31 DOWNTO 0); -- memory data register
            clk : IN STD_LOGIC;
            reset : IN STD_LOGIC;
            load : IN STD_LOGIC;
            MemtoReg_MEM : IN STD_LOGIC;
            RegWrite_MEM : IN STD_LOGIC;
            MemtoReg_WB : OUT STD_LOGIC;
            RegWrite_WB : OUT STD_LOGIC;
            RW_MEM : IN STD_LOGIC_VECTOR (4 DOWNTO 0); -- registro destino de la escritura
            RW_WB : OUT STD_LOGIC_VECTOR (4 DOWNTO 0); -- PC+4 en la etapa IDend Banco_WB;
            -- bits de validez
            valid_I_WB_in : IN STD_LOGIC;
            valid_I_WB : OUT STD_LOGIC);
    END COMPONENT;

    -- Contador
    COMPONENT counter
        GENERIC (
            size : INTEGER := 10);
        PORT (
            clk : IN STD_LOGIC;
            reset : IN STD_LOGIC;
            count_enable : IN STD_LOGIC;
            count : OUT STD_LOGIC_VECTOR (size - 1 DOWNTO 0));
    END COMPONENT;

    -- ***** DECLARACIÓN SEÑALES INTERNAS *****

    -- señales internas MIPS	
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
    -- señales solución
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
    SIGNAL Ins, data_stalls, control_stalls, Exceptions, Exception_cycles, paradas_mem : STD_LOGIC_VECTOR(7 DOWNTO 0);
    SIGNAL inc_cycles, inc_I, inc_data_stalls, inc_control_stalls, inc_Exceptions, inc_Exception_cycles, inc_paradas_mem : STD_LOGIC;
    -- interfaz con memoria
    SIGNAL Mem_ready : STD_LOGIC;

BEGIN
    -- ***** GESTIÓN DE IRQ *****
    -- Tipo			| Descripción		     | @ Vector excepciones
    -- ------------ | ---------------------- | --------------------
    -- IRQ			| Excpeción externa      | 0x00000004
    -- Data_abort	| Dirección no alineada  | 0x00000008
    -- UNDEF		| Opcode desconocido     | 0x0000000C

    -- Registro de estado
    -- Bit 1: IRQ Habitlitadas/deshabilitadas
    -- Bit 0: Modo normal/excepción 
    status_reg : reg GENERIC MAP(size => 2)
    PORT MAP(
        Din => status_input,
        clk => clk,
        reset => reset,
        load => update_status,
        Dout => MIPS_status
    );

    -- Acepta excepciones si hay alguna y estan habilitadas
    Exception_accepted <= (NOT MIPS_status(1)) AND (IRQ OR Data_Abort OR Undef);

    -- Actualiza estado cuando se produce o se sale de una excepción
    -- Debe comprobar que sea válida la RTE
    update_status <= Exception_accepted OR (RTE_ID AND Valid_I_ID);

    -- Selector de estado 
    -- Desactiva IRQ si se procesa excepción
    status_input <=
        "11" WHEN (Exception_accepted = '1') ELSE
        "00";

    -- Selector instrucción de retorno de excepción
    Return_I <=
        PC_exception_EX WHEN (valid_I_EX = '1') ELSE
        PC_exception_ID WHEN (valid_I_ID = '1') ELSE -- Si no hay instr. en EX
        PC_out; -- Si no hay instr. en ID

    -- Registro almacen de dirección de retorno 
    Exception_LR : reg GENERIC MAP(size => 32)
    PORT MAP(
        Din => Return_I,
        clk => clk, reset => reset,
        load => Exception_accepted,
        Dout => Exception_LR_output
    );

    -- ***** FETCH *****

    -- PC
    pc : reg GENERIC MAP(size => 32)
    PORT MAP(
        Din => PC_in,
        clk => clk,
        reset => reset,
        load => load_PC,
        Dout => PC_out
    );

    -- Carga en PC salvo si hay parada y no hay excepción que vaya a sustituir 
    -- a la instrucción que va a esperar
    load_PC <= NOT (Parar_ID AND NOT Exception_accepted);

    -- Sumador PC+4
    adder_4 : adder32 PORT MAP(
        Din0 => PC_out,
        Din1 => x"00000004",
        Dout => PC4
    );

    -- Dirección a cargar en PC
    PC_in <=
        x"00000008" WHEN Exception_accepted = '1' AND Data_abort = '1' ELSE -- Data_abort
        x"0000000C" WHEN Exception_accepted = '1' AND Undef = '1' ELSE -- UNDEF
        x"00000004" WHEN Exception_accepted = '1' AND IRQ = '1' ELSE -- IRQ
        Exception_LR_output WHEN salto_tomado = '1' AND RTE_ID = '1' ELSE -- Regreso de excepción
        Dirsalto_ID WHEN salto_tomado = '1' AND RTE_ID = '0' ELSE -- BEQ con salto
        PC4; -- Avanzar a siguiente instrucción

    -- Memoria de instrucciones.
    Mem_I : memoriaRAM_I PORT MAP(
        CLK => CLK,
        ADDR => PC_out,
        Din => x"00000000",
        WE => '0', -- Siempre lee, nunca escribe
        RE => '1',
        Dout => IR_in
    );

    -- Limpiar banco IF -> ID cuando hay una excepción o se reinicia el MIPS  
    reset_ID <= reset OR Exception_accepted;

    -- Señal de anulación de instrucción en IF
    valid_I_IF <= NOT(kill_IF);

    -- No pasa a ID si se detecta un riesgo de datos o de control 
    load_ID <= NOT(parar_ID);

    -- Banco IF -> ID
    Banco_IF_ID : Banco_ID PORT MAP(
        IR_in => IR_in,
        PC4_in => PC4,
        clk => clk,
        reset => reset_ID,
        load => load_ID,
        IR_ID => IR_ID,
        PC4_ID => PC4_ID,
        -- Nuevo
        valid_I_IF => valid_I_IF,
        valid_I_ID => valid_I_ID,
        PC_exception => PC_out,
        PC_exception_ID => PC_exception_ID
    );

    -- ***** DECODE *****

    -- Bancos de regisros seleccionados
    Reg_Rs_ID <= IR_ID(25 DOWNTO 21);
    Reg_Rt_ID <= IR_ID(20 DOWNTO 16);
    -- Solo escribir si WB es válida
    RegWrite <= RegWrite_WB AND valid_I_WB;

    -- Banco de registros
    INT_Register_bank : BReg PORT MAP(
        clk => clk,
        reset => reset,
        RA => Reg_Rs_ID,
        RB => Reg_Rt_ID,
        RW => RW_WB,
        BusW => BusW,
        RegWrite => RegWrite,
        BusA => BusA,
        BusB => BusB
    );

    -- Extensor de signo
    sign_ext : Ext_signo PORT MAP(inm => IR_ID(15 DOWNTO 0), inm_ext => inm_ext);

    -- Shifter
    two_bits_shift : two_bits_shifter PORT MAP(Din => inm_ext, Dout => inm_ext_x4);

    -- Sumador dirección
    adder_dir : adder32 PORT MAP(
        Din0 => inm_ext_x4,
        Din1 => PC4_ID,
        Dout => Dirsalto_ID
    );

    -- Flag Z
    Z <=
        '1' WHEN (busA = busB) ELSE
        '0';

    -- Opcode en ID
    IR_op_code <= IR_ID(31 DOWNTO 26);

    -- Si la instrucción en ID no es válida, todas las señales son 0
    UC_seg : UC PORT MAP(
        valid_I_ID => valid_I_ID,
        IR_op_code => IR_op_code,
        Branch => Branch_ID,
        RegDst => RegDst_ID,
        ALUSrc => ALUSrc_ID,
        MemWrite => MemWrite_ID,
        MemRead => MemRead_ID,
        MemtoReg => MemtoReg_ID,
        RegWrite => RegWrite_ID,
        -- señales nuevas
        RTE => RTE_ID, -- RTE
        undef => undef,
        write_output => write_output_UC -- Write_output
    );

    -- Write_output es deshabilitado si hay que parar por dependencia de datos
    Write_output <= NOT Parar_ID AND write_output_UC AND valid_I_ID;

    -- Indica si se produce salto
    salto_tomado <= ((Z AND Branch_ID) OR RTE_ID) AND valid_I_ID;

    -- Unidad de detención
    Unidad_detencion_riesgos : UD PORT MAP(
        valid_I_ID => valid_I_ID,
        valid_I_EX => valid_I_EX,
        valid_I_MEM => valid_I_MEM,
        Reg_Rs_ID => Reg_Rs_ID,
        Reg_Rt_ID => Reg_Rt_ID,
        MemRead_EX => MemRead_EX,
        RW_EX => RW_EX,
        RegWrite_EX => RegWrite_EX,
        RW_Mem => RW_Mem,
        RegWrite_Mem => RegWrite_Mem,
        IR_op_code => IR_op_code,
        salto_tomado => salto_tomado,
        kill_IF => kill_IF,
        parar_ID => parar_ID,
        Mem_ready => Mem_ready,
        parar_EX => parar_EX
    );

    -- Si no hay parada o salto, marca siguiente EX como válido
    valid_I_EX_in <= valid_I_ID AND NOT (parar_ID OR salto_tomado);

    -- Selector operación ALU
    ALUctrl_ID <=
        IR_ID(2 DOWNTO 0) WHEN IR_op_code = ARIT ELSE
        "000"; -- No es instrucción aritmérica

    -- Limpia ID -> EX si se reinicia o hay excepción
    reset_EX <= (reset OR Exception_accepted);

    -- Banco ID -> EX
    load_EX <= NOT(parar_EX);
    Banco_ID_EX : Banco_EX PORT MAP(
        clk => clk,
        reset => reset_EX,
        load => load_EX,
        busA => busA,
        busB => busB,
        busA_EX => busA_EX,
        busB_EX => busB_EX,
        RegDst_ID => RegDst_ID,
        ALUSrc_ID => ALUSrc_ID,
        MemWrite_ID => MemWrite_ID,
        MemRead_ID => MemRead_ID,
        MemtoReg_ID => MemtoReg_ID,
        RegWrite_ID => RegWrite_ID,
        RegDst_EX => RegDst_EX,
        ALUSrc_EX => ALUSrc_EX,
        MemWrite_EX => MemWrite_EX,
        MemRead_EX => MemRead_EX,
        MemtoReg_EX => MemtoReg_EX,
        RegWrite_EX => RegWrite_EX,
        -- Nuevo (para la anticipación)
        Reg_Rs_ID => Reg_Rs_ID,
        Reg_Rs_EX => Reg_Rs_EX,
        -- Fin nuevo
        ALUctrl_ID => ALUctrl_ID,
        ALUctrl_EX => ALUctrl_EX,
        inm_ext => inm_ext,
        inm_ext_EX => inm_ext_EX,
        Reg_Rt_ID => IR_ID(20 DOWNTO 16),
        Reg_Rd_ID => IR_ID(15 DOWNTO 11),
        Reg_Rt_EX => Reg_Rt_EX,
        Reg_Rd_EX => Reg_Rd_EX,
        -- Nuevo
        valid_I_EX_in => valid_I_EX_in,
        valid_I_EX => valid_I_EX,
        PC_exception_ID => PC_exception_ID,
        PC_exception_EX => PC_exception_EX
    ); -- Sol: para llevar el PC a la etapa MEM		

    -- ***** Etapa EX *****

    -- Unidad anticipación
    Unidad_Ant_INT : UA PORT MAP(
        valid_I_MEM => valid_I_MEM,
        valid_I_WB => valid_I_WB,
        Reg_Rs_EX => Reg_Rs_EX,
        Reg_Rt_EX => Reg_Rt_EX,
        RegWrite_MEM => RegWrite_MEM,
        RW_MEM => RW_MEM,
        RegWrite_WB => RegWrite_WB,
        RW_WB => RW_WB,
        MUX_ctrl_A => MUX_ctrl_A,
        MUX_ctrl_B => MUX_ctrl_B
    );

    -- Selector bus A
    Mux_A : mux4_1_32bits PORT MAP(
        DIn0 => BusA_EX, -- Bus A
        DIn1 => ALU_out_MEM, -- ALU memoria
        DIn2 => busW, -- Bus escritura WB
        DIn3 => x"00000000", -- A cero
        ctrl => MUX_ctrl_A, -- UA
        Dout => Mux_A_out
    );

    -- Selector bus B
    Mux_B : mux4_1_32bits PORT MAP(
        DIn0 => BusB_EX, -- Bus B
        DIn1 => ALU_out_MEM, -- ALU memoria 
        DIn2 => busW, -- Bus escritura WB
        DIn3 => x"00000000", -- A cero
        ctrl => MUX_ctrl_B, -- UA
        Dout => Mux_B_out
    );

    -- Selector operando B ALU
    muxALU_src : mux2_1 PORT MAP(
        Din0 => Mux_B_out, -- Bus B
        DIn1 => inm_ext_EX, -- Inmediato
        ctrl => ALUSrc_EX, -- Control
        Dout => ALU_Src_out
    );

    -- ALU
    ALU_MIPs : ALU PORT MAP(
        DA => Mux_A_out,
        DB => ALU_Src_out,
        ALUctrl => ALUctrl_EX,
        Dout => ALU_out_EX
    );

    -- Destino WB en etapa EX
    mux_dst : mux2_5bits PORT MAP(
        Din0 => Reg_Rt_EX, -- Rt
        DIn1 => Reg_Rd_EX, -- Rs
        ctrl => RegDst_EX, -- Control
        Dout => RW_EX
    );

    -- Solo resetear en caso de reinicio
    reset_MEM <= (reset OR Exception_accepted);

    -- No carga memoria si EX se detiene o si hay parada de datos
    load_MEM <= NOT(parar_EX); -- AND mem_ready


    -- Bancos EX -> MEM
    Banco_EX_MEM : Banco_MEM PORT MAP(
        ALU_out_EX => ALU_out_EX,
        ALU_out_MEM => ALU_out_MEM,
        clk => clk,
        reset => reset_MEM,
        load => load_MEM,
        MemWrite_EX => MemWrite_EX,
        MemRead_EX => MemRead_EX,
        MemtoReg_EX => MemtoReg_EX,
        RegWrite_EX => RegWrite_EX,
        MemWrite_MEM => MemWrite_MEM,
        MemRead_MEM => MemRead_MEM,
        MemtoReg_MEM => MemtoReg_MEM,
        RegWrite_MEM => RegWrite_MEM,
        -- sol:
        BusB_EX => Mux_B_out, -- antes ponía BusB_EX, pero entonces los sw no podían hacer cortos en rt
        -- fin sol
        BusB_MEM => BusB_MEM,
        RW_EX => RW_EX,
        RW_MEM => RW_MEM,
        -- Nuevo
        valid_I_EX => valid_I_EX,
        valid_I_MEM => valid_I_MEM,
        PC_exception_EX => PC_exception_EX,
        PC_exception_MEM => PC_exception_MEM
    ); -- Sol: para llevar el PC a la etapa MEM	

    -- ***** Etapa MEM ******

    -- Sólo se lee o escribe si es una instrucción válida
    WE <= MemWrite_MEM AND valid_I_MEM;
    RE <= MemRead_MEM AND valid_I_MEM;

    -- Memoria de datos
    Mem_D : Data_Memory_Subsystem PORT MAP(
        CLK => CLK,
        ADDR => ALU_out_MEM,
        Din => BusB_MEM,
        WE => WE,
        RE => RE,
        reset => reset,
        IO_input => IO_input,
        Mem_ready => Mem_ready,
        Dout => Mem_out,
        Data_abort => Data_abort
    );

    -- Instrucción de memoria válida si no hay que parar
    valid_I_WB_in <= valid_I_MEM AND NOT(parar_EX);

    -- Bancos MEM -> WB
    Banco_MEM_WB : Banco_WB PORT MAP(
        ALU_out_MEM => ALU_out_MEM,
        ALU_out_WB => ALU_out_WB,
        Mem_out => Mem_out,
        MDR => MDR,
        clk => clk,
        reset => reset,
        load => Mem_ready, -- Si memoria no está lista, no avanzar
        MemtoReg_MEM => MemtoReg_MEM,
        RegWrite_MEM => RegWrite_MEM,
        MemtoReg_WB => MemtoReg_WB,
        RegWrite_WB => RegWrite_WB,
        RW_MEM => RW_MEM,
        RW_WB => RW_WB,
        -- Nuevo
        valid_I_WB_in => valid_I_WB_in,
        valid_I_WB => valid_I_WB
    );

    -- ***** Etapa WB ***** 				

    -- Selector busW
    mux_busW : mux2_1 PORT MAP(
        Din0 => ALU_out_WB, -- ALU
        DIn1 => MDR, -- MDR
        ctrl => MemtoReg_WB,
        Dout => busW
    );

    -- Registro de salida
    output_reg : reg GENERIC MAP(size => 32)
    PORT MAP(
        Din => BusA,
        clk => clk,
        reset => reset,
        load => write_output,
        Dout => IO_output
    );

    -- ***** Contadores de eventos *****

    -- Contador de ciclos totales
    cont_cycles : counter GENERIC MAP(size => 16)
    PORT MAP(
        clk => clk,
        reset => reset,
        count_enable => inc_cycles,
        count => cycles
    );

    -- Contador de Instrucciones ejecutadas
    cont_I : counter GENERIC MAP(size => 8)
    PORT MAP(
        clk => clk,
        reset => reset,
        count_enable => inc_I,
        count => Ins
    );

    -- Contador de detenciones por riesgos de datos						
    cont_data_stalls : counter GENERIC MAP(size => 8)
    PORT MAP(
        clk => clk,
        reset => reset,
        count_enable => inc_data_stalls,
        count => data_stalls
    );

    -- Contador de detenciones por riesgos de control							
    cont_control_stalls : counter GENERIC MAP(size => 8)
    PORT MAP(
        clk => clk,
        reset => reset,
        count_enable => inc_control_stalls,
        count => control_stalls
    );

    -- Contador de número de excepciones							
    cont_Exceptions : counter GENERIC MAP(size => 8)
    PORT MAP(
        clk => clk,
        reset => reset,
        count_enable => inc_Exceptions,
        count => Exceptions
    );

    -- Contador de ciclos ejecutando excepciones						
    cont_Exceptions_cycles : counter GENERIC MAP(size => 8)
    PORT MAP(
        clk => clk,
        reset => reset,
        count_enable => inc_Exception_cycles,
        count => Exception_cycles
    );

    cont_paradas_mem : counter GENERIC MAP(size => 8)
    PORT MAP(
        clk => clk,
        reset => reset,
        count_enable => inc_paradas_mem,
        count => paradas_mem
    );

    inc_cycles <= '1'; -- Incrementa siempre

    inc_I <=
        '1' WHEN valid_I_WB = '1' ELSE
        '0';
    inc_data_stalls <=
        '1' WHEN Mem_ready = '1' AND Parar_ID = '1' AND parar_EX = '0' ELSE
        '0';
    inc_control_stalls <=
        '1' WHEN Mem_ready = '1' AND Kill_IF = '1' AND Parar_ID = '0' ELSE
        '0';
    inc_Exceptions <=
        '1' WHEN Exception_accepted = '1' ELSE
        '0';
    inc_Exception_cycles <=
        '1' WHEN MIPS_status(0) = '1' ELSE
        '0';
    inc_paradas_mem <=
        '1' WHEN Mem_ready = '0' ELSE
        '0';

END Behavioral;