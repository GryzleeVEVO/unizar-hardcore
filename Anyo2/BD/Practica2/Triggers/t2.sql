/*
    BASES DE DATOS
    Practica 2

    Fichero: t2

    Autores:    
        Alvaro Seral Gracia                     819425
        Cristian Andrei Selivanov Dobrisan      816456
        Dorian Boleslaw Wozniak                 817570

    Descripción:
        Trigger que comprueba que si una película 'Uno' es remake de una película 'Dos', entonces 'Dos' no puede 
        ser remake de 'Uno'. Lo mismo con precuelas y secuelas.
*/


CREATE OR REPLACE TRIGGER trigger2
BEFORE INSERT ON Categoriza
FOR EACH ROW
DECLARE
    n NUMBER;
BEGIN
    -- n = número de veces que la tupla de Películas opuesta a la nueva es del tipo nuevo
    SELECT count(*) 
    INTO n 
    FROM Categoriza
    WHERE 
          :NEW.clvPrincipal=clvReferencia AND 
          :NEW.clvReferencia=clvPrincipal AND 
          :NEW.Tipo=Tipo;
    -- Si n>1, ya existe una relación entre las dos películas nuevas del mismo tipo pero 
    -- con claves invertidas, lanza un error y no ejecuta el insert
    IF n>1 THEN
        RAISE_APPLICATION_ERROR(-20003, 'Ya existe el contrareciproco de esta insercion.');
    END IF;
END trigger2;
/