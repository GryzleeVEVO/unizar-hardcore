

```
; Conjunto 0
; Prueba leer y escribir en cada bloque
; Reemplaza dos veces cada vía

; Llena vias 
(0, 0)
lw 0x0000       ; Lectura C0 V0 B0      Miss
sw 0x0004       ; Escritura C0 V0 B1    Hit

(0, 1)
lw 0x0048       ; Lectura C0 V1 B2      Miss
sw 0x004C       ; Escritura C0 V1 B3    Hit

; 1er reemplazo vias, comprueba que se lee/escribe bien de cada bloque
(0, 0)
sw 0x0088       ; Escritura C0 V0 B2    Miss Debe provocar fetch
lw 0x008C       ; Lectura C0 V0 B3      Hit

(0, 1)
sw 0x00C0       ; Escritura C0 V1 B0    Miss
lw 0x00C4       ; Lectura C0 V1 B1      Hit

(0, 0)
lw 


```

- L/E Bloque 0 [x]
- L/E Bloque 1 [x]
- L/E Bloque 2 [x]
- L/E Bloque 3 [x]