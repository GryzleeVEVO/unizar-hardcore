----------------------------------------------------------------------------------
-- Description: Banco de registros que separa las etapas IF e ID. Almacena la instrucci�n en IR_ID y el PC+4 en PC4_ID
----------------------------------------------------------------------------------
LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
ENTITY Banco_ID IS
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
END Banco_ID;

ARCHITECTURE Behavioral OF Banco_ID IS

BEGIN
   SYNC_PROC : PROCESS (clk)
   BEGIN
      IF (clk'event AND clk = '1') THEN
         IF (reset = '1') THEN
            IR_ID <= x"00000000";
            PC4_ID <= x"00000000";
            --nuevo
            PC_exception_ID <= x"00000000";
            valid_I_ID <= '0';
         ELSE
            IF (load = '1') THEN
               IR_ID <= IR_in;
               PC4_ID <= PC4_in;
               valid_I_ID <= valid_I_IF;
               --nuevo excepciones
               PC_exception_ID <= PC_exception;
            END IF;
         END IF;
      END IF;
   END PROCESS;

END Behavioral;