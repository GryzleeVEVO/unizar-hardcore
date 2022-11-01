---
title: |
  | Inteligencia Artificial
  | Trabajo TP6: Parte 1
subtitle: "Memoria: Resolución de sudokus mediante propagación de restricciones"
author: "Dorian Boleslaw Wozniak (817570@unizar.es)"
---

### Sudokus

Un sudoku es un juego consistente en un tablero inicial con una serie de números en el rango 1-9 precolocados en ciertas celdas. El objetivo del juego es rellenar el sudoku con números del 1 al 9 tal que:

- No debe repetirse el mismo número dos veces en la misma fila
- No debe repetirse el mismo número dos veces en la misma columna
- No debe repetirse el mismo número dos veces en la misma caja (las cajas dividen el tablero en 9 cuadrados 3x3)

```
 A1 A2 A3| A4 A5 A6| A7 A8 A9
 B1 B2 B3| B4 B5 B6| B7 B8 B9
 C1 C2 C3| C4 C5 C6| C7 C8 C9
---------+---------+---------
 D1 D2 D3| D4 D5 D6| D7 D8 D9
 E1 E2 E3| E4 E5 E6| E7 E8 E9
 F1 F2 F3| F4 F5 F6| F7 F8 F9
---------+---------+---------
 G1 G2 G3| G4 G5 G6| G7 G8 G9
 H1 H2 H3| H4 H5 H6| H7 H8 H9
 I1 I2 I3| I4 I5 I6| I7 I8 I9
```

### Propagación de restricciones

Aunque existen mas estrategias para resolver el sudoku, se va a solucionar el problema utilizando métodos genericos mediante busqueda en anchura con *backtracking* y con propagación de restricciones.

En la propagación de restricciones, se define:
- Una serie de **variables** (en este caso, cada celda del sudoku)
- Un **dominio** de valores posibles para cada variable (del 1 al 9, menos los que se vayan eliminando a medida que se aplican restricciones)
- Una serie de **restricciones** entre variables (dos celdas no pueden ser iguales en la misma fila/columna/caja). 

Una **asignación** es un estado del problema con algunos o todos los valores asignados, tal que respeten las restricciones.

Todos las celdas del sudoku inicialmente pueden tener un valor en un dominio del 1 al 9. Se van eliminando del dominio de cada celda valores conforme se añaden números si la celda se encuentra en la misma fila, columna o caja de la celda a la que se ha introducido un número. Si solo tiene un valor en su dominio, entonces se inserta el número en la celda.

Para la propagación de restricciones, se puede aplicar las siguientes heurísticas:

- Valores restantes mínimos (MRV): Elige explorar en la variable con el menor dominio disponible. Si hay una variable con dominio vacío, detecta fallo. Trata de agotar los caminos que fallarán lo antes posible.
- Heurísitca de grados (*Degree*): Trata de elegir la variable involucrada en el mayor número de restricciones con otras variables. El objetivo es reducir el factor de ramificación para decisiones futuras. Se puede utilizar para resolver empates de MRV.
- Valor menos restringido (LCV): Una vez seleccionada una variable, elige un valor que provoque el menor número de descartes en el dominio de variables relacionadas por restricciones.
- Consistencia de arco (AC): Alternativa al *foward checking* (eliminar del dominio valores de una variable incompatibles \[inconsistentes\] al definir otra). Utiliza el grafo de restricciones entre variables para descartar rapidamente valores inconsistentes. Dada una restricción, si un valor en una variable tiene al menos un valor consistente en la otra variable, no se elimina.

### Comentarios

### Anexo: Traza de ejecución de SudokuApp 
