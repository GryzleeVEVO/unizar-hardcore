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

ENTITY BReg IS
    PORT (
        clk : IN STD_LOGIC;
        reset : IN STD_LOGIC;
        RA : IN STD_LOGIC_VECTOR (4 DOWNTO 0); --Dir para el puerto de lectura A
        RB : IN STD_LOGIC_VECTOR (4 DOWNTO 0); --Dir para el puerto de lectura A
        RW : IN STD_LOGIC_VECTOR (4 DOWNTO 0); --Dir para el puerto de escritura
        BusW : IN STD_LOGIC_VECTOR (31 DOWNTO 0);--entrada de datos para el puerto de escritura
        RegWrite : IN STD_LOGIC;
        BusA : OUT STD_LOGIC_VECTOR (31 DOWNTO 0);
        BusB : OUT STD_LOGIC_VECTOR (31 DOWNTO 0)
    );
END BReg;

ARCHITECTURE Behavioral OF BReg IS
    -- el banco de registros es un array de 32 registros de 32 bits
    TYPE reg_array IS ARRAY (0 TO 31) OF STD_LOGIC_VECTOR(31 DOWNTO 0);
    SIGNAL reg_file : reg_array;

BEGIN
    PROCESS (clk)
    BEGIN
        IF (clk'event AND clk = '0') THEN -- se escribe en flanco de bajada. clk'event vale uno cuando hay un flanco. Si ha habido un flaco y el resultado es que clk vale cero era un flanco de bajada
            --if RegWrite is 1, write BusW data in register RW
            IF reset = '1' THEN
                FOR i IN 0 TO 31 LOOP
                    reg_file(i) <= "00000000000000000000000000000000";
                END LOOP;
            ELSE
                IF RegWrite = '1' THEN
                    reg_file(conv_integer(RW)) <= BusW; --forma super compacta de vhdl para hacer el decodificador y la escritura en el banco de registros
                    -- la siguiente l�nea saca un aviso por pantalla con el dato que se ha escrito
                    REPORT "Simulation time : " & TIME'IMAGE(now) & ".  Data written: " & INTEGER'image(to_integer(signed(BusW))) & ", in Reg = " & INTEGER'image(to_integer(unsigned(RW)));
                END IF;
            END IF;
        END IF;
    END PROCESS;

    --get data stored at register RA
    BusA <= reg_file(conv_integer(RA)); -- esto es una forma muy r�pida de hacer un Mux en vhdl
    --get data stored at register RA
    BusB <= reg_file(conv_integer(RB));
END Behavioral;