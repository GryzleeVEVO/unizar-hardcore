-- Trigger 3: Cuando se usan particiones verticales, al intentar insertar elementos en la tabla (o vista) Vuelo, 
--            en vez de eso se han de insertar en las tablas VueloPrincipal y VueloSecundario
CREATE OR REPLACE TRIGGER trigger3
INSTEAD OF INSERT ON Vuelo
FOR EACH ROW
BEGIN
  INSERT INTO VueloPrincipal (idVuelo, clvOrigen, clvDestino, clvCompania, clvAvion) 
    VALUES (:NEW.idVuelo, :NEW.clvOrigen, :NEW.clvDestino, :NEW.clvCompania, :NEW.clvAvion);
  INSERT INTO VueloSecundario (idVuelo, numVuelo, Salida, Llegada, Fecha, Cancelado, Retraso) 
    VALUES (:NEW.idVuelo, :NEW.numVuelo, :NEW.Salida, :NEW.Llegada, :NEW.Fecha, :NEW.Cancelado, :NEW.Retraso);
END trigger3;
/