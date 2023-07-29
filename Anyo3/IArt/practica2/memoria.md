---
title: |
  | Inteligencia Artificial
  | Practica 1: Búsqueda informada
subtitle: "Memoria"
author: "Dorian Boleslaw Wozniak (817570@unizar.es)"
---

### Resultados obtenidos

```
h(1) == Manhattan
h(2) == Misplaced tile

------------------------------------------------------------------------------------------
||    ||      Nodos generados                  ||                   b*                  ||
------------------------------------------------------------------------------------------
||   d||    BFS  |    IDS  | A*h(1)  | A*h(2)  ||    BFS  |    IDS  | A*h(1)  | A*h(2)  ||
------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------
||   2||      8  |     10  |      5  |      5  ||   2.34  |   2.79  |   1.91  |   1.91  ||
||   3||     19  |     35  |      8  |      9  ||   2.27  |   2.85  |   1.62  |   1.70  ||
||   4||     37  |     96  |     11  |     12  ||   2.12  |   2.79  |   1.46  |   1.50  ||
||   5||     69  |    278  |     14  |     17  ||   2.05  |   2.81  |   1.37  |   1.45  ||
||   6||    126  |    810  |     18  |     23  ||   1.99  |   2.81  |   1.33  |   1.40  ||
||   7||    220  |   2253  |     23  |     34  ||   1.95  |   2.81  |   1.30  |   1.40  ||
||   8||    376  |   6479  |     28  |     49  ||   1.91  |   2.81  |   1.27  |   1.39  ||
||   9||    632  |  18116  |     33  |     70  ||   1.88  |   2.81  |   1.25  |   1.40  ||
||  10||   1025  |  51409  |     47  |    112  ||   1.85  |   2.82  |   1.26  |   1.42  ||
||  11||   1721  |      0  |     67  |    173  ||   1.83  |   0.00  |   1.27  |   1.43  ||
||  12||   2767  |      0  |     85  |    267  ||   1.81  |   0.00  |   1.27  |   1.44  ||
||  13||   4464  |      0  |    118  |    411  ||   1.79  |   0.00  |   1.27  |   1.45  ||
||  14||   7235  |      0  |    163  |    648  ||   1.78  |   0.00  |   1.28  |   1.46  ||
||  15||  11417  |      0  |    215  |    998  ||   1.76  |   0.00  |   1.28  |   1.47  ||
||  16||  17828  |      0  |    298  |   1479  ||   1.75  |   0.00  |   1.29  |   1.47  ||
||  17||  28054  |      0  |    381  |   2352  ||   1.74  |   0.00  |   1.29  |   1.48  ||
||  18||  42467  |      0  |    507  |   3606  ||   1.72  |   0.00  |   1.29  |   1.48  ||
||  19||  62846  |      0  |    748  |   5648  ||   1.71  |   0.00  |   1.30  |   1.48  ||
||  20||  92429  |      0  |   1034  |   9342  ||   1.69  |   0.00  |   1.31  |   1.49  ||
||  21|| 128374  |      0  |   1358  |  13140  ||   1.68  |   0.00  |   1.31  |   1.49  ||
||  22|| 174735  |      0  |   1796  |  21092  ||   1.66  |   0.00  |   1.30  |   1.50  ||
||  23|| 230437  |      0  |   2528  |  31377  ||   1.64  |   0.00  |   1.31  |   1.49  ||
||  24|| 290102  |      0  |   3071  |  47134  ||   1.62  |   0.00  |   1.31  |   1.50  ||
------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------
```

### Comentarios

Se ha evitado realizar IDS tras la produndiadad 10 por su crecimiento exponencial en tiempo al tener que ejecutar tantas iteraciones de DLS como sea su profundidad.

Tanto por número de nodos expandidos como por factor de ramificación efectivo, los mejores algoritmos son

1. Búsqueda A*, algoritmo de distancia Manhattan (Suma de diferencias de distancias entre el estado actual y el estado objetivo en ambos ejes)
2. Búsqueda A*, algoritmo de baldosas mal colocadas (Misplaced Tiles) (+1 si la baldosa está bien colocada)
3. Búsqueda en anchura (encuentra siempre la solución óptima, pero costes exponenciales)
4. Búsqueda iterativa en profundidad (encuentra profundidad perfecta para un DLS, pero costes exponenciales al tener que iterar por cada profundidad)

Se puede apreciar que las búsquedas A* tienen mejor rendimiento que las búsquedas en anchura, pero depende de la heurística aplicada. En este caso, la heurística de Manhattan está mejor informada que la de Misplaced Tiles.