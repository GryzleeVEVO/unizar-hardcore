/*
    BASES DE DATOS
    Practica 2

    Fichero: c1

    Autores:    
        Alvaro Seral Gracia                     819425
        Cristian Andrei Selivanov Dobrisan      816456
        Dorian Boleslaw Wozniak                 817570

    Descripción:
        Consulta SQL sobre una base de datos de cine que obtiene el procentaje
        de películas con hasta 3 actores o actrices
*/

-- (Número de películas con 3 actores o más / 
-- Número de películas totales) * 100
SELECT (seccion/total) * '100' Porcentaje
FROM
(
    -- Número de películas con 3 actores o más
    SELECT count(*)
    AS seccion
    FROM
    (
        -- Tabla con películas con 3 actores o más
        SELECT P.clvPelicula
        FROM Pelicula P, Actua A
        -- La obra en la que actua un Actor es una Película
        WHERE P.clvPelicula=A.clvObra
        -- Se agrupa por Películas
        GROUP BY P.clvPelicula
        -- 3 >= Número de Actores de la Película (agrupación)
        HAVING '3' >= count(A.clvActor)
    )
),
(
    -- Número de películas totales
    SELECT count(P.clvPelicula)
    AS total
    FROM Pelicula P
);
