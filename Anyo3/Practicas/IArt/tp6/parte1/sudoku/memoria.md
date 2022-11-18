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

Usando *ImprovedBacktrackingStategy* con todos los parámetros en falso (es decir, realizando una búsqueda en anchura), cada sudoku toma una cantidad de tiempo poco razonable para ser resuelto (y la ejecución suele "atascarse" en algunos de los sudokus). Por otro lado, poniendo los parámetros a true se aprecia una mejora substancial en el rendimiento, resolviendo sudokus entre unos 0,02 segundos y 3 segundos para casos mas complicados.


# Anexo: Traza de ejecución de SudokuApp (recortada)

```
+++++++++
---------
......8.1
6..2.....
...7.5...
...6...2.
.1....3..
.8.......
2......7.
.4..8....
5...3....
SUDOKU INCOMPLETO - Resolviendo
Time to solve: 2.013
SOLUCIÓN:
724369851
651248793
893715642
375691428
912874365
486523917
238456179
147982536
569137284
Sudoku solucionado correctamente
+++++++++
---------
.2.......
...6....3
.74.8....
.....3..2
.8..4..1.
6..5.....
....1.78.
5....9...
.......4.
SUDOKU INCOMPLETO - Resolviendo
Time to solve: 0.342
SOLUCIÓN:
126437958
895621473
374985126
457193862
983246517
612578394
269314785
548769231
731852649
Sudoku solucionado correctamente
+++++++++
---------
.52..68..
.....7.2.
......6..
..48..9..
2..41....
..1.....8
..61..38.
....9...6
3..6..1.9
SUDOKU INCOMPLETO - Resolviendo
Time to solve: 0.055
SOLUCIÓN:
152946837
963587421
847231695
574863912
289415763
631729548
796152384
415398276
328674159
Sudoku solucionado correctamente
+++++++++
---------
....1.78.
5....9...
.......4.
.2.......
...6....3
.74.8....
.....3..2
.8..4..1.
6..5.....
SUDOKU INCOMPLETO - Resolviendo
Time to solve: 0.295
SOLUCIÓN:
269314785
548769231
731852649
126437958
895621473
374985126
457193862
983246517
612578394
Sudoku solucionado correctamente
+++++++++
---------
1.......3
.6.3..7..
.7...5..1
21.7...9.
..7......
..8.1..2.
...8.64..
..9.2..6.
...4.....
SUDOKU INCOMPLETO - Resolviendo
Time to solve: 0.225
SOLUCIÓN:
152678943
864391752
973245681
215763894
497582136
638914527
321856479
549127368
786439215
Sudoku solucionado correctamente
+++++++++
---------
4...7.1..
..19.46.5
.....1...
...7....2
..2.3....
847..6...
.14...8.6
.2....3..
6...9....
SUDOKU INCOMPLETO - Resolviendo
Time to solve: 0.094
SOLUCIÓN:
496573128
381924675
275861943
153789462
962435781
847216539
714352896
529648317
638197254
Sudoku solucionado correctamente
+++++++++
---------
......8.1
7..2.....
...5.6...
...7...5.
.1....3..
.8.......
5......2.
.3..8....
6...4....
SUDOKU INCOMPLETO - Resolviendo
Time to solve: 1.59
SOLUCIÓN:
253479861
761238594
894516732
326791458
915824376
487653219
548167923
132985647
679342185
Sudoku solucionado correctamente
+++++++++
---------
963......
1....8...
...2.5...
.4.8.....
.1....7..
....3..25
7......3.
..9.2.4.7
......9..
SUDOKU INCOMPLETO - Resolviendo
Time to solve: 0.077
SOLUCIÓN:
963741258
152398674
874265391
345872169
218956743
697134825
721489536
589623417
436517982
Sudoku solucionado correctamente
+++++++++
---------
15.3.....
.7..4.2..
..4.72...
..8......
...9..1.8
.1..8.79.
.....38..
.........
6....7423
SUDOKU INCOMPLETO - Resolviendo
Time to solve: 0.196
SOLUCIÓN:
152398647
973641285
864572931
598714362
247936158
316285794
725463819
431829576
689157423
Sudoku solucionado correctamente
+++++++++
---------
.........
.5724...9
8....947.
..9..3...
5..9..12.
..3.1.9..
.6....25.
...56....
.7......6
SUDOKU INCOMPLETO - Resolviendo
Time to solve: 0.032
SOLUCIÓN:
946731582
157248639
832659471
719423865
584976123
623815947
461397258
398562714
275184396
Sudoku solucionado correctamente
+++++++++
---------
....75...
.1..2....
.4...3...
5.....3.2
...8...1.
......6..
...1..48.
2........
7........
SUDOKU INCOMPLETO - Resolviendo
Time to solve: 0.272
SOLUCIÓN:
932475861
617928534
845613279
568741392
429836715
173259648
356192487
294387156
781564923
Sudoku solucionado correctamente
+++++++++
---------
6.....7.3
.4.8.....
.........
...5.4.8.
7..2.....
1.3......
.2.....5.
....7.9..
....1....
SUDOKU INCOMPLETO - Resolviendo
Time to solve: 0.303
SOLUCIÓN:
618459723
342867519
579123468
296534187
784291635
153786294
927648351
861375942
435912876
Sudoku solucionado correctamente
+++++++++
---------
....6...4
..6.3....
1..4..5.7
7.....8.5
...8.....
6.8....9.
..2.9....
4....32..
..97..1..
SUDOKU INCOMPLETO - Resolviendo
Time to solve: 0.033
SOLUCIÓN:
957261384
846537921
123489567
734926815
295814736
618375492
572198643
481653279
369742158
Sudoku solucionado correctamente
+++++++++
---------
.32.....5
8..3.....
9.428...1
...4...39
...6...5.
....1....
.2...67.8
.....4...
.95....6.
SUDOKU INCOMPLETO - Resolviendo
Time to solve: 0.09
SOLUCIÓN:
132749685
857361924
964285371
216457839
348692157
579813246
421536798
683974512
795128463
Sudoku solucionado correctamente
+++++++++
---------
...5.3...
....6.7..
5.8....16
36..2....
...4.1...
....3...5
67....2.8
..4.7....
...2..5..
SUDOKU INCOMPLETO - Resolviendo
Time to solve: 0.168
SOLUCIÓN:
746513892
132869754
598742316
367925481
925481673
481637925
679154238
254378169
813296547
Sudoku solucionado correctamente
+++++++++
---------
.5.3.7.4.
1........
.3.......
5.8.3.61.
...8..5.9
.6..1....
....4...6
...6927..
..2...9..
SUDOKU INCOMPLETO - Resolviendo
Time to solve: 0.22
SOLUCIÓN:
956327841
127486395
834951267
548739612
271864539
369215478
793548126
415692783
682173954
Sudoku solucionado correctamente
+++++++++
---------
..5..8..1
8......9.
......78.
...4.....
64....9..
....53..2
.6.......
..138..5.
...9.714.
SUDOKU INCOMPLETO - Resolviendo
Time to solve: 0.174
SOLUCIÓN:
935748621
876231594
124695783
512469378
643872915
789153462
267514839
491386257
358927146
Sudoku solucionado correctamente
+++++++++
---------
.........
.72.6.1..
..51...82
.8...13..
4........
.37.9..1.
....238..
5.4..9...
......79.
SUDOKU INCOMPLETO - Resolviendo
Time to solve: 0.08
SOLUCIÓN:
143258679
872964153
695137482
986541327
451372968
237896514
719623845
564789231
328415796
Sudoku solucionado correctamente
+++++++++
---------
...658...
..4......
12.......
.....96.7
...3..5..
..2.8...3
..19..8..
3.6.....4
....473..
SUDOKU INCOMPLETO - Resolviendo
Time to solve: 0.111
SOLUCIÓN:
937658241
864291735
125734986
583419627
649372518
712586493
471963852
396825174
258147369
Sudoku solucionado correctamente
+++++++++
---------
.2.3.....
..6..8.9.
83.5.....
...2...8.
7.9..5...
.....6..4
.......1.
..1...4.2
2..7..8.9
SUDOKU INCOMPLETO - Resolviendo
Time to solve: 0.105
SOLUCIÓN:
924361758
156478293
837592641
613247985
749185326
582936174
498623517
371859462
265714839
Sudoku solucionado correctamente
+++++++++
---------
.5..9....
1.....6..
...3.8...
..8.4...9
514......
.3....2..
........4
.8...6..7
7..15..6.
SUDOKU INCOMPLETO - Resolviendo
Time to solve: 0.083
SOLUCIÓN:
856491372
143572698
927368451
278645139
514923786
639817245
361789524
485236917
792154863
Sudoku solucionado correctamente
+++++++++
---------
.....2...
....7...1
7..3...9.
8..7.....
.2.89.6..
.13..6...
.9..5.824
.....891.
.........
SUDOKU INCOMPLETO - Resolviendo
Time to solve: 0.046
SOLUCIÓN:
659412378
238679451
741385296
865723149
427891635
913546782
396157824
574268913
182934567
Sudoku solucionado correctamente
+++++++++
---------
3...8....
...7....5
1........
......36.
..2..4...
.7.......
....6.13.
.452.....
......8..
SUDOKU INCOMPLETO - Resolviendo
Time to solve: 0.36
SOLUCIÓN:
354186927
298743615
167952483
481527369
932614578
576398241
729865134
845231796
613479852
Sudoku solucionado correctamente
+++++++++
---------
85...24..
72......9
..4......
...1.7..2
3.5...9..
.4.......
....8..7.
.17......
....36.4.
SUDOKU INCOMPLETO - Resolviendo
Time to solve: 0.11
SOLUCIÓN:
859612437
723854169
164379528
986147352
375268914
241593786
432981675
617425893
598736241
Sudoku solucionado correctamente
+++++++++
---------
..53.....
8......2.
.7..1.5..
4....53..
.1..7...6
..32...8.
.6.5....9
..4....3.
.....97..
SUDOKU INCOMPLETO - Resolviendo
Time to solve: 0.065
SOLUCIÓN:
145327698
839654127
672918543
496185372
218473956
753296481
367542819
984761235
521839764
Sudoku solucionado correctamente
+++++++++
---------
12..4....
..5.69.1.
..9...5..
.......7.
7...52.9.
.3......2
.9.6...5.
4..9..8.1
..3...9.4
SUDOKU INCOMPLETO - Resolviendo
Time to solve: 0.058
SOLUCIÓN:
128547639
345869217
679213548
912486375
784352196
536791482
891624753
467935821
253178964
Sudoku solucionado correctamente
+++++++++
---------
...57..3.
1......2.
7...234..
....8...4
..7..4...
49....6.5
.42...3..
...7..9..
..18.....
SUDOKU INCOMPLETO - Resolviendo
Time to solve: 0.081
SOLUCIÓN:
624578139
135496827
789123456
216385794
857964213
493217685
942651378
568732941
371849562
Sudoku solucionado correctamente
+++++++++
---------
7..1523..
......92.
...3.....
1....47.8
.......6.
.........
..9...5.6
.4.9.7...
8....6.1.
SUDOKU INCOMPLETO - Resolviendo
Time to solve: 0.026
SOLUCIÓN:
796152384
531468927
428379651
152634798
384791265
967285143
219843576
645917832
873526419
Sudoku solucionado correctamente
+++++++++
---------
1....7.9.
.3..2...8
..96..5..
..53..9..
.1..8...2
6....4...
3......1.
.4......7
..7...3..
SUDOKU INCOMPLETO - Resolviendo
Time to solve: 0.166
SOLUCIÓN:
162857493
534129678
789643521
475312986
913586742
628794135
356478219
241935867
897261354
Sudoku solucionado correctamente
+++++++++
---------
1...34.8.
...8..5..
..4.6..21
.18......
3..1.2..6
......81.
52..7.9..
..6..9...
.9.64...2
SUDOKU INCOMPLETO - Resolviendo
Time to solve: 0.024
SOLUCIÓN:
152934687
763821549
984567321
618493275
375182496
249756813
521378964
436219758
897645132
Sudoku solucionado correctamente
+++++++++
---------
...92....
..68.3...
19..7...6
23..4.1..
..1...7..
..8.3..29
7...8..91
...5.72..
....64...
SUDOKU INCOMPLETO - Resolviendo
Time to solve: 0.023
SOLUCIÓN:
387926415
546813972
192475836
235749168
961258743
478631529
754382691
613597284
829164357
Sudoku solucionado correctamente
+++++++++
---------
.6.5.4.3.
1...9...8
.........
9...5...6
.4.6.2.7.
7...4...5
.........
4...8...1
.5.2.3.4.
SUDOKU INCOMPLETO - Resolviendo
Time to solve: 0.059
SOLUCIÓN:
869574132
124396758
375128694
932857416
541632879
786941325
217469583
493785261
658213947
Sudoku solucionado correctamente
+++++++++
---------
7.....4..
.2..7..8.
..3..8.79
9..5..3..
.6..2..9.
..1.97..6
...3..9..
.3..4..6.
..9..1.35
SUDOKU INCOMPLETO - Resolviendo
Time to solve: 0.033
SOLUCIÓN:
798635421
126974583
453218679
972586314
564123897
381497256
617352948
835749162
249861735
Sudoku solucionado correctamente
+++++++++
---------
....7..2.
8.......6
.1.2.5...
9.54....8
.........
3....85.1
...3.2.8.
4.......9
.7..6....
SUDOKU INCOMPLETO - Resolviendo
Time to solve: 0.104
SOLUCIÓN:
594876123
823914756
617235894
965421378
781653942
342798561
159342687
436587219
278169435
Sudoku solucionado correctamente
+++++++++
Número sudokus solucionados: 156
```