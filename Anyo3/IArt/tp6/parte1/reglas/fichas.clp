; Ingeniería artifical
; Trabajo TP6 - parte 1
;
; Fichero: fichas.clp
;
; Autor: Dorian Boleslaw Wozniak (817570@unizar.es)
;
; Descripción:
;   Implementación en CLIPS del problema de las fichas utilizando el 
;   algoritmo A*

; ------------------------------------------------------------------------------
; MAIN

(defmodule MAIN
    (export deftemplate nodo)
    (export deffunction heuristica)
)

; Un nodo del problema
(deftemplate MAIN::nodo
    (multislot estado)              ; Estado actual del tablero
    (multislot camino)              ; Estados recorridos
    (slot heuristica)               ; Valor de funcion heuristica
    (slot coste (default 0))        ; Coste para llegar al nodo
    (slot clase (default abierto))  ; Nodo expandido 
)

(defglobal MAIN
    ?*estado-inicial*   =  (create$ B B B H V V V)
    ?*estado-final*     =  (create$ V V V H B B B)
)

; Heurística: número de fichas mal colocadas
; Devuelve el número de fichas que no estan en su sitio respecto al estado final
; 0 = todas en su sitio, 7 = ninguna en su sitio 
(deffunction MAIN::heuristica ($?estado)
    (bind ?res 0)

    ; Suma 1 por cada slot que no coincide con el estado final
    (loop-for-count (?i 1 7)
        (if (neq (nth$ ?i $?estado)(nth$ ?i $?*estado-final*))
            then (bind ?res (+ ?res 1))
        )
    )

    ?res
)

; Algotimo A* en grafo
; Elige el nodo no explorado cuya función heurística sea la menor
(defrule MAIN::A-star-cerrar-estado

    ?nodo <- (nodo
        (heuristica ?h1)
        (coste ?c1)
        (clase abierto)
    )

    (not (nodo
        (heuristica ?h2)
        (coste ?c2&: (< (+ ?c2 ?h2) (+ ?c1 ?h1)))
        (clase abierto)
    ))
    
    ; Cierra el nodo y explora los caminos posibles
    => 
    (modify ?nodo (clase cerrado))
    (focus OPERADORES)
)

; Declaración estado inicial
(deffacts nodoInicial
    (nodo
        (estado ?*estado-inicial*)
        (camino)
        (heuristica (heuristica ?*estado-inicial*))
        (clase abierto)
    )
)

; ------------------------------------------------------------------------------
; OPERADORES

(defmodule OPERADORES
    (import MAIN deftemplate nodo)
    (import MAIN deffunction heuristica)
)

; Mueve una ficha a un hueco a su izquierda
(defrule OPERADORES::mover-1-izda
    (nodo
        (estado $?pre H ?x $?suf)
        (camino $?movimientos)
        (clase cerrado)
        (coste ?coste)
    )

    => 
    (bind $?nuevo-estado (create$ $?pre ?x H $?suf))
    (assert (nodo 
        (estado $?nuevo-estado)
        (camino $?movimientos $?nuevo-estado)
        (coste (+ ?coste 1))
        (heuristica (heuristica $?nuevo-estado))
    ))
)

; Mueve una ficha a un hueco a su derecha
(defrule OPERADORES::mover-1-dcha
    (nodo
        (estado $?pre ?x H $?suf)
        (camino $?movimientos )
        (clase cerrado)
        (coste ?coste)
    )

    =>
    (bind $?nuevo-estado (create$ $?pre H ?x $?suf))
    (assert (nodo 
        (estado $?nuevo-estado)
        (camino $?movimientos $?nuevo-estado)
        (coste (+ ?coste 1))
        (heuristica (heuristica $?nuevo-estado))
    ))
)

; Mueve una ficha a un hueco a su izquierda saltando una ficha
(defrule OPERADORES::mover-2-izda
    (nodo
        (estado $?pre H ?a ?x $?suf)
        (camino $?movimientos)
        (clase cerrado)
        (coste ?coste)
    )
    
    => 
    (bind $?nuevo-estado (create$ $?pre ?x ?a H $?suf))
    (assert (nodo 
        (estado $?nuevo-estado)
        (camino $?movimientos $?nuevo-estado)
        (coste (+ ?coste 1))
        (heuristica (heuristica $?nuevo-estado))
    ))
)

; Mueve una ficha a un hueco a su derecja saltando una ficha
(defrule OPERADORES::mover-2-dcha
    (nodo
        (estado $?pre ?x ?a H $?suf)
        (camino $?movimientos)
        (clase cerrado)
        (coste ?coste)
    )

    =>
    (bind $?nuevo-estado (create$ $?pre H ?a ?x $?suf))
    (assert (nodo 
        (estado $?nuevo-estado)
        (camino $?movimientos $?nuevo-estado)
        (coste (+ ?coste 1))
        (heuristica (heuristica $?nuevo-estado))
    ))
)

; Mueve una ficha a un hueco a su izquierda saltando dos fichas
(defrule OPERADORES::mover-3-izda
    (nodo
        (estado $?pre H ?a ?b ?x $?suf)
        (camino $?movimientos)
        (clase cerrado)
        (coste ?coste)
    )
    =>
    (bind $?nuevo-estado (create$ $?pre ?x ?a ?b H $?suf))
    (assert (nodo 
        (estado $?nuevo-estado)
        (camino $?movimientos $?nuevo-estado)
        (coste (+ ?coste 1))
        (heuristica (heuristica $?nuevo-estado))
    ))
)

; Mueve una ficha a un hueco a su derecha saltando dos fichas
(defrule OPERADORES::mover-3-dcha
    (nodo
        (estado $?pre ?x ?a ?b H $?suf)
        (camino $?movimientos)
        (clase cerrado)
        (coste ?coste)
    )

    =>
    (bind $?nuevo-estado (create$ $?pre H ?a ?b ?x $?suf))
    (assert (nodo 
        (estado $?nuevo-estado)
        (camino $?movimientos $?nuevo-estado)
        (coste (+ ?coste 1))
        (heuristica (heuristica $?nuevo-estado))
    ))
)
; ------------------------------------------------------------------------------
; RESTRICCIONES

(defmodule RESTRICCIONES
    (import MAIN deftemplate nodo)
)

; Búsqueda en grado
; Detecta nodos repetidos, descartando el del camino más largo
(defrule RESTRICCIONES::nodo-repetido
    (declare (auto-focus TRUE))

    ?nodo1 <- (nodo
        (estado $?estado)
        (camino $?camino1)
    )

    (nodo 
        (estado $?estado)
        (camino $?camino2&: (> (length$ ?camino1) (length$ ?camino2)))
    )

    ; Descarta el nodo mas largo
    => (retract ?nodo1)
)

; ------------------------------------------------------------------------------
; SOLUCIÓN

(defmodule SOLUCION
    (import MAIN deftemplate nodo)
)

; Solución encontrada
; Detecta un nodo cuyo estado contenga la solución
(defrule SOLUCION::reconoce-solucion
    (declare (auto-focus TRUE))

    ?nodo <- (nodo
        (estado V V V H B B B)
        (camino $?movimientos)
    )

    ; Imprime el camino obtenido
    => 
    (retract ?nodo)
    (assert (solucion $?movimientos))    
)

; Imprime camino y detiene el programa
(defrule SOLUCION::imprimir-solucion
    (solucion $?movimientos)

    => 
    (printout t "La solucion tiene " (div (length$ ?movimientos) 7) " pasos" crlf)
    (printout t "B B B H V V V" crlf)

    (loop-for-count (?i (div (length$ ?movimientos) 7))
        (bind ?paso (subseq$ ?movimientos (+ 1 (* (- ?i 1) 7)) (* ?i 7))) 
        (printout t (implode$ ?paso) crlf)
    )

    (halt)
)
