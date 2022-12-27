SET LINESIZE 200
SET PAGESIZE 1000


-- Consulta 1
EXPLAIN PLAN FOR 
SELECT COUNT(*) Num_Companias FROM (
  SELECT 1
  FROM Vuelo, Aeropuerto
  WHERE (clvDestino=IATA OR clvOrigen=IATA) AND Estado='AK'
  GROUP BY clvCompania
  HAVING 5<=(COUNT(DISTINCT IATA))
);

SELECT PLAN_TABLE_OUTPUT FROM TABLE (DBMS_XPLAN.DISPLAY());



-- Vista consulta 2
CREATE MATERIALIZED VIEW Medias
BUILD IMMEDIATE
REFRESH COMPLETE 
ON DEMAND
AS
  SELECT nombreA Aeropuerto, IATA, SUM(Montaje)/COUNT(Montaje) Media
  FROM Aeropuerto, Avion, Vuelo
  WHERE (clvDestino=IATA OR clvOrigen=IATA) AND clvAvion=Matricula -- AND Montaje is not null
  GROUP BY nombreA, IATA;


-- Consulta 2
EXPLAIN PLAN FOR 
SELECT Aeropuerto, IATA, 2022-Media Media
FROM Medias
WHERE media=(SELECT MAX(media) FROM Medias);

SELECT PLAN_TABLE_OUTPUT FROM TABLE (DBMS_XPLAN.DISPLAY());


DROP MATERIALIZED VIEW Medias;




-- Vista consulta 3
CREATE MATERIALIZED VIEW Porcentajes
BUILD IMMEDIATE
REFRESH COMPLETE 
ON DEMAND
AS
  SELECT VC.clvCompania, (COUNT(*)/VC.num)*100 p
  FROM Vuelo V, Aeropuerto A1, Aeropuerto A2, (
    SELECT clvCompania, COUNT(*) num
    FROM Vuelo 
    GROUP BY clvCompania) 
    VC
  WHERE A1.IATA=V.clvOrigen AND A2.IATA=V.clvDestino AND A1.Estado=A2.Estado 
    AND VC.clvCompania=V.clvCompania
  GROUP BY VC.clvCompania, VC.num; 
  

-- Consulta 3
EXPLAIN PLAN FOR 
SELECT NombreC Compania, p Porcentaje
FROM porcentajes, Compania
WHERE clvCompania=idCompania AND p=(SELECT MAX(p) FROM porcentajes);

SELECT PLAN_TABLE_OUTPUT FROM TABLE (DBMS_XPLAN.DISPLAY());


DROP MATERIALIZED VIEW porcentajes;