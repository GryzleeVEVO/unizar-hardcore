/*
    BASES DE DATOS
    Practica 2

    Fichero: c3.sql

    Autores:    
        Alvaro Seral Gracia                     819425
        Cristian Andrei Selivanov Dobrisan      816456
        Dorian Boleslaw Wozniak                 817570

    Descripci�n:
        Consulta sobre una base de datos de cine que obtiene el n�mero de
        actores que participan en una sola pel�cula de los a�os 90 que sea
        parte de una saga 
*/

-- Actores que solo han participado en una Pel�cula durante la 
-- d�cada de los 90
CREATE VIEW vistaActor AS
    SELECT A.clvActor
    FROM Pelicula P, Obra O, Actua A
    WHERE
        -- La fecha de Estreno de la Obra es de la d�cada de los 90
        (O.Estreno BETWEEN '1990' AND '1999') AND
        -- La Obra es una Pel�cula 
        O.clvObra = P.clvPelicula AND
    -- El actor ha participado en la Pel�cula   
    A.clvObra = P.clvPelicula
    -- Se agrupa por Actor 
    GROUP BY A.clvActor
    -- 1 = N�mero de veces que el Actor ha actuado (agregaci�n)
    HAVING '1' = count(A.clvActor);


-- Nombre del Actor
SELECT P.Nombre
FROM Persona P, vistaActor V, Categoriza C, Actua A, Obra O
WHERE
    -- El Actor se encuentra en la vista anterior
    V.clvActor = A.clvActor AND
    P.clvPersona = A.clvActor AND
    -- La obra es de la d�cada de los 90
    (O.Estreno BETWEEN '1990' AND '1999') AND
    -- El Actor ha actuado en la Obra
    O.clvObra = A.clvObra AND
    -- La Obra es una Pel�cula que adem�s tiene otra Pel�cula 
    -- que Categoriza
    A.clvObra = C.clvPrincipal AND
    -- La Pel�cula es precuela o secuela de otra
    C.Tipo IN ('follows', 'followed by')
-- Se agrega por Actor y Pel�cula
GROUP BY P.Nombre, O.Titulo
-- 2 <= N�mero de veces que la Pel�cula es Precuela o Saga (agregaci�n)
HAVING '2' <= count(C.clvPrincipal);

DROP VIEW vistaActor;