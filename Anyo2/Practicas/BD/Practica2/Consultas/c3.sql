/*
    BASES DE DATOS
    Practica 2

    Fichero: c3.sql

    Autores:    
        Alvaro Seral Gracia                     819425
        Cristian Andrei Selivanov Dobrisan      816456
        Dorian Boleslaw Wozniak                 817570

    Descripción:
        Consulta sobre una base de datos de cine que obtiene el número de
        actores que participan en una sola película de los años 90 que sea
        parte de una saga 
*/

-- Actores que solo han participado en una Película durante la 
-- década de los 90
CREATE VIEW vistaActor AS
    SELECT A.clvActor
    FROM Pelicula P, Obra O, Actua A
    WHERE
        -- La fecha de Estreno de la Obra es de la década de los 90
        (O.Estreno BETWEEN '1990' AND '1999') AND
        -- La Obra es una Película 
        O.clvObra = P.clvPelicula AND
    -- El actor ha participado en la Película   
    A.clvObra = P.clvPelicula
    -- Se agrupa por Actor 
    GROUP BY A.clvActor
    -- 1 = Número de veces que el Actor ha actuado (agregación)
    HAVING '1' = count(A.clvActor);


-- Nombre del Actor
SELECT P.Nombre
FROM Persona P, vistaActor V, Categoriza C, Actua A, Obra O
WHERE
    -- El Actor se encuentra en la vista anterior
    V.clvActor = A.clvActor AND
    P.clvPersona = A.clvActor AND
    -- La obra es de la década de los 90
    (O.Estreno BETWEEN '1990' AND '1999') AND
    -- El Actor ha actuado en la Obra
    O.clvObra = A.clvObra AND
    -- La Obra es una Película que además tiene otra Película 
    -- que Categoriza
    A.clvObra = C.clvPrincipal AND
    -- La Película es precuela o secuela de otra
    C.Tipo IN ('follows', 'followed by')
-- Se agrega por Actor y Película
GROUP BY P.Nombre, O.Titulo
-- 2 <= Número de veces que la Película es Precuela o Saga (agregación)
HAVING '2' <= count(C.clvPrincipal);

DROP VIEW vistaActor;