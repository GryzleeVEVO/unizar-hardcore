; ------------------------------------------------------------------------------
; MAIN

(defmodule MAIN
    (export deftemplate nodo)
    (export deffunction heuristica)
)

; Representa un nodo del problema
(deftemplate MAIN::nodo
    (multislot estado)  ; Estado actual
    (multislot camino)  ; 
    (slot heuristica)   ; Heurística utilizada
    (slot coste)        ; Coste
    (slot clase (default abierto))
)

; Estados inicial y final del problema
(defglobal MAIN
        ?*estado-inicial*   = (create$ B B B H V V V)
        ?*estado-final*     = (create$ V V V H B B B)
)

; Dado un estado, devuelve el valor de la función heurística 
(deffunction MAIN::heuristica ($?estado)
    (bind ?res)
    ; Heurística aquí
    
    ?res
)

; Arranca la búsqueda creando un nodo inicial
(defrule MAIN::inicial
    =>
    (assert (nodo
            (estado ?*estado-inicial*)
            (camino)
            (clase abierto)
        )
    )
)




; 
(defrule MAIN::

)



; ------------------------------------------------------------------------------
; OPERADORES

(defmodule OPERADORES
    (import MAIN deftemplate nodo)
    (import MAIN deffunction heuristica)
)


; Mover una ficha B posición a la izquieda
(defrule OPERADORES::mover-B-1-izda
    (nodo
        (estado  )
        (camino $?movimientos)
        (clase cerrado)
        (coste ?coste)
    ) => (
        (bind $?nuevo-estado (create$ ))
        (assert 
            (nodo
                (estado $?nuevo-estado)
                (camino $?movimientos ^)
                (coste (+ ?coste 1))
                (heuristica (heuristica $?nuevo-estado))
            )
        )
    )
)

; ------------------------------------------------------------------------------
; RESTRICCIONES

(defmodule RESTRICCIONES
    (import MAIN deftemplate nodo) 
)

; Restricción para búsqueda en grafo
; Dado dos nodos con el mismo estado, elimina el nodo con mayor número de caminos 
; disponibles
(defrule RESTRICCIONES::nodo-repetido (declare (auto-focus TRUE))    
    ?nodo1 <- (nodo (estado $?estado) (camino ($?camino1)))
    (nodo 
        (estado $?estado)
        (camino $?camino2&:(> (length$ ?camino1) (length$ ?camino2)))
    ) => (retract ?nodo1)
)

; ------------------------------------------------------------------------------
; SOLUCIÓN

(defmodule SOLUCION
    (import MAIN deftemplate nodo)
    (import MAIN)
)


(defrule SOLUCION::reconoce-solucion (declare (auto-focus TRUE))
    ?nodo <- (nodo (estado ()))

    ) => (
        (retract ?nodo)
        (assert (solucion $?movimientos))
)