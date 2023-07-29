/*
    BASES DE DATOS
    Práctica 1: Liga de fútbol
    Fichero: consulta2.sql
    Autores:    Álvaro Seral Gracia                 816456  816456@unizar.es
                Cristian Andrei Selivanov Dobrisan  819425  819425@unizar.es
                Dorian Boleslaw Wozniak             817570  817570@unizar.es
    Resumen: Consulta a la base de datos que muestra las temporadas en las que
            el ganador de segunda división ha ganado más partidos que el
            ganador de primera división.

    Entregado el 4 de abril de 2022
*/

SET WRAP OFF;
SET LINESIZE 400;
SET PAGESIZE 50;

SELECT DISTINCT t.anyo FROM Temporada t
WHERE
-- Cuenta victorias de ganador de temporada de 1a división
(SELECT count(*) FROM Partido p1, Temporada t1, Jornada j1, Compiten c1
-- Comprueba que son de la misma temporada
WHERE t.anyo=t1.anyo
-- Última jornada de 1a división
AND j1.clvTemporada=t1.clvTemporada AND t1.clvLiga='1a' AND j1.numero=
  (SELECT max(j1aux.numero) FROM Jornada j1aux
  WHERE j1aux.clvTemporada=t1.clvTemporada)
-- Primer puesto
AND c1.clvJornada=j1.clvJornada AND c1.puesto='1' AND p1.clvJornada IN
  (SELECT j1aux.clvJornada FROM Jornada j1aux
  WHERE j1aux.clvTemporada=t1.clvTemporada)
-- Partidos ganados
AND ((p1.clvEqLocal=c1.clvEquipo AND resLocal>resVisit) OR (p1.clvEqVisit=c1.clvEquipo AND resLocal<resVisit)))
< -- MENOR QUE
-- Cuenta victorias de ganador de temporada de 2a división
(SELECT count(*) FROM Partido p2, Temporada t2, Jornada j2, Compiten c2
-- Misma temporada
WHERE t.anyo=t2.anyo
-- Última jornada
AND j2.clvTemporada=t2.clvTemporada AND t2.clvLiga='2a' AND j2.numero=
  (SELECT max(j2aux.numero) FROM Jornada j2aux
  WHERE j2aux.clvTemporada=t2.clvTemporada)
-- Primer puesto
AND c2.clvJornada=j2.clvJornada AND c2.puesto='1' AND p2.clvJornada IN
  (SELECT j2aux.clvJornada FROM Jornada j2aux
  WHERE j2aux.clvTemporada=t2.clvTemporada)
-- Partidos gandaos
AND ((p2.clvEqLocal=c2.clvEquipo AND resLocal>resVisit) OR (p2.clvEqVisit=c2.clvEquipo AND resLocal<resVisit)));
