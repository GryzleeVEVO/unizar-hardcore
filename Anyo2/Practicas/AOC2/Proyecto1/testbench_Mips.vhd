-- TestBench Template 

LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.numeric_std.ALL;

ENTITY testbench IS
END testbench;

ARCHITECTURE behavior OF testbench IS

	-- Component Declaration
	COMPONENT MIPs_segmentado IS
		PORT (
			clk : IN STD_LOGIC;
			reset : IN STD_LOGIC;
			IRQ : IN STD_LOGIC;
			IO_input : IN STD_LOGIC_VECTOR (31 DOWNTO 0); -- 32 bits de entrada para el MIPS para IO
			IO_output : OUT STD_LOGIC_VECTOR (31 DOWNTO 0)); -- 32 bits de salida para el MIPS para IO
	END COMPONENT;

	SIGNAL clk, reset, IRQ : STD_LOGIC;
	SIGNAL IO_output, IO_input : STD_LOGIC_VECTOR(31 DOWNTO 0);

	-- Clock period definitions
	CONSTANT CLK_period : TIME := 10 ns;
BEGIN

	-- Component Instantiation
	uut : MIPs_segmentado PORT MAP(clk => clk, reset => reset, IRQ => IRQ, IO_input => IO_input, IO_output => IO_output);

	-- Clock process definitions
	CLK_process : PROCESS
	BEGIN
		CLK <= '0';
		WAIT FOR CLK_period/2;
		CLK <= '1';
		WAIT FOR CLK_period/2;
	END PROCESS;

	stim_proc : PROCESS
	BEGIN
		IRQ <= '0';
		IO_input <= x"00000000";
		reset <= '1';
		WAIT FOR CLK_period * 2;
		reset <= '0';

		-- SECUENCIAS INTERRUPCIONES

		-- Código IRQ

		--WAIT FOR CLK_period * 4;
		--IRQ <= '1';
		--WAIT FOR CLK_period;
		--IRQ <= '0';
		--WAIT FOR CLK_period * 41;
		--IRQ <= '1';
		--WAIT FOR CLK_period;
		--IRQ <= '0';
		--WAIT FOR CLK_period * 20;
		--IRQ <= '1';

		-- Test 4

		-- Genera una interrupción durante el primer ciclo de un riesgo de datos en el primer BEQ
		-- No se hace la parada y vuelve al LW
		--WAIT FOR CLK_period * 7;
		--IRQ <= '1';
		--WAIT FOR CLK_period;
		--IRQ <= '0';
--
		---- Genera una interrupción en el segundo ciclo de parada del WRO
		---- Vuelve a WRO porque por la 1a parada no hay instrucción en EX
		--WAIT FOR CLK_period * 57;
		--IRQ <= '1';
		--WAIT FOR CLK_period;
		--IRQ <= '0';
--
		---- Genera una interrupción en el primer ciclo de parada del ADD
		---- Vuelve al LW porque no se hace la parada
		--WAIT FOR CLK_period * 53;
		--IRQ <= '1';
		--WAIT FOR CLK_period;
		--IRQ <= '0';
--
		---- Genera una interrupción en el segundo SW
		---- Vuelve al LW porque el primer SW no pasa a EX
		--WAIT FOR CLK_period * 54;
		--IRQ <= '1';
		--WAIT FOR CLK_period;
		--IRQ <= '0';
--
		---- Interrumpe constantemente durante un tiempo.
		---- Se realiza tras un salto, por lo que vuelve al IF (1er BEQ)
		---- Tras las RTE también se vuelve desde la instr. en IF
		--WAIT FOR CLK_period * 56;
		--IRQ <= '1'; -- Ignora las IRQ mientras está tratando una
		--WAIT FOR CLK_period * 120;
		--IRQ <= '0';

		-- FIN SECUENCIAS INTERRUPCIONES

		WAIT;

	END PROCESS;

END;