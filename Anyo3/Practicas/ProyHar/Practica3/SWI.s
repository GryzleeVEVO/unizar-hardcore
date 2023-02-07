;/*****************************************************************************/
;/* SWI.S: SWI Handler                                                        */
;/*****************************************************************************/
;/* This file is part of the uVision/ARM development tools.                   */
;/* Copyright (c) 2005-2006 Keil Software. All rights reserved.               */
;/* This software may only be used under the terms of a valid, current,       */
;/* end user licence from KEIL for a compatible version of KEIL software      */
;/* development tools. Nothing else gives you the right to use this software. */
;/*****************************************************************************/

;   Proyecto Hardware
; 
;   Práctica 3
;
;   Fichero:
;   SWI.c
;
;   Autores:
;       Dorian Boleslaw Wozniak (817570@unizar.es)
;       Pablo Latre Villacampa (778043@unizar.es)
;
;   Código de inicalización para modo supervisión y llamadas al sistema
;   definidas en ensamblador

T_Bit           EQU     0x20

                PRESERVE8                      ; 8-Byte aligned Stack
                AREA    SWI_Area, CODE, READONLY
                ARM

                EXPORT  SWI_Handler
                
SWI_Handler   

                STMFD   SP!, {R12, LR}         ; Store R12, LR
                MRS     R12, SPSR              ; Get SPSR
                STMFD   SP!, {R8, R12}         ; Store R8, SPSR
                TST     R12, #T_Bit            ; Check Thumb Bit
                LDRNEH  R12, [LR,#-2]          ; Thumb: Load Halfword
                BICNE   R12, R12, #0xFF00      ; Extract SWI Number
                LDREQ   R12, [LR,#-4]          ; ARM:   Load Word
                BICEQ   R12, R12, #0xFF000000  ; Extract SWI Number

                ; Salta a enable_fiq
                CMP     r12, #0xff
                BEQ     __enable_fiq
                
                ; Salta a disable_fiq
                CMP     r12, #0xfe
                BEQ     __disable_fiq
                
                ; Salta a enable_irq_fiq
                CMP     r12, #0xfd
                BEQ     __enable_irq_fiq
                
                ; Salta a disable_irq_fiq
                CMP     r12, #0xfc
                BEQ     __disable_irq_fiq
                
                ; Salta a read_IRQ_bit
                CMP     r12, #0xfb
                BEQ     __read_IRQ_bit
                
                ; Salta a read_FIQ_bit
                CMP     r12, #0xfa
                BEQ     __read_FIQ_bit

                ; Salta a SWI vectorizada
                LDR     R8, SWI_Count
                CMP     R12, R8
                BHS     SWI_Dead               ; Overflow
                ADR     R8, SWI_Table
                LDR     R12, [R8,R12,LSL #2]   ; Load SWI Function Address
                MOV     LR, PC                 ; Return Address
                BX      R12                    ; Call SWI Function 
                LDMFD   SP!, {r8, r12}
                MSR     SPSR_cxsf, r12
                LDMFD   SP!, {r12, PC}^
                
SWI_Dead        B       SWI_Dead               ; None Existing SWI


                ; SWI Vectorizadas


SWI_Cnt         EQU    (SWI_End-SWI_Table)/4
SWI_Count       DCD     SWI_Cnt

                IMPORT  __SWI_0
                IMPORT  __SWI_1

SWI_Table
                DCD     __SWI_0                ; SWI 0 Function Entry
                DCD     __SWI_1                ; SWI 1 Function Entry

;               ...
SWI_End

                ; SWI no vectorizadas
                
; Devuelve el valor del bit IRQ del llamante
__read_IRQ_bit
                LDMFD   SP!, {r8, r12}
                AND     r0, r12, #0x80
                LSR     r0, #7
                MSR     SPSR_cxsf, r12
                LDMFD   SP!, {r12, PC}^

; Devuelve el valor del bit IRQ del llamante                
__read_FIQ_bit
                LDMFD   SP!, {r8, r12}
                AND        r0, r12, #0x40
                LSR     r0, #6
                MSR     SPSR_cxsf, r12
                LDMFD   SP!, {r12, PC}^
                
; Habilita todas las interrupciones
__enable_irq_fiq
                LDMFD   SP!, {r8, r12}
                BIC        r12, r12, #0xc0
                MSR     SPSR_cxsf, r12
                LDMFD   SP!, {r12, PC}^
                
; Habilita interrupciones rápidas
__enable_fiq    
                LDMFD   SP!, {r8, r12}
                BIC        r12, r12, #0x40
                MSR     SPSR_cxsf, r12
                LDMFD   SP!, {r12, PC}^

; Deshabilita todas las interrupciones
__disable_irq_fiq
                LDMFD   SP!, {r8, r12}
                ORR        r12, r12, #0xc0
                MSR     SPSR_cxsf, r12
                LDMFD   SP!, {r12, PC}^
                
; Deshabilita interrupciones rápidas
__disable_fiq
                LDMFD   SP!, {r8, r12}
                ORR        r12, r12, #0x40
                MSR     SPSR_cxsf, r12
                LDMFD   SP!, {r12, PC}^
                
                END
