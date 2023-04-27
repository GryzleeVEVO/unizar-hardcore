LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE ieee.std_logic_unsigned.ALL;
USE ieee.numeric_std.ALL; -- se usa para convertir std_logic a enteros
ENTITY Via IS
	GENERIC (num_via : INTEGER); -- se usa para los mensajes. Hay que poner el n�mero correcto al instanciarla
	PORT (
		CLK : IN STD_LOGIC;
		reset : IN STD_LOGIC;
		Dir_word : IN STD_LOGIC_VECTOR(1 DOWNTO 0); -- se usa para elegir la palabra a la que se accede en un conjunto la cache de datos. 
		Dir_cjto : IN STD_LOGIC_VECTOR(1 DOWNTO 0); -- se usa para elegir el conjunto
		Tag : IN STD_LOGIC_VECTOR(25 DOWNTO 0);
		Din : IN STD_LOGIC_VECTOR (31 DOWNTO 0);
		RE : IN STD_LOGIC; -- read enable		
		WE : IN STD_LOGIC; -- write enable	
		Tags_WE : IN STD_LOGIC; -- write enable para la memoria de etiquetas 
		hit : OUT STD_LOGIC; -- indica si es acierto
		Dout : OUT STD_LOGIC_VECTOR (31 DOWNTO 0));
END Via;

ARCHITECTURE Behavioral OF Via IS

	COMPONENT reg IS
		GENERIC (size : NATURAL := 32); -- por defecto son de 32 bits, pero se puede usar cualquier tama�o
		PORT (
			Din : IN STD_LOGIC_VECTOR (size - 1 DOWNTO 0);
			clk : IN STD_LOGIC;
			reset : IN STD_LOGIC;
			load : IN STD_LOGIC;
			Dout : OUT STD_LOGIC_VECTOR (size - 1 DOWNTO 0));
	END COMPONENT;

	-- definimos la memoria de contenidos de la cache de datos como un array de 16 palabras de 32 bits
	TYPE Ram_MC_data IS ARRAY(0 TO 15) OF STD_LOGIC_VECTOR(31 DOWNTO 0);
	SIGNAL MC_data : Ram_MC_data := (X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", -- posiciones 0,1,2,3,4,5,6,7
	X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000", X"00000000");
	-- definimos la memoria de etiquetas de la cache de datos como un array de 4 palabras de 26 bits
	TYPE Ram_MC_Tags IS ARRAY(0 TO 3) OF STD_LOGIC_VECTOR(25 DOWNTO 0);
	SIGNAL MC_Tags : Ram_MC_Tags := ("00000000000000000000000000", "00000000000000000000000000", "00000000000000000000000000", "00000000000000000000000000");
	SIGNAL valid_bits_in, valid_bits_out, mask : STD_LOGIC_VECTOR(3 DOWNTO 0); -- se usa para saber si un bloque tiene info v�lida. Cada bit representa un bloque.									
	SIGNAL valid_bit : STD_LOGIC;
	SIGNAL Dir_MC : STD_LOGIC_VECTOR(3 DOWNTO 0); -- se usa para leer/escribir las datos almacenas en al MC. 
	SIGNAL MC_Tags_Dout : STD_LOGIC_VECTOR(25 DOWNTO 0);

BEGIN
	-------------------------------------------------------------------------------------------------- 
	-----memoria_cache_D: memoria RAM que almacena los 4 bloques de 4 datos que puede guardar la Cache
	-------------------------------------------------------------------------------------------------- 
	Dir_MC <= Dir_cjto & Dir_word;
	memoria_cache_D : PROCESS (CLK)
	BEGIN
		IF (CLK'event AND CLK = '1') THEN
			IF (WE = '1') THEN -- s�lo se escribe si WE vale 1
				MC_data(conv_integer(Dir_MC)) <= Din;
				-- report saca un mensaje en la consola del simulador.  Nos imforma sobre qu� dato se ha escrito, d�nde y cu�ndo
				REPORT "Simulation time : " & TIME'IMAGE(now) & ".  Data written in via " & INTEGER'image(num_via) & ": " & INTEGER'image(to_integer(unsigned(Din))) & ", in Dir_cjto = " & INTEGER'image(to_integer(unsigned(Dir_cjto)));
			END IF;
		END IF;
	END PROCESS;
	Dout <= MC_data(conv_integer(Dir_MC)) WHEN (RE = '1') ELSE
		"00000000000000000000000000000000"; --s�lo se lee si RE_MC vale 1
	-------------------------------------------------------------------------------------------------- 
	-----MC_Tags: memoria RAM que almacena las 4 etiquetas
	-------------------------------------------------------------------------------------------------- 
	memoria_cache_tags : PROCESS (CLK)
	BEGIN
		IF (CLK'event AND CLK = '1') THEN
			IF (Tags_WE = '1') THEN -- s�lo se escribe si Tags_WE vale 1
				MC_Tags(conv_integer(dir_cjto)) <= Tag;
				-- report saca un mensaje en la consola del simulador. Nos imforma sobre qu� etiqueta se ha escrito, d�nde y cu�ndo
				REPORT "Simulation time : " & TIME'IMAGE(now) & ".  Tag written in via " & INTEGER'image(num_via) & ": " & INTEGER'image(to_integer(unsigned(Tag))) & ", in Dir_cjto = " & INTEGER'image(to_integer(unsigned(dir_cjto)));
			END IF;
		END IF;
	END PROCESS;
	MC_Tags_Dout <= MC_Tags(conv_integer(dir_cjto)); --s�lo se lee si RE_MC vale 1
	-------------------------------------------------------------------------------------------------- 
	-- registro de validez. Al resetear los bits de validez se ponen a 0 as� evitamos falsos positivos por basura en las memorias
	-- en el bit de validez se escribe a la vez que en la memoria de etiquetas. Hay que poner a 1 el bit que toque y mantener los dem�s, para eso usamos una mascara generada por un decodificador
	-------------------------------------------------------------------------------------------------- 
	mask <= "0001" WHEN dir_cjto = "00" ELSE
		"0010" WHEN dir_cjto = "01" ELSE
		"0100" WHEN dir_cjto = "10" ELSE
		"1000" WHEN dir_cjto = "11" ELSE
		"0000";
	valid_bits_in <= valid_bits_out OR mask;
	bits_validez : reg GENERIC MAP(size => 4) PORT MAP(Din => valid_bits_in, clk => clk, reset => reset, load => Tags_WE, Dout => valid_bits_out);
	-------------------------------------------------------------------------------------------------- 
	valid_bit <= valid_bits_out(0) WHEN dir_cjto = "00" ELSE
		valid_bits_out(1) WHEN dir_cjto = "01" ELSE
		valid_bits_out(2) WHEN dir_cjto = "10" ELSE
		valid_bits_out(3) WHEN dir_cjto = "11" ELSE
		'0';
	-------------------------------------------------------------------------------------------------- 
	-- Se�al de hit: se activa cuando la etiqueta coincide y el bit de valido es 1
	hit <= '1' WHEN ((MC_Tags_Dout = Tag) AND (valid_bit = '1'))ELSE
		'0'; --comparador que compara el tag almacenado en MC con el de la direcci�n y si es el mismo y el bloque tiene el bit de v�lido activo devuelve un 1
	-------------------------------------------------------------------------------------------------- 

END Behavioral;