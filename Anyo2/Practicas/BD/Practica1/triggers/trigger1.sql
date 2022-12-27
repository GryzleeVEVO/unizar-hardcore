/*
    BASES DE DATOS
    Práctica 1: Liga de fútbol
    Fichero: trigger1.sql
    Autores:    Álvaro Seral Gracia                 816456  816456@unizar.es
                Cristian Andrei Selivanov Dobrisan  819425  819425@unizar.es
                Dorian Boleslaw Wozniak             817570  817570@unizar.es
    Resumen: Disparador que asegura que no se disputan dos partidos con los
        mismos equipos como local y visitante

    Entregado el 4 de abril de 2022
*/


CREATE OR REPLACE TRIGGER trigger1
BEFORE INSERT ON Partido
FOR EACH ROW
DECLARE
  n NUMBER;
BEGIN
  -- Obtiene número de partidos jugados en la temporada...
  SELECT COUNT(p.clvPartido) INTO n FROM Partido p, Jornada j
    WHERE p.clvJornada IN
      (SELECT jaux.clvJornada FROM Jornada jaux
      WHERE jaux.clvTemporada=j.clvTemporada AND j.clvJornada=:NEW.clvJornada)
      -- ...tal que los equipos local y visitante sean iguales
    AND p.clvEqLocal=:NEW.clvEqLocal AND p.clvEqVisit=:NEW.clvEqVisit;
  IF n>0 THEN
    RAISE_APPLICATION_ERROR(-20001,'Ya existe ese partido en esa temporada');
  END IF;
END trigger1;
/
