----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    11:03:23 04/03/2014 
-- Design Name: 
-- Module Name:    BR - Behavioral 
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
----------------------------------------------------------------------------------
LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE ieee.numeric_std.ALL;
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
-- use IEEE.NUMERIC_STD.ALL;
-- librer�a para usar la funci�n conv_integer
USE IEEE.std_logic_unsigned.ALL;
-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

ENTITY FIFO_reg IS
	PORT (
		clk : IN STD_LOGIC;
		reset : IN STD_LOGIC;
		cjto : IN STD_LOGIC_VECTOR (1 DOWNTO 0); --Dir del cjto reemplazado
		new_block : IN STD_LOGIC; -- Indica que hay un reemplazo y por tanto hay que actualizar la info del fifo del conjunto correspondiente				        
		via_2_rpl : OUT STD_LOGIC
	);
END FIFO_reg;

ARCHITECTURE Behavioral OF FIFO_reg IS
	-- en el banco hay 4 registros de un bit
	TYPE reg_array IS ARRAY (0 TO 3) OF STD_LOGIC;
	SIGNAL reg_file : reg_array;

BEGIN
	PROCESS (clk)
	BEGIN
		IF (clk'event AND clk = '1') THEN -- se escribe en flanco de subida. 
			--if RegWrite is 1, write BusW data in register RW
			IF reset = '1' THEN
				FOR i IN 0 TO 3 LOOP
					reg_file(i) <= '0';
				END LOOP;
			ELSE
				IF (new_block = '1') THEN
					-- si hay un reemplazo negamos el bit que indica el cjto m�s antiguo (ya que habr� sido reemplazado)
					reg_file(conv_integer(cjto)) <= NOT(reg_file(conv_integer(cjto))); --forma super compacta de vhdl para hacer el decodificador y la escritura en el banco de registros
				END IF;
			END IF;
		END IF;
	END PROCESS;

	--get FIFO data for the current set (conjunto)
	via_2_rpl <= reg_file(conv_integer(cjto)); -- esto es una forma muy r�pida de hacer un Mux en vhdl

END Behavioral;