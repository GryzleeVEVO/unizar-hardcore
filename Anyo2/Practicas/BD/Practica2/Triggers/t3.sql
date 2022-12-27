/*
    BASES DE DATOS
    Practica 2

    Fichero: t3

    Autores:    
        Alvaro Seral Gracia                     819425
        Cristian Andrei Selivanov Dobrisan      816456
        Dorian Boleslaw Wozniak                 817570

    Descripción:
        Trigger que comprueba que si una película ‘Uno’ es precuela de una película ‘Dos’, 
        entonces ‘Dos’ es secuela de ‘Uno’; y viceversa.
*/


CREATE OR REPLACE TRIGGER trigger3
AFTER INSERT ON Categoriza
FOR EACH ROW
DECLARE
    n NUMBER;
BEGIN
    -- Si el tipo de inserción es una secuela con respecto a la referencia:
    IF :NEW.Tipo='follows' THEN
        -- n = número de veces que la tupla de Películas inversa aparece con el tipo precuela
        SELECT count(*) 
        INTO n 
        FROM Categoriza
        WHERE 
              :NEW.clvPrincipal=clvReferencia AND 
              :NEW.clvReferencia=clvPrincipal AND 
              Tipo='followed by';
        -- Si n = 0, no hay ninguna aparición, se inserta automáticamente la nueva fila 
        -- correspondiente
        IF n=0 THEN 
            INSERT INTO Categoriza (clvPrincipal,clvReferencia,Tipo) VALUES (:NEW.clvReferencia,:NEW.clvPrincipal,'followed by');
        END IF;
    END IF;
    -- Si el tipo de inserción es una precuela con respecto a la referencia:
    IF :NEW.Tipo='followed by' THEN
        -- n = número de veces que la tupla de Películas inversa aparece con el tipo secuela
        SELECT count(*) 
        INTO n 
        FROM Categoriza
        WHERE 
              :NEW.clvPrincipal=clvReferencia AND 
              :NEW.clvReferencia=clvPrincipal AND 
              Tipo='follows';
        -- Si n = 0, no hay ninguna aparición, se inserta automáticamente la nueva fila 
        -- correspondiente
        IF n=0 THEN 
            INSERT INTO Categoriza (clvPrincipal,clvReferencia,Tipo) VALUES (:NEW.clvReferencia,:NEW.clvPrincipal,'follows');
        END IF;
    END IF;
--
END trigger3;
/