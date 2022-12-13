; Ingeniería artifical
; Trabajo TP6 - parte 1
;
; Fichero: granjero.clp
;
; Autor: Dorian Boleslaw Wozniak (817570@unizar.es)
;
; Descripción:
;   Implementación en CLIPS del problema del granjero, la col, la cabra y el 
;   lobo utilizando el algoritmo de búsqueda de coste uniforme en grafo 

; -----------------------------------------------------------------------------
; MAIN

(defmodule MAIN
    (export deftemplate nodo)
    (export deffunction opuesta)
)

; Devuelve la orilla contraria
(deffunction MAIN::opuesta (?orilla)
    (if (eq ?orilla izquierda)
        then derecha
     else izquierda
    )
)

; Definición de un nodo
(deftemplate MAIN::nodo
    (slot localizacion-granjero)    ; Estado
    (slot localizacion-lobo)
    (slot localizacion-cabra)
    (slot localizacion-col)
    (slot coste (default 0))        ; Coste
    (slot clase (default abierto))  ; Nodo expandido
    (multislot camino)              ; Camino tomado
)

; Declara el nodo inicial
(deffacts MAIN::nodo-inicial
    (nodo
        (localizacion-granjero izquierda)
        (localizacion-lobo     izquierda)
        (localizacion-cabra    izquierda)
        (localizacion-col      izquierda)
        (coste 0)
        (clase abierto)
        (camino)
    )
)

(defrule MAIN::CU-cerrar-estado
	?nodo <- (nodo
        (coste ?c1)
        (clase abierto)
    )

	(not (nodo
        (clase abierto)
        (coste ?c2&:(< ?c2 ?c1))
    ))
    =>
	(modify ?nodo (clase cerrado))
	(focus OPERADORES)
)

; -----------------------------------------------------------------------------
; OPERADORES


(defmodule OPERADORES
    (import MAIN deftemplate nodo)
    (import MAIN deffunction opuesta)
)

; Solo se mueve el granjero
(defrule OPERADORES::movimiento-solo
    ?nodo <- (nodo
        (localizacion-granjero  ?orilla)
        (camino $?movimientos)
        (clase cerrado)
        (coste ?c)
    )
    =>
    (duplicate ?nodo
        (localizacion-granjero  (opuesta ?orilla))
        (camino $?movimientos solo)
        (clase abierto)
        (coste (+ ?c 1))
    )
)

; Se mueve el granjero y el lobo
(defrule OPERADORES::movimiento-con-lobo
    ?nodo <- (nodo  
        (localizacion-granjero  ?orilla)
        (localizacion-lobo      ?orilla)
        (camino $?movimientos)
        (clase cerrado)
        (coste ?c)
    )
    =>
    (duplicate ?nodo
        (localizacion-granjero  (opuesta ?orilla))
        (localizacion-lobo      (opuesta ?orilla))
        (camino $?movimientos lobo)
        (clase abierto)
        (coste (+ ?c 1))
    )
)

; Se mueve el granjero y la cabra
(defrule OPERADORES::movimiento-con-cabra
    ?nodo <- (nodo
        (localizacion-granjero  ?orilla)
        (localizacion-cabra     ?orilla)
        (camino $?movimientos)
        (clase cerrado)
        (coste ?c)
    )
    =>
    (duplicate ?nodo
        (localizacion-granjero  (opuesta ?orilla))
        (localizacion-cabra     (opuesta ?orilla))
        (camino $?movimientos cabra)
        (clase abierto)
        (coste (+ ?c 1))
        
    )
)

; Se mueve el granjero y la col
(defrule OPERADORES::movimiento-con-col
    ?nodo <- (nodo  
        (localizacion-granjero  ?orilla)
        (localizacion-col       ?orilla)
        (camino $?movimientos)
        (clase cerrado)
        (coste ?c)
    )
    =>
    (duplicate ?nodo
        (localizacion-granjero  (opuesta ?orilla))
        (localizacion-col       (opuesta ?orilla))
        (camino $?movimientos col)
        (clase abierto)
        (coste (+ ?c 1))
    )
)

; -----------------------------------------------------------------------------
; RESTRICCIONES

(defmodule RESTRICCIONES
    (import MAIN deftemplate nodo)
)

; Comprueba si el lobo y la cabra estan en la misma orilla sin el granjero
(defrule RESTRICCIONES::lobo-come-cabra
    (declare (auto-focus TRUE))
    
    ?nodo <- (nodo
        (localizacion-granjero  ?orilla1)
        (localizacion-lobo      ?orilla2 &~ ?orilla1)
        (localizacion-cabra     ?orilla2)
    ) => (retract ?nodo)
)

; Comprueba si la cabra y la col estan en la misma orilla sin el granjero
(defrule RESTRICCIONES::cabra-come-col
    (declare (auto-focus TRUE))

    ?nodo <- (nodo  
        (localizacion-granjero  ?orilla1)
        (localizacion-cabra     ?orilla2 &~ ?orilla1)
        (localizacion-col       ?orilla2)
    ) => (retract ?nodo)
)

; Elimina nodos repetidos (búsqueda en grafo)
(defrule RESTRICCIONES::camino-circular
    (declare (auto-focus TRUE))

    (nodo   
        (localizacion-granjero  ?granjero)
        (localizacion-lobo      ?lobo)
        (localizacion-cabra     ?cabra)
        (localizacion-col       ?col)
        (camino $?c1)
    )

    ?nodo <- (nodo
        (localizacion-granjero ?granjero)
        (localizacion-lobo ?lobo)
        (localizacion-cabra ?cabra)
        (localizacion-col ?col)
        (camino $?c1 ? $?c2))
    => (retract ?nodo)
)

; -----------------------------------------------------------------------------
; SOLUCIÓN

(defmodule SOLUCION
    (import MAIN deftemplate nodo)
    (import MAIN deffunction opuesta)
)

; Detecta la solución
(defrule SOLUCION::reconoce-solucion
    (declare (auto-focus TRUE))

    ?nodo <- (nodo
        (localizacion-granjero  derecha)
        (localizacion-lobo      derecha)
        (localizacion-cabra     derecha)
        (localizacion-col       derecha)
        (camino $?movimientos)
    )
    =>
    (retract ?nodo)
    (assert (solucion $?movimientos))
)

(defrule SOLUCION::escribe-solucion
    ?mv <- (solucion $?m)
    =>
    (retract ?mv)
    (printout t crlf crlf "Solucion encontrada " crlf)
    (bind ?orilla derecha)

    (loop-for-count (?i 1 (length$ $?m))
        (bind ?cosa (nth$ ?i $?m))
        
        (printout t "El granjero se mueve "
            (switch ?cosa
                (case solo then "solo ")
                (case lobo then "con el lobo ")
                (case cabra then "con la cabra ")
                (case col then "con la col ")
            )
            "a la " ?orilla "." crlf
        )

        (bind ?orilla (opuesta ?orilla))
    )

    (printout t crlf)
)
