; ------------------------------------------------------------------------------
; MAIN

(defmodule MAIN
    (export deftemplate nodo)
)


; Representa un nodo
(deftemplate MAIN::nodo
    ; Estado del nodo
    (slot localizacion-granjero)
    (slot localizacion-lobo)
    (slot localizacion-cabra)
    (slot localizacion-col)
    ; Recorrido realizado
    (multislot camino)
)







; ------------------------------------------------------------------------------
; OPERADORES

(defmodule OPERACIONES)


; ------------------------------------------------------------------------------
; RESTRICCIONES

(defmodule RESTRICCIONES)


; ------------------------------------------------------------------------------
; SOLUCIÓN

(defmodule SOLUCION)