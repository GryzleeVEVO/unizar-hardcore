-- Consulta 1: Número de compañías que operan en al menos cinco aeropuertos de Alaska.
-- Número de 1’s que aparecen, es decir, número de compañías que cumplen las condiciones
SELECT COUNT(*) Num_Companias FROM (
  -- ‘1’ por cada compañía que cumplen las condiciones
  SELECT 1
  FROM Vuelo, Aeropuerto
  -- El aeropuerto de salida o de llegada del vuelo 
  -- se encuentra en el estado de Alaska (‘AK’)
  WHERE (clvDestino=IATA OR clvOrigen=IATA) AND Estado='AK'
  -- Agrupar por compañías
  GROUP BY clvCompania
  -- El número de aeropuertos de Alaska donde opera la compañía debe ser >= 5
  HAVING 5<=(COUNT(DISTINCT IATA))
);



-- Extra: Nombres de las compañías que verifican la consulta 1.
SELECT NombreC Companias
FROM Vuelo, Aeropuerto, Compania
-- El aeropuerto de salida o de llegada del vuelo 
-- se encuentra en el estado de Alaska (‘AK’)
WHERE (clvDestino=IATA OR clvOrigen=IATA) AND Estado='AK' AND clvCompania=idCompania
-- Agrupar por nombres de compañías
GROUP BY NombreC
-- El número de aeropuertos de Alaska donde opera la compañía debe ser >= 5
HAVING 5<=(COUNT(DISTINCT IATA));