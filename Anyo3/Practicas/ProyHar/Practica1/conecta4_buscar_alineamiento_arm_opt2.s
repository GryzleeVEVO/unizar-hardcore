;
;    Proyecto Hardware
;    Práctica 1
;
;    Fichero: conecta4_buscar_alineamiento_arm_opt2.s
;
;    Autores:
;        Dorian Boleslaw Wozniak   (817570@unizar.es)
;        Pablo Latre Villacampa    (778043@unizar.es)
;
;    Descripción: Implementación de la función buscar_alineamiento en código ensamblador
;					sustituyendo el algoritmo recursivo por uno iterativo. Tambien
;					hace un mejor uso de los registros

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
						
		EXPORT conecta4_buscar_alineamiento_arm_opt2

;   Entrada:
;       r0 -> @cuadricula
;       r1 -> fila
;       r2 -> columna
;       r3 -> color
;       r11 + 4 -> *deltas_fila[i]
;       r11 + 8 -> *deltas_columna[i]
;
;   Salida:
;       r0 <- longuitud linea
;
;   Descripción:
;       Devuelve el número de celdas del mismo color consecutivas en
;       la línea recta dada por delta_fila y delta_columna a partir de 
;       cuadricula[fila][columna]

conecta4_buscar_alineamiento_arm_opt2
                ; Prologo
                mov     r12, r13   
                stmdb   r13!, { r4 - r10, r11, r12, r14, r15 }
                sub     r11, r12, #4

                ;   Bloque de activación:
                ;
                ;   SP  -> | r4 - r10          | buscar_alineamiento
                ;          | FP anterior	   |
                ;          | SP anterior       |
                ;          | LR	anterior       |
                ;   FP	-> | PC	anterior	   |
                ;          | ----------------- |
                ;          | delta_fila[i]     | hay_linea
                ;          | delta_columna[i]  |

                ; Mueve parametros a registros variables
                mov     r4, r0          ; r4 = @cuadricula
                add     r7, r11, #0x04  ; Calcula @ base de parametros
                ldmia   r7, { r5 - r6 } ; r5, r6 = delta_columna, delta_fila
				mov		r0 , #0			; i = num lineas encontradas
						
                ; while
                ;   !C4_fila_valida(fila) (! 1 <= fila <= NUM_FILAS)
ba_while        cmp     r1, #1
                blt     ba_return 
                cmp     r1, #NUM_FILAS
                bgt     ba_return

                ;   !C4_columna_valida(columna)) (! 1 <= columna <= NUM_COLUMNAS)
                cmp     r2, #1
                blt     ba_return 
                cmp     r2, #NUM_COLUMNAS
                bgt     ba_return

                ;   Obtiene valor de celda en cuadricula[fila][columna]
                add     r7, r4, r1, lsl #3     ; r7 = @cuadricula[fila][0] (@cuad + 8 * filas)
                ldrb    r7, [r7, r2]           ; r7 = *cuadricula[fila][columna]

                ;   celda_vacia(cuadricula[fila][columna]) ( celda & 0x04 == 0)
                tst     r7, #CELDA_VACIA
                beq     ba_return
            
                ;   celda_color(cuadricula[fila][columna] != ) ( celda & 0x03 != color)
                and     r8, r7, #CELDA_COLOR
                cmp     r8, r3
                bne     ba_return

                ; 	Actualiza índices
                add     r1, r1, r5
                add     r2, r2, r6
				
				;	Suma 1 al resultado
				add		r0, r0 , #1

                b ba_while

ba_return  		ldmdb   r11, { r4 - r10, r11, r13, r15 }

				END