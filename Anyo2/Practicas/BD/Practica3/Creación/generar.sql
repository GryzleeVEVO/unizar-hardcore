-- Aeropuerto
select iata IATA, airport NombreA, city Ciudad, state Estado from airports;

-- Avion
select tailnum Matricula, year Montaje, model clvModelo, manufacturer clvFabricante from planes;

-- Modelo
select distinct model nombreM, manufacturer clvFabricante, engine_type Motor from planes where model<>"";

-- Fabricante
select distinct manufacturer NombreF from planes where manufacturer<>"";

-- Compañia
select code idCompañia, name NombreC from carriers;

-- Vuelo
select @rownum:=@rownum+1 as idVuelo, flightNum numVuelo, crsDepTime Salida, crsArrTime Llegada, flightDate Fecha, cancelled Cancelado, 
(coalesce(carrierDelay,0)+coalesce(weatherDelay,0)+coalesce(nasDelay,0)+coalesce(securityDelay,0)+coalesce(lateAircraftDelay,0)+coalesce(divArrDelay,0)) Retraso,
origin clvOrigen, dest clvDestino, carrier clvCompañia, tailNum clvAvion
from (select @rownum:=0) as fila, flights200810;

-- Desvio
-- Problema: div1tailNum a veces es null
select 1 Escala, clvVuelo, clvAeropuerto, clvAvion
from (select @rownum:=@rownum+1 as clvVuelo, div1airport clvAeropuerto, div1TailNum clvAvion, diverted from (select @rownum:=0) as fila, flights200810) as T
where diverted=1 and clvAeropuerto<>""
union
select 2 Escala, clvVuelo, clvAeropuerto, clvAvion
from (select @rownum:=@rownum+1 as clvVuelo, div2airport clvAeropuerto, div2TailNum clvAvion, diverted from (select @rownum:=0) as fila, flights200810) as T
where diverted=1 and clvAeropuerto<>"";