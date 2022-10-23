#include "boton_eint0.h"
#include <LPC210X.H>                            // LPC21XX Peripheral Registers

// Cuenta de interrupciones
static volatile unsigned int eint0_count = 0;
// != 0 si ha habido interrupción
static volatile int eint0_nueva_pulsacion = 0;

void eint0_ISR (void) __irq {
	eint0_count++;
	
	// Baja interrupción
	EXTINT =  EXTINT | 1;                
	VICVectAddr = 0;
	
	eint0_nueva_pulsacion = 1;
}

// Tras tratar pulsación
void eint0_clear_nueva_pulsacion(void){ eint0_nueva_pulsacion = 0; };

// Devuelve si se ha pulsado el botón
unsigned int eint0_read_nueva_pulsacion(void){ return eint0_nueva_pulsacion; };

// Devuelve número de pulsaciones
unsigned int eint0_read_count(void){ return eint0_count; };

void eint0_init (void) {
// NOTA: según el manual se puede configurar cómo se activan las interrupciones: por flanco o nivel, alta o baja. 
// Se usarían los registros EXTMODE y EXTPOLAR. 
// Sin embargo parece que el simulador los ignora y no aparecen en la ventana de ocnfiguración de EXT Interrupts
// configure EXTINT0 active if a rising-edge is detected
//	EXTMODE 	=	1; //1 edge, 0 level
//	EXTPOLAR	=	1; // 1 high, rising-edge; 0 low, falling-edge
//  prueba = EXTMODE;
	
	
	eint0_nueva_pulsacion = 0;
	eint0_count = 0;
	
	// clear interrupt flag  
	EXTINT =  EXTINT | 1;        
	
	// Configura tratamiento para IRQ
	VICVectAddr2 = (unsigned long) eint0_ISR;    

   // 0x20 bit 5 enables vectored IRQs. 
	// 14 is the number of the interrupt assigned. Number 14 is the EINT0 (see table 40 of the LPC2105 user manual
	
	PINSEL1 		= PINSEL1 & 0xfffffffC;	//Sets bits 0 and 1 to 0
	PINSEL1 		= PINSEL1 | 1;					//Enable the EXTINT0 interrupt
	VICVectCntl2 = 0x20 | 14;       
	
  VICIntEnable = VICIntEnable | 0x00004000; // Enable EXTINT0 Interrupt
}
