/*
    BASES DE DATOS
    Práctica 1: Liga de fútbol
    Fichero: consulta3.sql
    Autores:    Álvaro Seral Gracia                 816456  816456@unizar.es
                Cristian Andrei Selivanov Dobrisan  819425  819425@unizar.es
                Dorian Boleslaw Wozniak             817570  817570@unizar.es
    Resumen: Consulta a la base de datos que muestra los equipos y temporadas
            donde el ganador de la liga haya perdido todos los partidos contra
            el subcampeón

    Entregado el 4 de abril de 2022
*/

SET WRAP OFF;
SET LINESIZE 400;
SET PAGESIZE 50;

SELECT DISTINCT c1.clvEquipo, t1.anyo, t1.clvLiga FROM Temporada t1, Jornada j1, Compiten c1
-- Última jornada de temporada
WHERE j1.clvTemporada=t1.clvTemporada AND j1.numero=
  (SELECT DISTINCT max(jaux.numero) FROM Jornada jaux
  WHERE jaux.clvTemporada=t1.clvTemporada)
-- Primero de la temporada
AND c1.clvJornada=j1.clvJornada AND c1.puesto='1' AND 0=
-- Selecciona sólo si no ha ganado ningún partido contra el segundo
  (SELECT count(p.clvPartido) FROM Jornada j2, Compiten c2, Partido p
-- Segundo de la temporada
  WHERE c2.clvJornada=j1.clvJornada AND c2.puesto='2' AND p.clvJornada IN
-- Dentro de la temporada...
    (SELECT DISTINCT jaux.clvJornada FROM Jornada jaux
    WHERE jaux.clvTemporada=t1.clvTemporada)
-- ...partidos que haya ganado el primero de liga al final contra el segundo
  AND ((p.clvEqLocal=c1.clvEquipo AND p.clvEqVisit=c2.clvEquipo AND resLocal>resVisit)
    OR (p.clvEqLocal=c2.clvEquipo AND p.clvEqVisit=c1.clvEquipo AND resLocal<resVisit)));
