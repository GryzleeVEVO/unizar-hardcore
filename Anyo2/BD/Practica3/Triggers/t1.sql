-- Trigger 1: No pueden existir desvios de vuelos cancelados
--            y el destino del desvio no puede ser el mismo que el destino del vuelo.
CREATE OR REPLACE TRIGGER trigger1
BEFORE INSERT ON Desvio
FOR EACH ROW
DECLARE
  n NUMBER;
  m NUMBER;
BEGIN
  -- No pueden existir desvios de vuelos cancelados.
  SELECT Cancelado
  INTO m
  FROM Vuelo
  WHERE idVuelo=:NEW.clvVuelo;
  IF m=1 THEN RAISE_APPLICATION_ERROR(-20002, 'No pueden existir desvios de vuelos cancelados.');
  END IF;  
  -- El destino del desvio no puede ser el mismo que el destino del vuelo.
  SELECT COUNT(*)
  INTO n
  FROM Vuelo
  WHERE idVuelo=:NEW.clvVuelo AND clvDestino=:NEW.clvAeropuerto;
  IF n=1 THEN RAISE_APPLICATION_ERROR(-20001, 'El destino del desvio no puede ser el mismo que el destino del vuelo.');
  END IF;
END trigger1;
/