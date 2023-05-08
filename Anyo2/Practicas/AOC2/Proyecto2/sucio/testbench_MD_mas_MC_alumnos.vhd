-- TestBench Template 

LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
USE ieee.numeric_std.ALL;
USE IEEE.std_logic_arith.ALL;
USE IEEE.std_logic_unsigned.ALL;
ENTITY testbench_MD_mas_MC IS
END testbench_MD_mas_MC;

ARCHITECTURE behavior OF testbench_MD_mas_MC IS

	-- Component Declaration
	COMPONENT MD_mas_MC IS PORT (
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

	SIGNAL clk, reset, RE, WE, Mem_ready, Data_abort : STD_LOGIC;
	SIGNAL ADDR, Din, Dout, IO_input : STD_LOGIC_VECTOR (31 DOWNTO 0);
	SIGNAL test_id : STD_LOGIC_VECTOR (7 DOWNTO 0); --Para saber por qu� prueba vamos
	-- Clock period definitions
	CONSTANT CLK_period : TIME := 10 ns;
BEGIN

	-- Component Instantiation
	uut : MD_mas_MC PORT MAP(
		clk => clk,
		reset => reset,
		ADDR => ADDR,
		Din => Din,
		RE => RE,
		WE => WE,
		IO_input => IO_input,
		Mem_ready => Mem_ready,
		Data_abort => Data_abort,
		Dout => Dout
	);

	-- Clock process definitions
	CLK_process : PROCESS
	BEGIN
		CLK <= '0';
		WAIT FOR CLK_period/2;
		CLK <= '1';
		WAIT FOR CLK_period/2;
	END PROCESS;

	stim_proc : PROCESS
		VARIABLE done : INTEGER := 0;
	BEGIN

		reset <= '1';

		addr <= conv_std_logic_vector(0, 32);
		Din <= conv_std_logic_vector(0, 32);

		-- IO_input = 1024
		IO_input <= conv_std_logic_vector(1024, 32);

		RE <= '0';
		WE <= '0';

		WAIT FOR 20 ns;

		--Test 0---------------------------------------------------------------------------------------------------------
		-- FALLO LECTURA en 0x40
		-- (0,0) <- MD 0x4x
		-- MIPS <- (0,0) 0 = 1

		test_id <= conv_std_logic_vector(0, 8);
		reset <= '0';
		RE <= '1';

		Addr <= conv_std_logic_vector(64, 32);

		done := 0;
		WAIT FOR 1 ns;
		WHILE done < 1 LOOP
			IF Mem_ready = '0' THEN
				WAIT UNTIL Mem_ready = '1';
				WAIT FOR 1 ns;
				IF Mem_ready = '1' THEN
					done := 1;
				END IF;
			ELSE
				WAIT FOR 1 ns;
				IF Mem_ready = '1' THEN
					done := 1;
				END IF;
			END IF;
		END LOOP;
		WAIT UNTIL clk'event;
		WAIT UNTIL clk'event;

		--Test 1---------------------------------------------------------------------------------------------------------
		-- ACIERTO LECTURA EN 0x44
		-- MIPS <- (0,0) 1 = 2

		test_id <= conv_std_logic_vector(1, 8);
		Addr <= conv_std_logic_vector(68, 32);

		done := 0;
		WAIT FOR 1 ns;
		WHILE done < 1 LOOP
			IF Mem_ready = '0' THEN
				WAIT UNTIL Mem_ready = '1';
				WAIT FOR 1 ns;
				IF Mem_ready = '1' THEN
					done := 1;
				END IF;
			ELSE
				WAIT FOR 1 ns;
				IF Mem_ready = '1' THEN
					done := 1;
				END IF;
			END IF;
		END LOOP;
		WAIT UNTIL clk'event;
		WAIT UNTIL clk'event;

		--Test 2---------------------------------------------------------------------------------------------------------
		-- ACIERTO DE ESCRITURA en 0x48
		-- (0,0) 3 <- FF
		-- 0x48 <- FF

		test_id <= conv_std_logic_vector(2, 8);

		-- IO_input = 2048
		IO_input <= conv_std_logic_vector(2048, 32);

		Addr <= conv_std_logic_vector(72, 32);
		Din <= conv_std_logic_vector(255, 32);
		RE <= '0';
		WE <= '1';

		done := 0;
		WAIT FOR 1 ns;
		WHILE done < 1 LOOP
			IF Mem_ready = '0' THEN
				WAIT UNTIL Mem_ready = '1';
				WAIT FOR 1 ns;
				IF Mem_ready = '1' THEN
					done := 1;
				END IF;
			ELSE
				WAIT FOR 1 ns;
				IF Mem_ready = '1' THEN
					done := 1;
				END IF;
			END IF;
		END LOOP;
		WAIT UNTIL clk'event;
		WAIT UNTIL clk'event;

		--Test 3---------------------------------------------------------------------------------------------------------
		-- FALLO DE ESCRITURA en 0x60
		-- (2, 0) <- MD 0x6x
		-- (2, 0) 0 <- FF
		-- 0x60 <- FF

		test_id <= conv_std_logic_vector(3, 8);
		Addr <= conv_std_logic_vector(96, 32);
		Din <= conv_std_logic_vector(255, 32);
		RE <= '0';
		WE <= '1';

		done := 0;
		WAIT FOR 1 ns;
		WHILE done < 1 LOOP
			IF Mem_ready = '0' THEN
				WAIT UNTIL Mem_ready = '1';
				WAIT FOR 1 ns;
				IF Mem_ready = '1' THEN
					done := 1;
				END IF;
			ELSE
				WAIT FOR 1 ns;
				IF Mem_ready = '1' THEN
					done := 1;
				END IF;
			END IF;
		END LOOP;
		WAIT UNTIL clk'event;
		WAIT UNTIL clk'event;

		--Test 4---------------------------------------------------------------------------------------------------------
		-- ACIERTO DE ESCRITURA en 0x60
		-- (2,0) 0 <- FE
		-- 0x60 <- FE

		test_id <= conv_std_logic_vector(4, 8);
		Din <= conv_std_logic_vector(254, 32);
		Addr <= conv_std_logic_vector(96, 32);
		RE <= '0';
		WE <= '1';

		done := 0;
		WAIT FOR 1 ns;
		WHILE done < 1 LOOP
			IF Mem_ready = '0' THEN
				WAIT UNTIL Mem_ready = '1';
				WAIT FOR 1 ns;
				IF Mem_ready = '1' THEN
					done := 1;
				END IF;
			ELSE
				WAIT FOR 1 ns;
				IF Mem_ready = '1' THEN
					done := 1;
				END IF;
			END IF;
		END LOOP;
		WAIT UNTIL clk'event;
		WAIT UNTIL clk'event;

		--Test 5---------------------------------------------------------------------------------------------------------
		-- FALLO DE LECTURA en 0x80
		-- (0, 1) <- MD 0x8x
		-- MIPS <- (0, 1) 0 = 9

		--Debe ser un fallo de lectura y almacenarse 9, 10, 11 y 12 en el cjto 0 en la via 1
		test_id <= conv_std_logic_vector(5, 8); --Test 5

		-- IO_input = 4096 
		IO_input <= conv_std_logic_vector(4096, 32);

		Addr <= conv_std_logic_vector(128, 32);
		RE <= '1';
		WE <= '0';

		done := 0;
		WAIT FOR 1 ns;
		WHILE done < 1 LOOP
			IF Mem_ready = '0' THEN
				WAIT UNTIL Mem_ready = '1';
				WAIT FOR 1 ns;
				IF Mem_ready = '1' THEN
					done := 1;
				END IF;
			ELSE
				WAIT FOR 1 ns;
				IF Mem_ready = '1' THEN
					done := 1;
				END IF;
			END IF;
		END LOOP;
		WAIT UNTIL clk'event;
		WAIT UNTIL clk'event;

		--Test 6---------------------------------------------------------------------------------------------------------
		-- ACIERTO DE LECTURA EN 0x40
		-- MIPS <- (0,0) 0 = 1

		test_id <= conv_std_logic_vector(6, 8);
		Addr <= conv_std_logic_vector(64, 32);
		RE <= '1';
		WE <= '0';

		done := 0;
		WAIT FOR 1 ns;
		WHILE done < 1 LOOP
			IF Mem_ready = '0' THEN
				WAIT UNTIL Mem_ready = '1';
				WAIT FOR 1 ns;
				IF Mem_ready = '1' THEN
					done := 1;
				END IF;
			ELSE
				WAIT FOR 1 ns;
				IF Mem_ready = '1' THEN
					done := 1;
				END IF;
			END IF;
		END LOOP;
		WAIT UNTIL clk'event;
		WAIT UNTIL clk'event;

		--Test 7---------------------------------------------------------------------------------------------------------
		-- FALLO DE LECTURA en 0x100
		-- (0, 0) <- MD 0x10x (reemplazo)
		-- MIPS <- (0,0) 0 = c

		test_id <= conv_std_logic_vector(7, 8);
		Addr <= conv_std_logic_vector(256, 32);
		RE <= '1';
		WE <= '0';

		done := 0;
		WAIT FOR 1 ns;
		WHILE done < 1 LOOP
			IF Mem_ready = '0' THEN
				WAIT UNTIL Mem_ready = '1';
				WAIT FOR 1 ns;
				IF Mem_ready = '1' THEN
					done := 1;
				END IF;
			ELSE
				WAIT FOR 1 ns;
				IF Mem_ready = '1' THEN
					done := 1;
				END IF;
			END IF;
		END LOOP;
		WAIT UNTIL clk'event;
		WAIT UNTIL clk'event;

		--Test 8---------------------------------------------------------------------------------------------------------
		-- FALLO DE LECTURA en 0xc0
		-- (0, 1) <- MD 0xcx
		-- MIPS <- (0, 1) 0 = c

		test_id <= conv_std_logic_vector(8, 8);
		Addr <= conv_std_logic_vector(192, 32);
		RE <= '1';
		WE <= '0';

		done := 0;
		WAIT FOR 1 ns;
		WHILE done < 1 LOOP
			IF Mem_ready = '0' THEN
				WAIT UNTIL Mem_ready = '1';
				WAIT FOR 1 ns;
				IF Mem_ready = '1' THEN
					done := 1;
				END IF;
			ELSE
				WAIT FOR 1 ns;
				IF Mem_ready = '1' THEN
					done := 1;
				END IF;
			END IF;
		END LOOP;
		WAIT UNTIL clk'event;
		WAIT UNTIL clk'event;

		--Test 9---------------------------------------------------------------------------------------------------------
		-- ESCRITURA EN SCRATCH 0x10000004
		-- Scratch 0x1 <- FE

		test_id <= conv_std_logic_vector(9, 8);
		Din <= conv_std_logic_vector(254, 32);
		Addr <= x"10000004";
		WE <= '1';
		RE <= '0';

		done := 0;
		WAIT FOR 1 ns;
		WHILE done < 1 LOOP
			IF Mem_ready = '0' THEN
				WAIT UNTIL Mem_ready = '1';
				WAIT FOR 1 ns;
				IF Mem_ready = '1' THEN
					done := 1;
				END IF;
			ELSE
				WAIT FOR 1 ns;
				IF Mem_ready = '1' THEN
					done := 1;
				END IF;
			END IF;
		END LOOP;
		WAIT UNTIL clk'event;
		WAIT UNTIL clk'event;

		--Test 10---------------------------------------------------------------------------------------------------------
		-- LECTURA DE SCRATCH 0x10000004
		-- MIPS <- 0x1000004 = FE
		test_id <= conv_std_logic_vector(10, 8); --Test 10
		Addr <= x"10000004";
		RE <= '1';
		WE <= '0';

		done := 0;
		WAIT FOR 1 ns;
		WHILE done < 1 LOOP
			IF Mem_ready = '0' THEN
				WAIT UNTIL Mem_ready = '1';
				WAIT FOR 1 ns;
				IF Mem_ready = '1' THEN
					done := 1;
				END IF;
			ELSE
				WAIT FOR 1 ns;
				IF Mem_ready = '1' THEN
					done := 1;
				END IF;
			END IF;
		END LOOP;
		WAIT UNTIL clk'event;
		WAIT UNTIL clk'event;

		--Test 11---------------------------------------------------------------------------------------------------------
		-- LECTURA DE SCRATCH 0x10000000
		-- MIPS <- 0x10000000 = 4096  

		--Leemos el valor que ha escrito Master_IO. El �ltimo es 4096
		test_id <= conv_std_logic_vector(11, 8);
		Addr <= x"10000000";
		RE <= '1';
		WE <= '0';

		done := 0;
		WAIT FOR 1 ns;
		WHILE done < 1 LOOP
			IF Mem_ready = '0' THEN
				WAIT UNTIL Mem_ready = '1';
				WAIT FOR 1 ns;
				IF Mem_ready = '1' THEN
					done := 1;
				END IF;
			ELSE
				WAIT FOR 1 ns;
				IF Mem_ready = '1' THEN
					done := 1;
				END IF;
			END IF;
		END LOOP;
		WAIT UNTIL clk'event;
		WAIT UNTIL clk'event;

		--Test 12---------------------------------------------------------------------------------------------------------
		-- DIRECCIÓN NO EXISTE, data abort
		-- internal (0x1000000) <- 0x01110000

		test_id <= conv_std_logic_vector(12, 8);
		Addr <= x"01110000";
		RE <= '1';
		WE <= '0';

		done := 0;
		WAIT FOR 1 ns;
		WHILE done < 1 LOOP
			IF Mem_ready = '0' THEN
				WAIT UNTIL Mem_ready = '1';
				WAIT FOR 1 ns;
				IF Mem_ready = '1' THEN
					done := 1;
				END IF;
			ELSE
				WAIT FOR 1 ns;
				IF Mem_ready = '1' THEN
					done := 1;
				END IF;
			END IF;
		END LOOP;
		WAIT UNTIL clk'event;
		WAIT UNTIL clk'event;

		--Test 13---------------------------------------------------------------------------------------------------------
		-- MIPS solicita @ error del abort
		-- MIPS <- internal (0x01000000) = 0x01110000

		test_id <= conv_std_logic_vector(13, 8);
		Addr <= x"01000000";
		RE <= '1';
		WE <= '0';

		done := 0;
		WAIT FOR 1 ns;
		WHILE done < 1 LOOP
			IF Mem_ready = '0' THEN
				WAIT UNTIL Mem_ready = '1';
				WAIT FOR 1 ns;
				IF Mem_ready = '1' THEN
					done := 1;
				END IF;
			ELSE
				WAIT FOR 1 ns;
				IF Mem_ready = '1' THEN
					done := 1;
				END IF;
			END IF;
		END LOOP;
		WAIT UNTIL clk'event;
		WAIT UNTIL clk'event;

		--Test 255---------------------------------------------------------------------------------------------------------
		-- FIN
		-- LECTURA DE SCRATCH 0x10000000
		-- MIPS <- 0x10000000 = 4096

		test_id <= conv_std_logic_vector(255, 8);
		Addr <= x"10000000";
		RE <= '1';
		WE <= '0';
		WAIT;
	END PROCESS;
END;