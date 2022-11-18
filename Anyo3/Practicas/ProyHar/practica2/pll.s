;    Proyecto Hardware
;    Práctica 2
;
;    Fichero:
;        pll.c
;
;    Autores:
;        Dorian Boleslaw Wozniak (817570@unizar.es)
;        Pablo Latre Villacampa (778043@unizar.es)
;
;    Descripción: 
;		 Implementación del reinicio del PLL tras despertar del estado 
;		 power-down tal y como se configura durante el startup

	AREA datos, DATA
		
; Definiciones de constantes para gestionar PLL
PLL_BASE        EQU     0xE01FC080      ; PLL Base Address
PLLCON_OFS      EQU     0x00            ; PLL Control Offset
PLLCFG_OFS      EQU     0x04            ; PLL Configuration Offset
PLLSTAT_OFS     EQU     0x08            ; PLL Status Offset
PLLFEED_OFS     EQU     0x0C            ; PLL Feed Offset
PLLCON_PLLE     EQU     (1<<0)          ; PLL Enable
PLLCON_PLLC     EQU     (1<<1)          ; PLL Connect
PLLCFG_MSEL     EQU     (0x1F<<0)       ; PLL Multiplier
PLLCFG_PSEL     EQU     (0x03<<5)       ; PLL Divider
PLLSTAT_PLOCK   EQU     (1<<10)         ; PLL Lock Status

;// <e> PLL Setup
;//   <o1.0..4>   MSEL: PLL Multiplier Selection
;//               <1-32><#-1>
;//               <i> M Value
;//   <o1.5..6>   PSEL: PLL Divider Selection
;//               <0=> 1   <1=> 2   <2=> 4   <3=> 8
;//               <i> P Value
;// </e>
PLL_SETUP       EQU     1
PLLCFG_Val      EQU     0x00000024

	AREA codigo, CODE

	EXPORT reiniciar_pll

reiniciar_pll
				LDR     R0, =PLL_BASE
                MOV     R1, #0xAA
                MOV     R2, #0x55

;  Configure and Enable PLL
                MOV     R3, #PLLCFG_Val
                STR     R3, [R0, #PLLCFG_OFS] 
                MOV     R3, #PLLCON_PLLE
                STR     R3, [R0, #PLLCON_OFS]
                STR     R1, [R0, #PLLFEED_OFS]
                STR     R2, [R0, #PLLFEED_OFS]

;  Wait until PLL Locked
PLL_Loop        LDR     R3, [R0, #PLLSTAT_OFS]
                ANDS    R3, R3, #PLLSTAT_PLOCK
                BEQ     PLL_Loop

;  Switch to PLL Clock
                MOV     R3, #(PLLCON_PLLE:OR:PLLCON_PLLC)
                STR     R3, [R0, #PLLCON_OFS]
                STR     R1, [R0, #PLLFEED_OFS]
                STR     R2, [R0, #PLLFEED_OFS]
				
				BX 		r14
		END