-- Vista con clvCompania y porcentaje de vuelos de cada compañía 
-- que despegan y aterrizan en un mismo estado
CREATE VIEW Porcentajes AS
  -- Para calcular el porcentaje de vuelos de cada compañía:
  -- Se cuenta el número de vuelos que cumplen las condiciones y se divide entre
  -- el número total de vuelos de la compañía. Luego, se multiplica por 100
  SELECT VC.clvCompania, (COUNT(*)/VC.num)*100 p
  FROM Vuelo V, Aeropuerto A1, Aeropuerto A2, (
    -- Tabla con el número total de vuelos de cada compañía
    SELECT clvCompania, COUNT(*) num
    FROM Vuelo 
    -- Agrupar por compañía
    GROUP BY clvCompania) 
    VC
  -- Los aeropuertos de salida y de llegada del vuelo ocurren en el mismo estado
  WHERE A1.IATA=V.clvOrigen AND A2.IATA=V.clvDestino AND A1.Estado=A2.Estado 
    -- El número de vuelos totales y el número de vuelos condicionados 
    -- pertenecen a la misma compañía
    AND VC.clvCompania=V.clvCompania
  -- Agrupar por compañía (y número de vuelos totales de esta)
  GROUP BY VC.clvCompania, VC.num; 



-- Consulta 3: Compania (o companias, en caso de empate) con el mayor porcentaje de vuelos que despegan y aterrizan en un mismo estado.
SELECT NombreC Compania, p Porcentaje
FROM porcentajes, Compania
-- El porcentaje de vuelos tiene que ser el mayor
WHERE clvCompania=idCompania AND p=(SELECT MAX(p) FROM porcentajes);



DROP VIEW porcentajes;