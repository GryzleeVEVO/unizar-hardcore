		AREA datos, DATA

NUM_FILAS       EQU 6
NUM_COLUMNAS    EQU 7
PADDING_FIL     EQU 1
PADDING_COL     EQU 1
TAM_FILS        EQU NUM_FILAS + PADDING_FIL
TAM_COLS        EQU NUM_COLUMNAS + PADDING_COL

CELDA_VACIA     EQU 0x04
CELDA_COLOR     EQU 0x03

FALSE           EQU 0
TRUE            EQU 1
	
		AREA codigo, CODE
						
		EXPORT conecta4_buscar_alineamiento_asm_iter

;   Entrada:
;       r0 -> @cuadricula
;       r1 -> fila
;       r2 -> columna
;       r3 -> color
;       r11 + 4 -> *deltas_fila[i]
;       r11 + 8 -> *deltas_columna[i]
;
;   Salida
;       r0 <- longuitud linea
;
;   Descripción
;
;       Devuelve el número de celdas del mismo color consecutivas en
;       la línea recta dada por delta_fila y delta_columna a partir de 
;       cuadricula[fila][columna]

conecta4_buscar_alineamiento_asm_iter
                ; Prologo
                mov     r12, r13   
                stmdb   r13!, { r4 - r10, r11, r12, r14, r15 }
                sub     r11, r12, #4

                ;   Bloque de activación:
                ;
                ;   r13' ->  r4 - r10        buscar_alineamiento
                ;           r11
                ;           r13
                ;           r14
                ;   r11' ->  r15
                ;           --------------
                ;           delta_fila[i]   hay_linea
                ;           delta_columna[i]

                ; Mueve parametros a registros variables
                mov     r4, r0          ; r4 = @cuadricula
                mov     r5, r1          ; r5 = fila
                mov     r6, r2          ; r6 = columna
                mov     r7, r3          ; r7 = color
                add     r9, r11, #0x04  ; Calcula @ base de parametros
                ldmia   r9, { r8 - r9 } ; r8, r9 = delta_columna, delta_fila
				mov		r0 , #0			; i = num lineas encontradas
				
				
                ; while
                ;   !C4_fila_valida(fila) (! 1 <= fila <= NUM_FILAS)
ba_while        cmp     r5, #1
                blt     ba_return_zero 
                cmp     r5, #NUM_FILAS
                bgt     ba_return_zero

                ;   !C4_columna_valida(columna)) (! 1 <= columna <= NUM_COLUMNAS)
                cmp     r6, #1
                blt     ba_return_zero 
                cmp     r6, #NUM_COLUMNAS
                bgt     ba_return_zero


                ;   Obtiene valor de celda en cuadricula[fila][columna]
                add     r10, r4, r5, lsl #3     ; r10 = @cuadricula[fila][0] (@cuad + 8 * filas)
                ldrb    r10, [r10, r6]          ; r10 = *cuadricula[fila][columna]

                ;   celda_vacia(cuadricula[fila][columna]) ( celda & 0x04 == 0)

                and     r3, r10, #CELDA_VACIA
                cmp     r3, #0
                beq     ba_return_zero
            
                ;   celda_color(cuadricula[fila][columna] != ) ( celda & 0x03 != color)
                and     r3, r10, #CELDA_COLOR
                cmp     r3, r7
                bne     ba_return_zero

                ; 	Actualiza índices
                add     r5, r5, r8
                add     r6, r6, r9
				
				add		r0, r0 , #1

                b ba_while

ba_return_zero  ldmdb   r11, { r4 - r10, r11, r13, r15 }

				END