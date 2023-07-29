---
title: |
  | Inteligencia Artificial
  | Práctica 5: Redes bayesianas
subtitle: "Memoria"
author: "Dorian Boleslaw Wozniak (817570@unizar.es)"
---

### Estudio previo

#### Apartado 1

![Estudio previo del apartado 1](IMG.jpg)

#### Apartado 2



### Trabajo en OpenMarkov

#### Apartado 1

Para poder probar los resultados del primer apartado, se ha creado en OpenMarkov una red bayesiana como la que se ha dado en el enunciado. 

![Red bayesiana creada en OpenMarkov](screenshot.jpg)

![Tabla de probabilidades para la red bayesiana](screenshot2.jpg)

##### Subapartado a)

Se ha comprobado que en ambos casos dados, las variables no son independientes dado S. Para ello, se ha observado, utilizando el modo Inferencia, que marcando S como presente e intentando modificar alguna de las dos variables, la otra también cambia (aunque sea poco).

![Probabilidades de las variables al dar la variable S](screenshot3.jpg)

![Probabilidades de las variables al dar la variable S e I](screenshot4.jpg)

![Probabilidades de las variables al dar la variable S y V](screenshot5.jpg)

##### Subapartado b)

Similar al primer apartado, se han dado las variables dadas y se ha comprobado la probabilidad de la variable C.

![Probabilidad de C al conocer E, S y no V](screenshot6.jpg)

#### Apartado 2

##### Subapartado a)

Se ha seguido las indicaciones del guión para generar la red.

![Red bayesiana generada por el aprendizaje](screenshot7.jpg)

![Tabla de probabilidades de la variable Sexo](screenshot8.jpg)

![Tabla de probabilidades de la variable Carrera](screenshot9.jpg)

![Tabla de probabilidades de la variable Admitidos](screenshot10.jpg)

##### Subapartado b)

Se ha utliizado la herramienta Inferencia para calcular las probabilidades.

![Probabilidad para ser admitido como hombre](screenshot11.jpg)

![Probabilidad para ser admitido como hombre](screenshot12.jpg)

##### Subapartado c)

El resultado (hay menos mujeres admitidas) no tiene que significar necesariamente que existe discriminación de género. Los pequeños tamaños muestrales para las mujeres que han intentado entrar en las carreras A y B pueden estar distorsionando las probabilidades reales de admisión. 