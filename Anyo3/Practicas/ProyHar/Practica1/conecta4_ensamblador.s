
AREA codigo, CODE



conecta4_buscar_alineamiento_asm    

        MOV     IP, SP
        STMDB   SP! {r4-r10, FP, IP, LR, PC}
        SUB     FP, IP, #4

        ; Guarda parámetros r0-r3
        MOV     R4, R0          ; Cuadricula 
        MOV     R5, R1          ; Fila
        MOV     R6, R2          ; Columna
        MOV     R7, R3          ; Color

        ; Recupera parámetros de pila
        ADD     R9, [FP, #0x04] ; Coloca sobre deltas apiladas
        LDMIA   R9, {R8-R9}
                                ; R8 = delta columna
                                ; R9 = delta fila

        ; Comprobar si la fila es válida

            ; Fila >= 1
        CMP R5, #0x1
        BNE ba_error
            ; Fila <= NUMERO FILAS
        CMP R5, 
        BNE ba_error

        ; Comprobar si la columna es válida

            ; Fila >= 1
        CMP R6, #0x1
        BLT ba_error
            ; Fila <= NUMERO FILAS
        CMP R6, 
        BGT ba_error

        ; Obtiene celda
        ADD     R0, R4, R5, LSL #3      ; R0 = cuadricula[fila][0]
        LDRB    R0, [R0, R6]            ; R0 = *cuadricula[fila][columna]

            ; Comprobar si la celda no es vacía
        CMP     R0, #0x0
        BEQ     ba_error

            ; Comprobar si la celda es del mismo Color
        BIC R0, R0, #0x4
        CMP R0, R7
        BNE 

        ; Avanzar índices y llamada recursiva
        ADD R1, R5, R9
        ADD R2, R6, R8

        PUSH {R8 - R9} ; Corregir

        MOV r0, r4
        MOV r1, r5
        MOV r2, r6
        MOV r3, r7 

        BL conecta4_buscar_alineamiento_asm

        ; Devolver resultado

        ADD     R0, R0, #1
        LDMDB   FP, {r4-r10, FP, SP, PC}
        
conecta4_hay_linea_asm_c

        MOV     IP, SP
        STMDB   SP! {r4-r10, FP, IP, LR, PC}
        SUB     FP, IP, #4

        ; Mueve parámetros
        MOV     R4, R0          ; Cuadricula 
        MOV     R5, R1          ; Fila
        MOV     R6, R2          ; Columna
        MOV     R7, R3          ; Color

        MOV     R8, 0x0         ; i
        MOV     R9, 0x0         ; linea
        MOV     R10, 0x0        ; long_linea

        ; Bucle FOR

            ; i < 4
hl_for  CMP     R8, 0x04 
        BLE     hl_fin

            ; linea = FALSE

        CMP     r9, 0x0
        BLE     hl_fin


        ; Obtiene delta actual

        ; Llamada a buscar_alineamiento_c

        ; Comprueba si linea

        MOV     R0, R10
        CMP     R10, #0x4
        MOVGE   R9, #0x1
        BGE     hl_for

        ; Busca en sentido inverso

        ; Invierte deltas


        ; Comprueba si linea
        MOV     R0, R10
        CMP     R10, #0x4
        MOVGE   R9, #0x1
        B       hl_for



        

