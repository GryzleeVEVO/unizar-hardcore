/*
    BASES DE DATOS
    Práctica 1: Liga de fútbol
    Fichero: consulta1.sql
    Autores:    Álvaro Seral Gracia                 816456  816456@unizar.es
                Cristian Andrei Selivanov Dobrisan  819425  819425@unizar.es
                Dorian Boleslaw Wozniak             817570  817570@unizar.es
    Resumen: Consulta a la base de datos que muestra los equipos que han estado
            en primera división al menos cinco temporadas sin haber ganado
            ninguna liga

    Entregado el 4 de abril de 2022
*/

SET WRAP OFF;
SET LINESIZE 400;
SET PAGESIZE 50;

-- Selecciona equipos que aparecen en primera división 5 veces o más
SELECT DISTINCT e.clvEquipo FROM Equipo e
WHERE '5'<=(
  -- Obtiene equipos que aparezcan en la tabla final de la temporada
  SELECT count(*) FROM Compiten c
  WHERE e.clvEquipo=c.clvEquipo AND c.clvJornada IN (
    SELECT DISTINCT j.clvJornada FROM Temporada t, Jornada j
    -- Temporadas identificadas como de 1a división
    WHERE j.clvTemporada=t.clvTemporada AND t.clvLiga='1a' AND j.numero=(
      -- Selecciona la última jornada de cada temporada
      SELECT max(j2.numero) FROM Jornada j2
      WHERE j2.clvTemporada=t.clvTemporada)))
INTERSECT
-- Selecciona equipos han participado en 1a división y no ganaron
SELECT DISTINCT c.clvEquipo FROM Compiten c
-- Invierte selección
WHERE c.clvEquipo NOT IN (
  -- Equipos que han ganado 1a división
  SELECT DISTINCT c2.clvEquipo FROM Compiten c2
  WHERE c2.clvJornada IN (
    SELECT DISTINCT j.clvJornada FROM Temporada t, Jornada j
    -- Temporadas identificadas como de 1a división
    WHERE j.clvTemporada=t.clvTemporada AND t.clvLiga='1a'
    -- Donde el equipo seleccionado queda primero
    AND c2.puesto='1' AND j.numero=(
      -- En la ultima jornada de la temporada
      SELECT max(j2.numero) FROM Jornada j2
      WHERE j2.clvTemporada=t.clvTemporada)));
