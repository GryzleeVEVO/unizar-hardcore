#include <LPC210X.h>

#include "temporizador.h"
#include "cola_asyn.h"


// timer0 encola eventos con un periodo dado

// timer1 mide tiempos
static volatile uint32_t timer1_int_count;

// Rutinas de servicio
void timer0_RSI (void) __irq {
	timer0_int_count++;
	T0IR = 1;						// Baja interrupción
	cola_encolar_eventos(TIMER0, timer0_int_count, 0);
	VICVectAddr = 0;		// Indica interrupción tratada
}

void timer1_RSI (void) __irq {
	timer1_int_count++;
	T1IR = 1;						// Baja interrupción
	VICVectAddr = 0;		// Indica interrupción tratada
}

// Inicialilza el temporizador
void temporizador_iniciar() {
	T1PR = 15 - 1; 		  // Tick cada 15 ciclos (~0,066 us)
	T1MR0 = 0xffffffff - 1;	// Actúa cada vez que llegue al final del contador
	T1MCR = 0x1 | 0x2; 	// Interrumpe y reinicia el contador
	
	VICVectAddr1 = (uint32_t) timer1_RSI; // VIC[1] = RSI timer1
	VICVectCntl1 =  0x20 | 0x5 ; // Habilita VIC[1] para interrupciones de timer1
}


// Inicia un temporizador
void temporizador_empezar() {
	
	T1TCR = 3; 														// Reinicia TC y empieza a contar
	VICIntEnable = VICIntEnable | 0x10; 	// Habilita interrupciones en VIC[1]
}

// Devuelve el tiempo transcurrido desde la última interrupción
uint32_t temporizador_leer() {

	// Lee tiempo actual (en us)
	uint32_t time = T1TC;
	
	// Comprueba si se ha desbordado el TC desde la última vez
	// Se desborda cada (2^32 - 1) * 15 * (15 * 10^6))^-1 segundos
	// Fórmula: (Nº máximo us) * ciclos * periodo
	// for (int i = timer1_int_count; i >= 0; i--) time += (0xffffffff)
	
	return time;
}

// Detiene temporizador y devuelve tiempo transcurrido desde su último inicio
uint32_t temporizador_parar() {
	T1TCR = 0; // Detiene timer1
	VICIntEnable = VICIntEnable & ~0x10; // Deshabilita interrupciones
	
	return timer1_int_count;
}

// Inicia un temporizador que encola un evento cada periodo ms
void temporizador_reloj(uint32_t periodo) {
	timer0_int_count = 0;

	
	T0PR = 0; 								// Aumenta el contador (TC) del reloj cada ciclo
	T0MR0 = 12000 * periodo; 	// Hace una acción cuando el TC llega a 1ms * periodo
	T0MCR = 0x1 | 0x2; 				// Habilita interrupciones y reinicia TC si iguala a MR0	
	
	
	VICVectAddr0 = (uint32_t) timer0_RSI; // VIC[0] = RSI timer0
	VICVectCntl0 =  0x20 | 0x4 ; // Habilita VIC[0] para interrupciones de timer0
	
	T0TCR = 1;	// Inicia el contador
	VICIntEnable = VICIntEnable | 0x8; 	// Habilita interrupciones en VIC[0]
}