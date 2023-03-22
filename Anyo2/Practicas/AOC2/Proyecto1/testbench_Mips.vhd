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

		--IRQ <= '1';
		--WAIT FOR CLK_period;
		--IRQ <= '0';
		WAIT FOR CLK_period * 10;
		IRQ <= '1';
		WAIT FOR CLK_period;
		IRQ <= '0';
		WAIT FOR CLK_period * 41;
		IRQ <= '1';
		WAIT FOR CLK_period;
		IRQ <= '0';
		WAIT FOR CLK_period * 20;
		IRQ <= '1';

		WAIT;

	END PROCESS;

END;