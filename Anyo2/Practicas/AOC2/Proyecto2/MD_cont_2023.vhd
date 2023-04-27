----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    14:12:11 04/04/2014 
-- Design Name: 
-- Module Name:    DMA - Behavioral 
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
LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.std_logic_arith.ALL;
USE IEEE.std_logic_unsigned.ALL;
ENTITY MD_cont IS PORT (
	CLK : IN STD_LOGIC;
	reset : IN STD_LOGIC;
	Bus_Frame : IN STD_LOGIC; -- indica que el master quiere m�s datos
	bus_last_word : IN STD_LOGIC; --indica que es el �ltimo dato de la transferencia
	bus_Read : IN STD_LOGIC;
	bus_Write : IN STD_LOGIC;
	Bus_Addr : IN STD_LOGIC_VECTOR (31 DOWNTO 0); --Direcciones 
	Bus_Data : IN STD_LOGIC_VECTOR (31 DOWNTO 0); --Datos  
	MD_Bus_DEVsel : OUT STD_LOGIC; -- para avisar de que se ha reconocido que la direcci�n pertenece a este m�dulo
	MD_Bus_TRDY : OUT STD_LOGIC; -- para avisar de que se va a realizar la operaci�n solicitada en el ciclo actual
	MD_send_data : OUT STD_LOGIC; -- para enviar los datos al bus
	MD_Dout : OUT STD_LOGIC_VECTOR (31 DOWNTO 0) -- salida de datos
);
END MD_cont;

ARCHITECTURE Behavioral OF MD_cont IS

	COMPONENT counter IS
		GENERIC (size : INTEGER := 10);
		PORT (
			clk : IN STD_LOGIC;
			reset : IN STD_LOGIC;
			count_enable : IN STD_LOGIC;
			count : OUT STD_LOGIC_VECTOR (size - 1 DOWNTO 0));
	END COMPONENT;

	-- misma memoria que en el proyecto anterior
	COMPONENT RAM_128_32 IS PORT (
		CLK : IN STD_LOGIC;
		enable : IN STD_LOGIC; --solo se lee o escribe si enable est� activado
		ADDR : IN STD_LOGIC_VECTOR (31 DOWNTO 0); --Dir 
		Din : IN STD_LOGIC_VECTOR (31 DOWNTO 0);--entrada de datos para el puerto de escritura
		WE : IN STD_LOGIC; -- write enable	
		RE : IN STD_LOGIC; -- read enable		  
		Dout : OUT STD_LOGIC_VECTOR (31 DOWNTO 0));
	END COMPONENT;

	COMPONENT reg IS
		GENERIC (size : NATURAL := 32); -- por defecto son de 32 bits, pero se puede usar cualquier tama�o
		PORT (
			Din : IN STD_LOGIC_VECTOR (size - 1 DOWNTO 0);
			clk : IN STD_LOGIC;
			reset : IN STD_LOGIC;
			load : IN STD_LOGIC;
			Dout : OUT STD_LOGIC_VECTOR (size - 1 DOWNTO 0));
	END COMPONENT;
	SIGNAL BUS_RE, BUS_WE, MEM_WE, contar_palabras, reset_cuenta, reset_cont_retardos, MD_enable, memoria_preparada, contar_retardos, direccion_distinta, reset_retardo, load_addr, Addr_in_range : STD_LOGIC;
	SIGNAL addr_frame, last_addr : STD_LOGIC_VECTOR (6 DOWNTO 0);
	SIGNAL cuenta_palabras, cuenta_retardos : STD_LOGIC_VECTOR (7 DOWNTO 0);
	SIGNAL MD_addr : STD_LOGIC_VECTOR (31 DOWNTO 0);
	TYPE state_type IS (Espera, Transferencia, Detectado);
	SIGNAL state, next_state : state_type;
	SIGNAL last_addr_valid_vector : STD_LOGIC_VECTOR (0 DOWNTO 0);--indica si el registo last_addr tiene una direcci�n v�lida y no un 0 proveniente de un reset
	SIGNAL load_control, Internal_read, Internal_write, last_addr_valid : STD_LOGIC; --signals to store inputs bus_read, and bus_write
BEGIN
	---------------------------------------------------------------------------
	-- Decodificador: identifica cuando la direcci�n pertenece a la MD: (X"00000000"-X"000001FF")
	---------------------------------------------------------------------------
	-- Se activa cuando el bus quiere realizar una operaci�n (bus_read o bus_write = '1') y la direcci�n est� en el rango
	Addr_in_range <= '1' WHEN (Bus_Addr(31 DOWNTO 9) = "00000000000000000000000") AND ((bus_Read = '1') OR (bus_Write = '1')) ELSE
		'0';

	---------------------------------------------------------------------------
	-- Registro que almacena las se�ales de control del bus
	---------------------------------------------------------------------------

	Read_Write_register : PROCESS (clk)
	BEGIN
		IF (clk'event AND clk = '1') THEN
			IF (reset = '1') THEN
				Internal_read <= '0';
				Internal_write <= '0';
			ELSIF load_control = '1' THEN
				Internal_read <= bus_Read;
				Internal_write <= bus_write;
			END IF;
		END IF;
	END PROCESS;

	BUS_RE <= Internal_read;

	BUS_WE <= Internal_write;

	---------------------------------------------------------------------------
	-- HW para introducir retardos:
	-- Con un contador y una sencilla m�quina de estados introducimos un retardo en la memoria de forma articial. 
	-- Cuando se pide una direcci�n nueva manda la primera palabra en 4 ciclos y el resto cada dos
	-- Si se accede dos veces a la misma direcci�n la segunda vez no hay retardo inicial
	---------------------------------------------------------------------------
	reset_cont_retardos <= reset OR reset_retardo;
	cont_retardos : counter GENERIC MAP(size => 8) PORT MAP(clk => clk, reset => reset_retardo, count_enable => contar_retardos, count => cuenta_retardos);

	-- este registro almacena la ultima direcci�n accedida. Cada vez que cambia la direcci�n se resetea el contador de retaros
	-- La idea es simular que cuando accedes a una direcci�n nueva tarda m�s. Si siempre accedes a la misma no introducir� retardos adicionales
	reg_last_addr : reg GENERIC MAP(size => 7)
	PORT MAP(Din => Bus_Addr(8 DOWNTO 2), CLK => CLK, reset => reset, load => load_addr, Dout => last_addr);
	reg_last_addr_valid : reg GENERIC MAP(size => 1)
	PORT MAP(Din => "1", CLK => CLK, reset => reset, load => load_addr, Dout => last_addr_valid_vector);
	last_addr_valid <= last_addr_valid_vector(0); -- conversi�n de tipos						
	direccion_distinta <= '0' WHEN ((last_addr = Bus_Addr(8 DOWNTO 2)) AND (last_addr_valid = '1')) ELSE
		'1';
	--introducimos un retardo en la memoria de forma articial. Manda la primera palabra en el cuarto ciclo y el resto cada dos ciclos
	-- Pero si los accesos son a direcciones repetidas el retardo inicial desaparece

	memoria_preparada <= '0' WHEN (cuenta_retardos < "00000011" OR cuenta_retardos(0) = '1') ELSE
		'1';
	---------------------------------------------------------------------------
	-- M�quina de estados para gestionar las transferencias e introducir retardos
	---------------------------------------------------------------------------

	SYNC_PROC : PROCESS (clk)
	BEGIN
		IF (clk'event AND clk = '1') THEN
			IF (reset = '1') THEN
				state <= Espera;
			ELSE
				state <= next_state;
			END IF;
		END IF;
	END PROCESS;

	--MEALY State-Machine - Outputs based on state and inputs
	OUTPUT_DECODE : PROCESS (state, direccion_distinta, Addr_in_range, memoria_preparada, Bus_Frame, reset_cuenta)
	BEGIN
		-- valores por defecto, si no se asigna otro valor en un estado valdr�n lo que se asigna aqu�
		contar_retardos <= '0';
		reset_retardo <= '0';
		load_addr <= '0';
		load_control <= '0';
		next_state <= Espera;
		MD_Bus_DEVsel <= '0';
		MD_Bus_TRDY <= '0';
		MD_send_data <= '0';
		MEM_WE <= '0';
		MD_enable <= '0';
		contar_palabras <= '0';
		CASE state IS
				-- Estado inicial: Espera 
			WHEN Espera =>
				IF (Addr_in_range = '0') THEN -- si no piden nada no hacemos nada
					next_state <= Espera;
				ELSE -- si detectamos que hay una transferencia y la direcci�n nos pertenece vamos al estado de transferencia
					next_state <= Detectado;
					MD_Bus_DEVsel <= '1'; -- avisamos de que hemos visto que la direcci�n es nuestra
					load_control <= '1'; -- Para cargar las se�ales de Bus_read y Bus_write
					IF (direccion_distinta = '1') THEN
						reset_retardo <= '1'; -- si se repite la direcci�n no metemos los retardos iniciales
						load_addr <= '1'; --cargamos  la direcci�n 
					END IF;
				END IF;
				-- Estado Detectado: sirve para informar de que hemos visto que la direcci�n es nuestra y de que vamos a empezar a leer/escribir datos 
			WHEN Detectado =>
				IF (Bus_Frame = '1') THEN
					next_state <= Transferencia;
					MD_Bus_DEVsel <= '1'; -- avisamos de que hemos visto que la direcci�n es nuestra
					-- No empezamos a leer/escribir por si acaso no mandan los datos hasta el ciclo siguiente
				ELSE --Cuando Bus_Frame es 0 es que hemos terminado. No deber�a pasar porque todav�a no hemos hecho nada
					next_state <= Espera;
				END IF;
				-- Estado Transferencia
			WHEN Transferencia =>
				IF (Bus_Frame = '1') THEN -- si estamos en una transferencia seguimos enviando/recibiendo datos hasta que el master diga que no quiere m�s
					MD_Bus_DEVsel <= '1'; -- avisamos de que hemos visto que la direcci�n es nuestra
					MD_enable <= '1'; --habilitamos la MD para leer o escribir
					contar_retardos <= '1';
					MD_Bus_TRDY <= memoria_preparada;
					contar_palabras <= memoria_preparada; -- cada vez que mandamos una palabra se incrementa el contador
					MEM_WE <= Bus_WE AND memoria_preparada; --evitamos escribir varias veces
					MD_send_data <= Bus_RE AND memoria_preparada; -- si la direcci�n est� en rango y es una lectura se carga el dato de MD en el bus
					IF ((bus_last_word = '1') AND (memoria_preparada = '1')) THEN --si estamos enviando la �ltima palabra hemos terminado
						next_state <= Espera;
						reset_retardo <= '1';
					ELSE
						next_state <= Transferencia;
					END IF;
				ELSE --no deber�a pasar. Si pasa quiere decir que han desactivado el frame sin poner last_word
					next_state <= Transferencia;
					next_state <= Espera;
				END IF;
		END CASE;
	END PROCESS;

	---------------------------------------------------------------------------
	-- calculo direcciones 
	-- el contador cuenta mientras frame est� activo, la direcci�n pertenezca a la memoria y la memoria est� preparada para realizar la operaci�n actual. 
	---------------------------------------------------------------------------

	-- Para que este esquema funcione hay que avisar cuando se pide la �ltima palabra. Al enviarla se resetea la cuenta de la r�faga, y as� la siguiente r�faga empezara por la direcci�n inicial
	reset_cuenta <= '1' WHEN ((reset = '1') OR ((bus_last_word = '1') AND (memoria_preparada = '1'))) ELSE
		'0';
	cont_palabras : counter GENERIC MAP(size => 8) PORT MAP(clk => clk, reset => reset_cuenta, count_enable => contar_palabras, count => cuenta_palabras);
	-- La direcci�n se calcula sumando la cuenta de palabras a la direcci�n inicial almacenada en el registro last_addr
	addr_Frame <= last_addr + cuenta_palabras(6 DOWNTO 0);
	-- s�lo asignamos los bits que se usan. El resto se quedan a 0.
	MD_addr(8 DOWNTO 2) <= addr_Frame;
	MD_addr(1 DOWNTO 0) <= "00";
	MD_addr(31 DOWNTO 9) <= "00000000000000000000000";

	---------------------------------------------------------------------------
	-- Memoria de datos original 
	---------------------------------------------------------------------------
	MD : RAM_128_32 PORT MAP(CLK => CLK, enable => MD_enable, ADDR => MD_addr, Din => Bus_Data, WE => MEM_WE, RE => Bus_RE, Dout => MD_Dout);
END Behavioral;