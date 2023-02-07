;
;    Proyecto Hardware
;    Práctica 1
;
;    Fichero: conecta4_hay_linea_arm_arm_opt2.s
;
;    Autores:
;        Dorian Boleslaw Wozniak   (817570@unizar.es)
;        Pablo Latre Villacampa    (778043@unizar.es)
;
;    Descripción: Implementación de la función hay_linea en código ensamblador
;					con llamada a buscar_alineamiento_arm_opt2

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

deltas_fila     DCB 0, -1, -1, 1
deltas_columna  DCB -1, 0, -1, -1


		AREA codigo, CODE
		
		IMPORT conecta4_buscar_alineamiento_arm_opt2
		PRESERVE8 {TRUE}
		
		EXPORT conecta4_hay_linea_arm_arm_opt2    
		
;   Entrada:
;       r0 -> @cuadricula
;       r1 -> fila
;       r2 -> columna
;       r3 -> color
;
;   Salida
;       r0 <- >= 1 si hay linea, 0 si no hay linea
;
;   Descripción:
;		Devuelve verdad si ha encontrado una línea de celdas del mismo color al
;		dado de 4 celdas o más. En caso contrario, devueve falso.
conecta4_hay_linea_arm_arm_opt2
                ; Prologo
                mov     r12, r13   
                stmdb   r13!, { r4 - r10, r11, r12, r14, r15 }
                sub     r11, r12, #4
				
                ;   Bloque de activación:
                ;
                ;   SP  -> | r4 - r10          | hay_linea
                ;          | FP anterior	   |
                ;          | SP anterior       |
                ;          | LR	anterior       |
                ;   FP	-> | PC	anterior	   |
                ;          | ----------------- |
                ;          | 		           | C4_verificar_4_en_linea

                ; Mueve parametros a registros variables
                mov     r4, r0          ; r4 = @cuadricula
                mov     r5, r1          ; r5 = fila
                mov     r6, r2          ; r6 = columna
                mov     r7, r3          ; r7 = color

                ; Inicializa variables del bucle
                mov     r8, #0           ; i              
                mov     r9, #0           ; linea
                mov     r10, #0          ; long_linea

                ; for()
                ;   i < 4
hl_for          cmp     r8, #4
                bge     hl_return
                ;   linea == FALSE
                cmp     r9, #FALSE
                bne     hl_return

                ;   Obtiene deltas[i]
                LDR     r0, =deltas_fila
                ldrsb   r0, [r0, r8]
                LDR     r1, =deltas_columna
                ldrsb   r1, [r1, r8]

                ;   Prepara primera llamada a buscar_alineamiento
                stmdb   r13!, {r0, r1}
                mov     r0, r4
                mov     r1, r5
                mov     r2, r6
                mov     r3, r7

				bl      conecta4_buscar_alineamiento_arm_opt2

                ;   linea = long_linea >= 4
                mov     r10, r0
                cmp     r10, #4
                movge   r9, #TRUE
                bge     hl_for

                ;   Recupera y niega deltas
                ldmia   r13!, { r0, r3 }
                rsb     r0, r0, #0     
                rsb     r3, r3, #0
		
		        ;   Prepara segunda llamada a buscar_alineamiento
                add     r1, r5, r0
                add     r2, r6, r3
                stmdb   r13!, { r0, r3 }
                mov     r0, r4
                mov     r3, r7

                bl      conecta4_buscar_alineamiento_arm_opt2
				
                ;   linea = long_linea >= 4
                add     r10, r10, r0
                cmp     r10, #4
                movge   r9, #TRUE

                ;   i++
                add     r8, r8, #1
        
                b     	hl_for

hl_return       mov     r0, r9
                ldmdb   r11, { r4 - r10, r11, r13, r15 }
				
				END