-- Trigger 2: Un vuelo no puede ocurrir en un avion que todavia no se ha creado.
CREATE OR REPLACE TRIGGER trigger2
BEFORE INSERT ON Vuelo
FOR EACH ROW
DECLARE
  n NUMBER;
BEGIN
  SELECT COUNT(*) 
  INTO n
  FROM ( 
    SELECT Montaje, SUBSTR(:NEW.Fecha,1,4) Anyo 
    FROM Avion
    WHERE Matricula=:NEW.clvAvion )
  WHERE Montaje>Anyo;
  IF n=1 THEN RAISE_APPLICATION_ERROR(-20004, 'Un vuelo no puede ocurrir en un avion que todavia no se ha creado.');
  END IF;
END trigger2;
/