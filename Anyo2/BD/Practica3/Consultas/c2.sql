-- Vista con nombre, IATA y media de edad (en años) de cada aeropuerto.
CREATE VIEW Medias AS
  -- Para calcular la media de edad de cada aeropuerto: 
  -- Se suman todas las fechas de montaje de cada avión y el resultado 
  -- se divide entre el número de aviones que disponen de fecha de montaje.
  SELECT NombreA, IATA, SUM(Montaje)/COUNT(Montaje) Media
  FROM (
    -- Tabla con los distintos aviones y sus fechas de montaje que operan en cada aeropuerto
    SELECT DISTINCT NombreA, IATA, Matricula, Montaje
    FROM Aeropuerto, Avion, Vuelo
    -- “Relacionar” el avión de cada vuelo con el aeropuerto en cuestión
    WHERE (clvDestino=IATA OR clvOrigen=IATA) AND clvAvion=Matricula
  )
  -- Agrupar por nombre y IATA de aeropuertos
  GROUP BY NombreA, IATA;


-- Consulta 2: Nombre, IATA y media de edad del aeropuerto en el que operan los aviones mas modernos (es decir, con menor media de edad). 
-- Para conseguir la media de edad real a partir de la media de los años 
-- se hace la resta entre 2022 y la media calculada
SELECT NombreA Aeropuerto, IATA, 2022-Media Media
FROM Medias
-- La media de edad (en años) tiene que ser la mayor entre las medias calculadas
WHERE Media=(SELECT MAX(Media) FROM Medias);



DROP VIEW Medias;