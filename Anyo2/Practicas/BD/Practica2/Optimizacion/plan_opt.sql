/*
    BASES DE DATOS
    Practica 2

    Fichero: plan_opt.sql

    Autores:    
        Alvaro Seral Gracia                     819425
        Cristian Andrei Selivanov Dobrisan      816456
        Dorian Boleslaw Wozniak                 817570

    Descripción:
        Sentencias SQL que obtienen los planes de ejecución de las tres
        consultas realizadas optmizadas
*/

SET LINESIZE 200
SET PAGESIZE 1000


CREATE MATERIALIZED VIEW vistaActor 
BUILD IMMEDIATE
REFRESH COMPLETE 
ON DEMAND
AS
    SELECT A.clvActor 
    FROM Pelicula P, Obra O, Actua A
    WHERE 
        (O.Estreno BETWEEN '1990' AND '1999') AND 
        O.clvObra = P.clvPelicula AND 
        A.clvObra = P.clvPelicula
    GROUP BY A.clvActor
    HAVING '1' = count(A.clvActor);

CREATE INDEX ind_act_clvObra on Actua(clvObra);



EXPLAIN PLAN FOR 
SELECT seccion, total, (seccion / total) * '100' Porcentaje 
FROM
(
    SELECT count(*) 
    AS seccion
    FROM 
    (
        SELECT DISTINCT P.clvPelicula 
        FROM Pelicula P, Actua A
        WHERE P.clvPelicula = A.clvObra 
        GROUP BY P.clvPelicula
        HAVING '3' >= count(A.clvActor)
    )
),
(
    SELECT count(DISTINCT P.clvPelicula) 
    AS total 
    FROM Pelicula P
);   


SELECT PLAN_TABLE_OUTPUT FROM TABLE (DBMS_XPLAN.DISPLAY());

EXPLAIN PLAN FOR 
SELECT DISTINCT 
      O.Titulo Titulo_Obra, 
      Pj.Descripcion Descripcion_Personaje, 
      count(I.clvActor) Numero_Actores 
FROM Obra O, Personaje Pj, Interpreta I, Tema T
WHERE
      O.clvObra=I.clvObra AND 
      Pj.clvPersonaje=I.clvPersonaje AND
      O.clvObra=T.clvObra AND 
      (
            T.Genero LIKE '%terror' OR 
            T.Genero LIKE '%horror%' OR 
            T.Genero LIKE '%fear%' 
      )
GROUP BY O.Titulo, Pj.Descripcion, T.Genero
HAVING '50' <= count(I.clvActor); 

SELECT PLAN_TABLE_OUTPUT FROM TABLE (DBMS_XPLAN.DISPLAY());

EXPLAIN PLAN FOR
SELECT P.Nombre
FROM Persona P, vistaActor V, Categoriza C, Actua A, Obra O
WHERE 
    V.clvActor = A.clvActor AND 
    P.clvPersona = A.clvActor AND 
    (O.Estreno BETWEEN '1990' AND '1999') AND 
    O.clvObra = A.clvObra AND
    A.clvObra = C.clvPrincipal AND 
    C.Tipo IN ('follows', 'followed by')
GROUP BY P.Nombre, O.Titulo
HAVING '2' <= count(C.clvPrincipal);

SELECT PLAN_TABLE_OUTPUT FROM TABLE (DBMS_XPLAN.DISPLAY());

DROP MATERIALIZED VIEW vistaActor;

DROP INDEX ind_act_clvObra;