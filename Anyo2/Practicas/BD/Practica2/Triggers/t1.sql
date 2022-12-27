/*
    BASES DE DATOS
    Practica 2

    Fichero: t1

    Autores:    
        Alvaro Seral Gracia                     819425
        Cristian Andrei Selivanov Dobrisan      816456
        Dorian Boleslaw Wozniak                 817570

    Descripción:
        Trigger que comprueba que cualquier identificador de la tabla de Películas no puede estar como 
        identificador en la tabla de Series, y viceversa
*/


-- Cualquier identificador de la tabla de Películas no puede estar como 
-- identificador en la tabla de Series 
CREATE OR REPLACE TRIGGER trigger1P
BEFORE INSERT ON Pelicula
FOR EACH ROW
DECLARE 
    n NUMBER;
BEGIN
    -- n = número de veces que la clvPelicula nueva aparece 
    -- en la tabla Serie
    SELECT count(*) 
    INTO n 
    FROM Serie
    WHERE clvSerie=:NEW.clvPelicula;
    -- Si n>0, ya existe esa la clvPelicula en Series,  
    -- lanza un error y no ejecuta el insert
    IF n>0 THEN RAISE_APPLICATION_ERROR(-20001, 'Ya existe una serie con id');
    END IF;
END trigger1P;
/

-- Cualquier identificador de la tabla de Series no puede estar como
-- identificador en la tabla de Películas
CREATE OR REPLACE TRIGGER trigger1S
BEFORE INSERT ON Serie
FOR EACH ROW
DECLARE 
    n NUMBER;
BEGIN
    -- n = número de veces que la clvSerie nueva aparece 
    -- en la tabla Película
    SELECT count(*) 
    INTO n 
    FROM Pelicula
    WHERE clvPelicula=:NEW.clvSerie;
    -- Si n>0, ya existe esa la clvSerie en Películas,  
    -- lanza un error y no ejecuta el insert
    IF n>0 THEN RAISE_APPLICATION_ERROR(-20002, 'Ya existe una película con esa id');
    END IF;
END trigger1S;
/