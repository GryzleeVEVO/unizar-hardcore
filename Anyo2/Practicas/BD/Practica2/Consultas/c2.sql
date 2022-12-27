/*
    BASES DE DATOS
    Practica 2

    Fichero: c2.sql

    Autores:    
        Alvaro Seral Gracia                     819425
        Cristian Andrei Selivanov Dobrisan      816456
        Dorian Boleslaw Wozniak                 817570
        
    Descripción:
      Consulta SQL sobre una base de datos de cine que obtiene las películas
      y series de terror en las que el mismo personaje ha sido interpretado por
      50 actores o más, junto al personaje y número de actores
      
*/

-- Titulo de la Obra, Descripción del Personaje, 
-- Número de Actores que lo interpretan en la Película
SELECT DISTINCT
      O.Titulo Titulo_Obra,
      Pj.Descripcion Descripcion_Personaje,
      count(I.clvActor) Numero_Actores
FROM Obra O, Personaje Pj, Interpreta I, Tema T
WHERE
      O.clvObra = I.clvObra AND
      -- El Personaje interpretado es de la Obra
      Pj.clvPersonaje = I.clvPersonaje AND
      -- El Genero a buscar está asociado a la Obra
      O.clvObra = T.clvObra AND
      -- El Género es de Terror (o semejantes)
      -- Al utilizar <%> antes y después de las palabras, cogemos 
      -- cualquier género que tenga esa secuencia. En terror solo está 
      -- al principio ya que todos los nuevos Géneros que surgirían al 
      -- poner <%> después son de terrorismo
      (
            T.Genero LIKE '%terror' OR
            T.Genero LIKE '%horror%' OR
            T.Genero LIKE '%fear%'
      )
-- Se agrupa por Obras, Personaje y Tema
GROUP BY O.Titulo, Pj.Descripcion, T.Genero
-- 50 <= Número de Actores de la Obra con ese Tema y ese Personaje 
-- (agregación)
HAVING '50' <= count(I.clvActor);

