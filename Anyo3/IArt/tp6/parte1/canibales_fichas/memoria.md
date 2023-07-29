---
title: |
  | Inteligencia Artificial
  | Trabajo TP6: Parte 1
subtitle: "Memoria"
author: "Dorian Boleslaw Wozniak (817570@unizar.es)"
---

# Problema de los canibales

### Descripción

Se quiere mover a un grupo de 3 misioneros y 3 canibales de una orilla a otra.
Para ello se dispone de una barca.

> RIBERA-IZQ M M M C C C BOTE --RIO --                  RIBERA-DCH

Sin embargo, hay una serie de condiciones que se deben de cumplir en todo momento:

- Debe haber en cada orilla tantos misioneros que canibales o más. En caso de que los misioneros fueran minoria, estos serían comidos por los canibales.
- Alternativamente, puede haber canibales si no hay misioneros en su orilla
- La barca puede transportar 2 personas en las siguientes combinaciones:
  - 1 misionero
  - 2 misioneros
  - 1 canibal
  - 2 canibales
  - 1 misionero y un canibal
- En ningún caso el bote puede viajar sin pasajeros. Se suben al bote personas de la misma orilla que el bote
- Si el bote esta en una orilla, tras un movimiento se encontrará en la orilla contraria

Se considera el problema resuelto una vez todas las personas se encuentren en la otra orilla:

> RIBERA-IZQ                  --RIO -- BOTE M M M C C C RIBERA-DCH

Para resolver el problema, se ha codificado el estado del problema como una tupla (MO, CO, MD, CD, LB), donde O representa la orilla origen, D la orilla destino, M a los misionarios, C a los canibales y LB al lado de la barca, según esté en la orilla origen o destino. Se han considerado las 5 acciones anteriormente descritas para el agente, donde realiza una acción diferente según el lado de la barca del estado suministrado, pero se podría considerar una solución de 10 acciones posibles si se trata la misma acción desde dos orillas diferentes como acciones separadas.



### Comentarios 

Se han tratado de resolver los problemas con los mismos algoritmos de búsqueda no informada vistos en la Práctica 1, tanto con búsqueda en grafo como en árbol.

El algoritmo BFS en grafo logra encontrar rápidamente la solución óptima en poco tiempo y expandiendo solo 13 nodos. La búsqueda en árbol, por otra parte, expande más de 5000. Esto se debe a que todo movimiento puede deshacerse, por lo que existen muchas estructuras repetidas que aumentan los nodos que se expanden innecesariamente. Por suerte, la solución no se encuentra a muchos pasos del estado origen y el problema de los canibales tiene relativamente pocos estados válidos posibles.

El algoritmo DFS en grafo tambien encuentra la solución óptima (11 pasos) pero no estaba garantizado de que encontrase dicha solución. El algoritmo en árbol no acaba, quedandose sin memoria al encontrarse con bucles por la posibilidad de revertir movimientos.

La búsqueda DLS a 11 pasos y el algoritmo IDLS ambas logran acabar encontrando la solución óptima. La búsqueda limitada solo encuentra la solución si la profundidad de búsqueda es igual o mayor que la solución menos profunda, y garantiza la solución óptima sólo si dicha profundidad de búsqueda es igual a la profundidad de esa solución. Por otro lado, el IDLS no sufre la subida exponencial de coste en tiempo al ser la solución relativamente sencilla.

El algoritmo UCS tiene un comportamiento parecido al BFS al no tener asociado los movimientos un coste, pero explorando mas nodos.

# Problema de las fichas

Se dispone de la siguiente tabla:

>+---+---+---+---+---+---+---+
>| B | B | B |   | V | V | V |
>+---+---+---+---+---+---+---+

Se desea obtener una tabla similar, pero con las fichas cambiadas de lado:

>+---+---+---+---+---+---+---+
>| V | V | V |   | B | B | B |
>+---+---+---+---+---+---+---+

Las reglas para mover una ficha son las siguiente:

- Una ficha se debe mover al hueco
- Una ficha puede saltar sobre máximo otras 2 fichas para ocupar el hueco
  - O dicho de otra manera, el hueco debe estar a 3 cuadros de su posición en la tabla
  - O, como se ha planteado al implementar el problema, el hueco se puede intercambiar con cualquier ficha a máximo 3 posiciones a la izquierda o a la derecha
- Las fichas evidentemente no pueden salir del tablero

Para ello, se ha definido como estado una 7-tupla que representa directamente el tablero. La solución implementada contiene 12 acciones:

- Mover una ficha B
  - A la izquierda
    - 1 casilla
    - 2 casillas
    - 3 casillas
  - A la derecha
    - 1 casilla
    - 2 casillas
    - 3 casillas
- Mover una ficha V
  - A la izquierda
    - 1 casilla
    - 2 casillas
    - 3 casillas
  - A la derecha
    - 1 casilla
    - 2 casillas
    - 3 casillas


### Descripción

### Comentarios

Respecto a los resultados del problema de los canibales, se diferencia en los siguientes aspectos:

La búsqueda en anchura en árbol expande mas de 500000 nodos en comparación de los 134 expandidos y tarda significativamente más tiempo de reloj para acabar. Al tener este problema acciones reversibles, más estados válidos posibles que el problema de los canibales, y más tipos de movimientos posibles definidos, el problema se encuentra al borde de quedarse sin memoria para resolver. Es mas, la búsqueda de coste uniforme en árbol acaba lanzando una excepción por falta de espacio debido a su uso mayor de memoria al expandir mas nodos de normal (aunque los movimientos no tengan un coste asociado). 

La búsqueda primero en profundidad esta vez no encuentra la solución óptima, devolviendo 24 pasos en vez de 10. Esto es un comportamiento esperable, y puede depender del orden de la expansión de los nodos en la implementación. La búsqueda en anchura tampoco acaba por razones similares a la anterior. Las búsquedas en profundidad limitadas e iterativa se comportan de forma similar y ambas acaban.

# ANEXO

### Anexo 1: Resultados: problema de los canibales

```
Misioneros y canibales BFS-G -->
pathCost : 11
nodesExpanded : 13
queueSize : 1
maxQueueSize : 3
Tiempo: 54

SOLUCIÓN:
GOAL STATE
RIBERA-IZQ M M M C C C BOTE --RIO --                  RIBERA-DCH
CAMINO ENCONTRADO
       ESTADO INCIAL    RIBERA-IZQ M M M C C C BOTE --RIO --                  RIBERA-DCH
   Action[name==M2C]    RIBERA-IZQ M M M     C      --RIO -- BOTE         C C RIBERA-DCH
   Action[name==M1C]    RIBERA-IZQ M M M   C C BOTE --RIO --                C RIBERA-DCH
   Action[name==M2C]    RIBERA-IZQ M M M            --RIO -- BOTE       C C C RIBERA-DCH
   Action[name==M1C]    RIBERA-IZQ M M M     C BOTE --RIO --              C C RIBERA-DCH
   Action[name==M2M]    RIBERA-IZQ     M     C      --RIO -- BOTE   M M   C C RIBERA-DCH
 Action[name==M1M1C]    RIBERA-IZQ   M M   C C BOTE --RIO --          M     C RIBERA-DCH
   Action[name==M2M]    RIBERA-IZQ         C C      --RIO -- BOTE M M M     C RIBERA-DCH
   Action[name==M1C]    RIBERA-IZQ       C C C BOTE --RIO --      M M M       RIBERA-DCH
   Action[name==M2C]    RIBERA-IZQ           C      --RIO -- BOTE M M M   C C RIBERA-DCH
   Action[name==M1M]    RIBERA-IZQ     M     C BOTE --RIO --        M M   C C RIBERA-DCH
 Action[name==M1M1C]    RIBERA-IZQ                  --RIO -- BOTE M M M C C C RIBERA-DCH

Misioneros y canibales BFS-T -->
pathCost : 11
nodesExpanded : 5124
queueSize : 6752
maxQueueSize : 6753
Tiempo: 153

SOLUCIÓN:
GOAL STATE
RIBERA-IZQ M M M C C C BOTE --RIO --                  RIBERA-DCH
CAMINO ENCONTRADO
       ESTADO INCIAL    RIBERA-IZQ M M M C C C BOTE --RIO --                  RIBERA-DCH
   Action[name==M2C]    RIBERA-IZQ M M M     C      --RIO -- BOTE         C C RIBERA-DCH
   Action[name==M1C]    RIBERA-IZQ M M M   C C BOTE --RIO --                C RIBERA-DCH
   Action[name==M2C]    RIBERA-IZQ M M M            --RIO -- BOTE       C C C RIBERA-DCH
   Action[name==M1C]    RIBERA-IZQ M M M     C BOTE --RIO --              C C RIBERA-DCH
   Action[name==M2M]    RIBERA-IZQ     M     C      --RIO -- BOTE   M M   C C RIBERA-DCH
 Action[name==M1M1C]    RIBERA-IZQ   M M   C C BOTE --RIO --          M     C RIBERA-DCH
   Action[name==M2M]    RIBERA-IZQ         C C      --RIO -- BOTE M M M     C RIBERA-DCH
   Action[name==M1C]    RIBERA-IZQ       C C C BOTE --RIO --      M M M       RIBERA-DCH
   Action[name==M2C]    RIBERA-IZQ           C      --RIO -- BOTE M M M   C C RIBERA-DCH
   Action[name==M1M]    RIBERA-IZQ     M     C BOTE --RIO --        M M   C C RIBERA-DCH
 Action[name==M1M1C]    RIBERA-IZQ                  --RIO -- BOTE M M M C C C RIBERA-DCH

Misioneros y canibales DFS-G -->
pathCost : 11
nodesExpanded : 11
queueSize : 3
maxQueueSize : 4
Tiempo: 9

SOLUCIÓN:
GOAL STATE
RIBERA-IZQ M M M C C C BOTE --RIO --                  RIBERA-DCH
CAMINO ENCONTRADO
       ESTADO INCIAL    RIBERA-IZQ M M M C C C BOTE --RIO --                  RIBERA-DCH
 Action[name==M1M1C]    RIBERA-IZQ   M M   C C      --RIO -- BOTE     M     C RIBERA-DCH
   Action[name==M1M]    RIBERA-IZQ M M M   C C BOTE --RIO --                C RIBERA-DCH
   Action[name==M2C]    RIBERA-IZQ M M M            --RIO -- BOTE       C C C RIBERA-DCH
   Action[name==M1C]    RIBERA-IZQ M M M     C BOTE --RIO --              C C RIBERA-DCH
   Action[name==M2M]    RIBERA-IZQ     M     C      --RIO -- BOTE   M M   C C RIBERA-DCH
 Action[name==M1M1C]    RIBERA-IZQ   M M   C C BOTE --RIO --          M     C RIBERA-DCH
   Action[name==M2M]    RIBERA-IZQ         C C      --RIO -- BOTE M M M     C RIBERA-DCH
   Action[name==M1C]    RIBERA-IZQ       C C C BOTE --RIO --      M M M       RIBERA-DCH
   Action[name==M2C]    RIBERA-IZQ           C      --RIO -- BOTE M M M   C C RIBERA-DCH
   Action[name==M1C]    RIBERA-IZQ         C C BOTE --RIO --      M M M     C RIBERA-DCH
   Action[name==M2C]    RIBERA-IZQ                  --RIO -- BOTE M M M C C C RIBERA-DCH

Misioneros y canibales DLS-11 -->
pathCost : 11
nodesExpanded : 2792
Tiempo: 68

SOLUCIÓN:
GOAL STATE
RIBERA-IZQ M M M C C C BOTE --RIO --                  RIBERA-DCH
CAMINO ENCONTRADO
       ESTADO INCIAL    RIBERA-IZQ M M M C C C BOTE --RIO --                  RIBERA-DCH
   Action[name==M2C]    RIBERA-IZQ M M M     C      --RIO -- BOTE         C C RIBERA-DCH
   Action[name==M1C]    RIBERA-IZQ M M M   C C BOTE --RIO --                C RIBERA-DCH
   Action[name==M2C]    RIBERA-IZQ M M M            --RIO -- BOTE       C C C RIBERA-DCH
   Action[name==M1C]    RIBERA-IZQ M M M     C BOTE --RIO --              C C RIBERA-DCH
   Action[name==M2M]    RIBERA-IZQ     M     C      --RIO -- BOTE   M M   C C RIBERA-DCH
 Action[name==M1M1C]    RIBERA-IZQ   M M   C C BOTE --RIO --          M     C RIBERA-DCH
   Action[name==M2M]    RIBERA-IZQ         C C      --RIO -- BOTE M M M     C RIBERA-DCH
   Action[name==M1C]    RIBERA-IZQ       C C C BOTE --RIO --      M M M       RIBERA-DCH
   Action[name==M2C]    RIBERA-IZQ           C      --RIO -- BOTE M M M   C C RIBERA-DCH
   Action[name==M1M]    RIBERA-IZQ     M     C BOTE --RIO --        M M   C C RIBERA-DCH
 Action[name==M1M1C]    RIBERA-IZQ                  --RIO -- BOTE M M M C C C RIBERA-DCH

Misioneros y canibales IDLS -->
pathCost : 11
nodesExpanded : 9097
Tiempo: 111

SOLUCIÓN:
GOAL STATE
RIBERA-IZQ M M M C C C BOTE --RIO --                  RIBERA-DCH
CAMINO ENCONTRADO
       ESTADO INCIAL    RIBERA-IZQ M M M C C C BOTE --RIO --                  RIBERA-DCH
   Action[name==M2C]    RIBERA-IZQ M M M     C      --RIO -- BOTE         C C RIBERA-DCH
   Action[name==M1C]    RIBERA-IZQ M M M   C C BOTE --RIO --                C RIBERA-DCH
   Action[name==M2C]    RIBERA-IZQ M M M            --RIO -- BOTE       C C C RIBERA-DCH
   Action[name==M1C]    RIBERA-IZQ M M M     C BOTE --RIO --              C C RIBERA-DCH
   Action[name==M2M]    RIBERA-IZQ     M     C      --RIO -- BOTE   M M   C C RIBERA-DCH
 Action[name==M1M1C]    RIBERA-IZQ   M M   C C BOTE --RIO --          M     C RIBERA-DCH
   Action[name==M2M]    RIBERA-IZQ         C C      --RIO -- BOTE M M M     C RIBERA-DCH
   Action[name==M1C]    RIBERA-IZQ       C C C BOTE --RIO --      M M M       RIBERA-DCH
   Action[name==M2C]    RIBERA-IZQ           C      --RIO -- BOTE M M M   C C RIBERA-DCH
   Action[name==M1M]    RIBERA-IZQ     M     C BOTE --RIO --        M M   C C RIBERA-DCH
 Action[name==M1M1C]    RIBERA-IZQ                  --RIO -- BOTE M M M C C C RIBERA-DCH

Misioneros y canibales UCS-G -->
pathCost : 11
nodesExpanded : 14
maxQueueSize : 3
Tiempo: 2

SOLUCIÓN:
GOAL STATE
RIBERA-IZQ M M M C C C BOTE --RIO --                  RIBERA-DCH
CAMINO ENCONTRADO
       ESTADO INCIAL    RIBERA-IZQ M M M C C C BOTE --RIO --                  RIBERA-DCH
 Action[name==M1M1C]    RIBERA-IZQ   M M   C C      --RIO -- BOTE     M     C RIBERA-DCH
   Action[name==M1M]    RIBERA-IZQ M M M   C C BOTE --RIO --                C RIBERA-DCH
   Action[name==M2C]    RIBERA-IZQ M M M            --RIO -- BOTE       C C C RIBERA-DCH
   Action[name==M1C]    RIBERA-IZQ M M M     C BOTE --RIO --              C C RIBERA-DCH
   Action[name==M2M]    RIBERA-IZQ     M     C      --RIO -- BOTE   M M   C C RIBERA-DCH
 Action[name==M1M1C]    RIBERA-IZQ   M M   C C BOTE --RIO --          M     C RIBERA-DCH
   Action[name==M2M]    RIBERA-IZQ         C C      --RIO -- BOTE M M M     C RIBERA-DCH
   Action[name==M1C]    RIBERA-IZQ       C C C BOTE --RIO --      M M M       RIBERA-DCH
   Action[name==M2C]    RIBERA-IZQ           C      --RIO -- BOTE M M M   C C RIBERA-DCH
   Action[name==M1M]    RIBERA-IZQ     M     C BOTE --RIO --        M M   C C RIBERA-DCH
 Action[name==M1M1C]    RIBERA-IZQ                  --RIO -- BOTE M M M C C C RIBERA-DCH

Misioneros y canibales UCS-T -->
pathCost : 11
nodesExpanded : 11127
queueSize : 14251
maxQueueSize : 14252
Tiempo: 175

SOLUCIÓN:
GOAL STATE
RIBERA-IZQ M M M C C C BOTE --RIO --                  RIBERA-DCH
CAMINO ENCONTRADO
       ESTADO INCIAL    RIBERA-IZQ M M M C C C BOTE --RIO --                  RIBERA-DCH
   Action[name==M2C]    RIBERA-IZQ M M M     C      --RIO -- BOTE         C C RIBERA-DCH
   Action[name==M1C]    RIBERA-IZQ M M M   C C BOTE --RIO --                C RIBERA-DCH
   Action[name==M2C]    RIBERA-IZQ M M M            --RIO -- BOTE       C C C RIBERA-DCH
   Action[name==M1C]    RIBERA-IZQ M M M     C BOTE --RIO --              C C RIBERA-DCH
   Action[name==M2M]    RIBERA-IZQ     M     C      --RIO -- BOTE   M M   C C RIBERA-DCH
 Action[name==M1M1C]    RIBERA-IZQ   M M   C C BOTE --RIO --          M     C RIBERA-DCH
   Action[name==M2M]    RIBERA-IZQ         C C      --RIO -- BOTE M M M     C RIBERA-DCH
   Action[name==M1C]    RIBERA-IZQ       C C C BOTE --RIO --      M M M       RIBERA-DCH
   Action[name==M2C]    RIBERA-IZQ           C      --RIO -- BOTE M M M   C C RIBERA-DCH
   Action[name==M1M]    RIBERA-IZQ     M     C BOTE --RIO --        M M   C C RIBERA-DCH
 Action[name==M1M1C]    RIBERA-IZQ                  --RIO -- BOTE M M M C C C RIBERA-DCH
```

### Anexo 2: Resultados: problema de los canibales


```
Problema de las fichas BFS-G -->
pathCost : 10
nodesExpanded : 134
queueSize : 3
maxQueueSize : 27
Tiempo: 55

SOLUCIÓN:
GOAL STATE
+---+---+---+---+---+---+---+
| B | B | B |   | V | V | V |
+---+---+---+---+---+---+---+

CAMINO ENCONTRADO
ESTADO INCIAL
+---+---+---+---+---+---+---+
| B | B | B |   | V | V | V |
+---+---+---+---+---+---+---+

Action[name==MB2D]
+---+---+---+---+---+---+---+
| B |   | B | B | V | V | V |
+---+---+---+---+---+---+---+

Action[name==MV3I]
+---+---+---+---+---+---+---+
| B | V | B | B |   | V | V |
+---+---+---+---+---+---+---+

Action[name==MB2D]
+---+---+---+---+---+---+---+
| B | V |   | B | B | V | V |
+---+---+---+---+---+---+---+

Action[name==MV3I]
+---+---+---+---+---+---+---+
| B | V | V | B | B |   | V |
+---+---+---+---+---+---+---+

Action[name==MV1I]
+---+---+---+---+---+---+---+
| B | V | V | B | B | V |   |
+---+---+---+---+---+---+---+

Action[name==MB3D]
+---+---+---+---+---+---+---+
| B | V | V |   | B | V | B |
+---+---+---+---+---+---+---+

Action[name==MB3D]
+---+---+---+---+---+---+---+
|   | V | V | B | B | V | B |
+---+---+---+---+---+---+---+

Action[name==MV2I]
+---+---+---+---+---+---+---+
| V | V |   | B | B | V | B |
+---+---+---+---+---+---+---+

Action[name==MV3I]
+---+---+---+---+---+---+---+
| V | V | V | B | B |   | B |
+---+---+---+---+---+---+---+

Action[name==MB2D]
+---+---+---+---+---+---+---+
| V | V | V |   | B | B | B |
+---+---+---+---+---+---+---+

Problema de las fichas BFS-T -->
pathCost : 10
nodesExpanded : 560385
queueSize : 1947453
maxQueueSize : 1947454
Tiempo: 4999

SOLUCIÓN:
GOAL STATE
+---+---+---+---+---+---+---+
| B | B | B |   | V | V | V |
+---+---+---+---+---+---+---+

CAMINO ENCONTRADO
ESTADO INCIAL
+---+---+---+---+---+---+---+
| B | B | B |   | V | V | V |
+---+---+---+---+---+---+---+

Action[name==MB2D]
+---+---+---+---+---+---+---+
| B |   | B | B | V | V | V |
+---+---+---+---+---+---+---+

Action[name==MV3I]
+---+---+---+---+---+---+---+
| B | V | B | B |   | V | V |
+---+---+---+---+---+---+---+

Action[name==MB2D]
+---+---+---+---+---+---+---+
| B | V |   | B | B | V | V |
+---+---+---+---+---+---+---+

Action[name==MV3I]
+---+---+---+---+---+---+---+
| B | V | V | B | B |   | V |
+---+---+---+---+---+---+---+

Action[name==MV1I]
+---+---+---+---+---+---+---+
| B | V | V | B | B | V |   |
+---+---+---+---+---+---+---+

Action[name==MB3D]
+---+---+---+---+---+---+---+
| B | V | V |   | B | V | B |
+---+---+---+---+---+---+---+

Action[name==MB3D]
+---+---+---+---+---+---+---+
|   | V | V | B | B | V | B |
+---+---+---+---+---+---+---+

Action[name==MV2I]
+---+---+---+---+---+---+---+
| V | V |   | B | B | V | B |
+---+---+---+---+---+---+---+

Action[name==MV3I]
+---+---+---+---+---+---+---+
| V | V | V | B | B |   | B |
+---+---+---+---+---+---+---+

Action[name==MB2D]
+---+---+---+---+---+---+---+
| V | V | V |   | B | B | B |
+---+---+---+---+---+---+---+

Problema de las fichas DFS-G -->
pathCost : 24
nodesExpanded : 28
queueSize : 38
maxQueueSize : 39
Tiempo: 5

SOLUCIÓN:
GOAL STATE
+---+---+---+---+---+---+---+
| B | B | B |   | V | V | V |
+---+---+---+---+---+---+---+

CAMINO ENCONTRADO
ESTADO INCIAL
+---+---+---+---+---+---+---+
| B | B | B |   | V | V | V |
+---+---+---+---+---+---+---+

Action[name==MV2I]
+---+---+---+---+---+---+---+
| B | B | B | V | V |   | V |
+---+---+---+---+---+---+---+

Action[name==MB3D]
+---+---+---+---+---+---+---+
| B | B |   | V | V | B | V |
+---+---+---+---+---+---+---+

Action[name==MV2I]
+---+---+---+---+---+---+---+
| B | B | V | V |   | B | V |
+---+---+---+---+---+---+---+

Action[name==MV2I]
+---+---+---+---+---+---+---+
| B | B | V | V | V | B |   |
+---+---+---+---+---+---+---+

Action[name==MB1D]
+---+---+---+---+---+---+---+
| B | B | V | V | V |   | B |
+---+---+---+---+---+---+---+

Action[name==MV3D]
+---+---+---+---+---+---+---+
| B | B |   | V | V | V | B |
+---+---+---+---+---+---+---+

Action[name==MB2D]
+---+---+---+---+---+---+---+
|   | B | B | V | V | V | B |
+---+---+---+---+---+---+---+

Action[name==MV3I]
+---+---+---+---+---+---+---+
| V | B | B |   | V | V | B |
+---+---+---+---+---+---+---+

Action[name==MV2I]
+---+---+---+---+---+---+---+
| V | B | B | V | V |   | B |
+---+---+---+---+---+---+---+

Action[name==MB3D]
+---+---+---+---+---+---+---+
| V | B |   | V | V | B | B |
+---+---+---+---+---+---+---+

Action[name==MV2D]
+---+---+---+---+---+---+---+
|   | B | V | V | V | B | B |
+---+---+---+---+---+---+---+

Action[name==MB1I]
+---+---+---+---+---+---+---+
| B |   | V | V | V | B | B |
+---+---+---+---+---+---+---+

Action[name==MV3I]
+---+---+---+---+---+---+---+
| B | V | V | V |   | B | B |
+---+---+---+---+---+---+---+

Action[name==MB2I]
+---+---+---+---+---+---+---+
| B | V | V | V | B | B |   |
+---+---+---+---+---+---+---+

Action[name==MV3D]
+---+---+---+---+---+---+---+
| B | V | V |   | B | B | V |
+---+---+---+---+---+---+---+

Action[name==MV1D]
+---+---+---+---+---+---+---+
| B | V |   | V | B | B | V |
+---+---+---+---+---+---+---+

Action[name==MB2D]
+---+---+---+---+---+---+---+
|   | V | B | V | B | B | V |
+---+---+---+---+---+---+---+

Action[name==MV3I]
+---+---+---+---+---+---+---+
| V | V | B |   | B | B | V |
+---+---+---+---+---+---+---+

Action[name==MV3I]
+---+---+---+---+---+---+---+
| V | V | B | V | B | B |   |
+---+---+---+---+---+---+---+

Action[name==MB2D]
+---+---+---+---+---+---+---+
| V | V | B | V |   | B | B |
+---+---+---+---+---+---+---+

Action[name==MV1D]
+---+---+---+---+---+---+---+
| V | V | B |   | V | B | B |
+---+---+---+---+---+---+---+

Action[name==MB1D]
+---+---+---+---+---+---+---+
| V | V |   | B | V | B | B |
+---+---+---+---+---+---+---+

Action[name==MV2I]
+---+---+---+---+---+---+---+
| V | V | V | B |   | B | B |
+---+---+---+---+---+---+---+

Action[name==MB1D]
+---+---+---+---+---+---+---+
| V | V | V |   | B | B | B |
+---+---+---+---+---+---+---+

Problema de las fichas DLS-10 -->
pathCost : 10
nodesExpanded : 391851
Tiempo: 2175

SOLUCIÓN:
GOAL STATE
+---+---+---+---+---+---+---+
| B | B | B |   | V | V | V |
+---+---+---+---+---+---+---+

CAMINO ENCONTRADO
ESTADO INCIAL
+---+---+---+---+---+---+---+
| B | B | B |   | V | V | V |
+---+---+---+---+---+---+---+

Action[name==MB2D]
+---+---+---+---+---+---+---+
| B |   | B | B | V | V | V |
+---+---+---+---+---+---+---+

Action[name==MV3I]
+---+---+---+---+---+---+---+
| B | V | B | B |   | V | V |
+---+---+---+---+---+---+---+

Action[name==MB2D]
+---+---+---+---+---+---+---+
| B | V |   | B | B | V | V |
+---+---+---+---+---+---+---+

Action[name==MV3I]
+---+---+---+---+---+---+---+
| B | V | V | B | B |   | V |
+---+---+---+---+---+---+---+

Action[name==MV1I]
+---+---+---+---+---+---+---+
| B | V | V | B | B | V |   |
+---+---+---+---+---+---+---+

Action[name==MB3D]
+---+---+---+---+---+---+---+
| B | V | V |   | B | V | B |
+---+---+---+---+---+---+---+

Action[name==MB3D]
+---+---+---+---+---+---+---+
|   | V | V | B | B | V | B |
+---+---+---+---+---+---+---+

Action[name==MV2I]
+---+---+---+---+---+---+---+
| V | V |   | B | B | V | B |
+---+---+---+---+---+---+---+

Action[name==MV3I]
+---+---+---+---+---+---+---+
| V | V | V | B | B |   | B |
+---+---+---+---+---+---+---+

Action[name==MB2D]
+---+---+---+---+---+---+---+
| V | V | V |   | B | B | B |
+---+---+---+---+---+---+---+

Problema de las fichas IDLS -->
pathCost : 10
nodesExpanded : 721646
Tiempo: 3584

SOLUCIÓN:
GOAL STATE
+---+---+---+---+---+---+---+
| B | B | B |   | V | V | V |
+---+---+---+---+---+---+---+

CAMINO ENCONTRADO
ESTADO INCIAL
+---+---+---+---+---+---+---+
| B | B | B |   | V | V | V |
+---+---+---+---+---+---+---+

Action[name==MB2D]
+---+---+---+---+---+---+---+
| B |   | B | B | V | V | V |
+---+---+---+---+---+---+---+

Action[name==MV3I]
+---+---+---+---+---+---+---+
| B | V | B | B |   | V | V |
+---+---+---+---+---+---+---+

Action[name==MB2D]
+---+---+---+---+---+---+---+
| B | V |   | B | B | V | V |
+---+---+---+---+---+---+---+

Action[name==MV3I]
+---+---+---+---+---+---+---+
| B | V | V | B | B |   | V |
+---+---+---+---+---+---+---+

Action[name==MV1I]
+---+---+---+---+---+---+---+
| B | V | V | B | B | V |   |
+---+---+---+---+---+---+---+

Action[name==MB3D]
+---+---+---+---+---+---+---+
| B | V | V |   | B | V | B |
+---+---+---+---+---+---+---+

Action[name==MB3D]
+---+---+---+---+---+---+---+
|   | V | V | B | B | V | B |
+---+---+---+---+---+---+---+

Action[name==MV2I]
+---+---+---+---+---+---+---+
| V | V |   | B | B | V | B |
+---+---+---+---+---+---+---+

Action[name==MV3I]
+---+---+---+---+---+---+---+
| V | V | V | B | B |   | B |
+---+---+---+---+---+---+---+

Action[name==MB2D]
+---+---+---+---+---+---+---+
| V | V | V |   | B | B | B |
+---+---+---+---+---+---+---+

Problema de las fichas UCS-G -->
pathCost : 10
nodesExpanded : 136
queueSize : 3
maxQueueSize : 30
Tiempo: 6

SOLUCIÓN:
GOAL STATE
+---+---+---+---+---+---+---+
| B | B | B |   | V | V | V |
+---+---+---+---+---+---+---+

CAMINO ENCONTRADO
ESTADO INCIAL
+---+---+---+---+---+---+---+
| B | B | B |   | V | V | V |
+---+---+---+---+---+---+---+

Action[name==MB2D]
+---+---+---+---+---+---+---+
| B |   | B | B | V | V | V |
+---+---+---+---+---+---+---+

Action[name==MV3I]
+---+---+---+---+---+---+---+
| B | V | B | B |   | V | V |
+---+---+---+---+---+---+---+

Action[name==MB2D]
+---+---+---+---+---+---+---+
| B | V |   | B | B | V | V |
+---+---+---+---+---+---+---+

Action[name==MV3I]
+---+---+---+---+---+---+---+
| B | V | V | B | B |   | V |
+---+---+---+---+---+---+---+

Action[name==MV1I]
+---+---+---+---+---+---+---+
| B | V | V | B | B | V |   |
+---+---+---+---+---+---+---+

Action[name==MB3D]
+---+---+---+---+---+---+---+
| B | V | V |   | B | V | B |
+---+---+---+---+---+---+---+

Action[name==MB3D]
+---+---+---+---+---+---+---+
|   | V | V | B | B | V | B |
+---+---+---+---+---+---+---+

Action[name==MV2I]
+---+---+---+---+---+---+---+
| V | V |   | B | B | V | B |
+---+---+---+---+---+---+---+

Action[name==MV3I]
+---+---+---+---+---+---+---+
| V | V | V | B | B |   | B |
+---+---+---+---+---+---+---+

Action[name==MB2D]
+---+---+---+---+---+---+---+
| V | V | V |   | B | B | B |
+---+---+---+---+---+---+---+
```