/*
    BASES DE DATOS
    Práctica 1: Liga de fútbol
    Fichero: trigger2.sql
    Autores:    Álvaro Seral Gracia                 816456  816456@unizar.es
                Cristian Andrei Selivanov Dobrisan  819425  819425@unizar.es
                Dorian Boleslaw Wozniak             817570  817570@unizar.es
    Resumen: Disparador que asegura que un equipo no participe en dos
        divisiones diferentes en la misma temporada

    Entregado el 4 de abril de 2022
*/

CREATE OR REPLACE TRIGGER trigger2
BEFORE INSERT ON Partido
FOR EACH ROW
DECLARE
  n NUMBER;
  m NUMBER;
BEGIN
  -- Cuenta divisiones en las que participa el equipo local aparte de la que se va a insertar
  SELECT COUNT(DISTINCT t.clvLiga) INTO n FROM Partido p, Jornada j, Temporada t
    WHERE j.clvJornada=:NEW.clvJornada AND t.clvTemporada=j.clvTemporada AND p.clvJornada IN
      (SELECT jaux.clvJornada FROM Jornada jaux, Temporada taux
      -- Mismo año, liga diferente
      WHERE jaux.clvTemporada=taux.clvTemporada AND taux.anyo=t.anyo AND taux.clvLiga<>t.clvLiga)
    -- el equipo nuevo local debe ser el mismo que cualquiera de los equipos local o visitante de otras temporadas
    AND (p.clvEqLocal=:NEW.clvEqLocal OR p.clvEqVisit=:NEW.clvEqLocal);
  -- Cuenta divisiones en las que participa el equipo local aparte de la que se va a insertar
  SELECT COUNT(DISTINCT t.clvLiga) INTO m FROM Partido p, Jornada j, Temporada t
    WHERE j.clvJornada=:NEW.clvJornada AND t.clvTemporada=j.clvTemporada AND p.clvJornada IN
      (SELECT jaux.clvJornada FROM Jornada jaux, Temporada taux
      -- Mismo año, liga diferente
      WHERE jaux.clvTemporada=taux.clvTemporada AND taux.anyo=t.anyo AND taux.clvLiga<>t.clvLiga)
    -- el equipo nuevo visitante debe ser el mismo que cualquiera de los equipos local o visitante de otras temporadas
    AND (p.clvEqLocal=:NEW.clvEqVisit OR p.clvEqVisit=:NEW.clvEqVisit);
  IF n>0 OR m>0 THEN
    RAISE_APPLICATION_ERROR(-20002,'Ese partido ya esta en otra division esta temporada');
  END IF;
END trigger2;
/
