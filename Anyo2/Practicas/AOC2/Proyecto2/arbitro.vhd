----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date:    15:02:29 04/04/2014 
-- Design Name: 
-- Module Name:    reg32 - Behavioral 
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

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx primitives in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

ENTITY Arbitro IS
   PORT (
      clk : IN STD_LOGIC;
      reset : IN STD_LOGIC;
      bus_frame : IN STD_LOGIC;-- para saber que hay una transferenica en curso
      last_word : IN STD_LOGIC; -- Cuando termina una transferencia cambiamos las prioridades
      Bus_TRDY : IN STD_LOGIC; --para saber que la ultima transferencia va a realizarse este ciclo
      Req0 : IN STD_LOGIC; -- hay dos solicitudes
      Req1 : IN STD_LOGIC;
      Grant0 : OUT STD_LOGIC;
      Grant1 : OUT STD_LOGIC);
END Arbitro;

ARCHITECTURE Behavioral OF Arbitro IS
   SIGNAL priority : STD_LOGIC;
BEGIN
   -- La prioridad de concesi�n es round robin. Es decir se alterna la prioridad entre los dos dispositivos
   SYNC_PROC : PROCESS (clk)
   BEGIN
      IF (clk'event AND clk = '1') THEN
         IF (reset = '1') THEN
            priority <= '0';
         ELSIF (bus_frame = '1') AND (last_word = '1') AND (Bus_TRDY = '1') THEN -- Cuando sabemos que la transferencia est� en su ultimo ciclo se cambian las prioridades
            priority <= NOT priority;
         END IF;
      END IF;
   END PROCESS;
   -- La se�al Frame inhibe al �rbitro
   grant1 <= (NOT(bus_frame) AND ((Req1 AND priority) OR (Req1 AND NOT(Req0)))); --si req 1 est� activado y prioridad vale 1	se concede el bus al dispositivo 1
   grant0 <= (NOT(bus_frame) AND ((Req0 AND NOT(priority)) OR (Req0 AND NOT(Req1)))); --si req0 est� activado y prioridad vale 0	y se concede el bus al dispositivo 0
END Behavioral;