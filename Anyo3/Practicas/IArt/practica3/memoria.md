---
title: |
  | Inteligencia Artificial
  | Practica 3: Búsqueda local
subtitle: "Memoria"
author: "Dorian Boleslaw Wozniak (817570@unizar.es)"
---

### Resultados obtenidos

```
NQueens HillClimbing con 10000 estados iniciales diferentes -->
Fallos: 85.80
Coste medio fallos: 4.05
Éxitos: 14.20
Coste medio Éxitos: 5.07

Search Outcome=SOLUTION_FOUND
Final State=
----Q---
------Q-
---Q----
Q-------
--Q-----
-------Q
-----Q--
-Q------

Número de intentos: 10
Coste medio fallos: 4.00
Coste éxito: 7

NQueens Simulated Annealing con 1000 estados iniciales diferentes -->
Parámetros Scheduler: Scheduler (5, 0.500000, 200)
Fallos: 72.90
Coste medio fallos: 200.00
Éxitos: 27.10
Coste medio Éxitos: 200.00

Search Outcome=SOLUTION_FOUND
Final State=
--Q-----
-----Q--
---Q----
-Q------
-------Q
----Q---
------Q-
Q-------

Parámetros Scheduler: Scheduler (5, 0.500000, 200)
Número de intentos: 1
Coste medio fallos: 0.00
Coste éxito: 200

Genetic algorithm
Parámetros iniciales: 	Población: 50, Probabilidad mutación: 0.35
Mejor indivíduo=
----Q---
Q-------
---Q----
-----Q--
-------Q
-Q------
------Q-
--Q-----

Tamaño tablero		= 8
Fitness			= 28.00
Es objetivo		= true
Tamaño de población	= 50
Iteraciones		= 874
Tiempo			= 4845ms
```

### Comentarios

Tras realizar una serie de pruebas con diferentes valores para k y $\Delta$ para el algoritmo de enfriamiento simulado, y para población inicial y probabilidad de mutación para el algoritmo genético, se ha deciddo elegir los siguientes valores como equilibrio entre probabilidad de éxito y tiempo tomado:

- Enfriamiento simulado: (5, 0.5, 200)
- Genético: (50, 0.35)