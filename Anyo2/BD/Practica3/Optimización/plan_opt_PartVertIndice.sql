SET LINESIZE 200
SET PAGESIZE 1000

CREATE INDEX ind_vuelo1 on VueloPrincipal(clvOrigen, clvDestino, clvAvion);
CREATE INDEX ind_vuelo2 on VueloPrincipal(clvOrigen, clvDestino, clvCompania);


-- Consulta 1
EXPLAIN PLAN FOR 
SELECT COUNT(*) Num_Companias FROM (
  SELECT 1
  FROM VueloPrincipal, Aeropuerto
  WHERE (clvDestino=IATA OR clvOrigen=IATA) AND Estado='AK'
  GROUP BY clvCompania
  HAVING 5<=(COUNT(DISTINCT IATA))
);

SELECT PLAN_TABLE_OUTPUT FROM TABLE (DBMS_XPLAN.DISPLAY());



-- Vista consulta 2
CREATE VIEW Medias AS
  SELECT Aeropuerto, IATA, SUM(Montaje)/COUNT(Montaje) Media
  FROM (
    SELECT nombreA Aeropuerto, IATA, Matricula, Montaje
    FROM Aeropuerto, Avion, VueloPrincipal
    WHERE (clvDestino=IATA OR clvOrigen=IATA) AND clvAvion=Matricula
    GROUP BY nombreA, IATA, Matricula, Montaje
  )
  GROUP BY Aeropuerto, IATA;


-- Consulta 2
EXPLAIN PLAN FOR 
SELECT Aeropuerto, IATA, 2022-Media Media
FROM Medias
WHERE Media=(SELECT MAX(Media) FROM Medias);

SELECT PLAN_TABLE_OUTPUT FROM TABLE (DBMS_XPLAN.DISPLAY());


DROP VIEW Medias;




-- Vista consulta 3
CREATE VIEW Porcentajes AS
  SELECT VC.clvCompania, (COUNT(*)/VC.num)*100 p
  FROM VueloPrincipal V, Aeropuerto A1, Aeropuerto A2, (
    SELECT clvCompania, COUNT(*) num
    FROM VueloPrincipal 
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


DROP VIEW porcentajes;


DROP INDEX ind_vuelo1;
DROP INDEX ind_vuelo2;