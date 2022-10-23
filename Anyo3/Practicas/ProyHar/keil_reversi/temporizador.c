#include "temporizador.h"

// Número de interrupciones
static volatile uint32_t timer_int_count = 0;
static volatile uint32_t timer_last_int = 0;
static volatile uint32_t timer_last_start = 0;

// Inicialilza el temporizador
void temporizador_iniciar() {
	// Basado en el ejemplo
	
	timer_int_count = 0;
	timer_last_int = 0;
	timer_last_start = 0;
	
	// TxMRy compara su valor con TC y hace acciones cuando son iguales
	// Puede haber 4 puntos de comparación máximos
	
	T0MR0 = 150000 - 1; 		// Acción cada 0,05ms (cada tick son ~0,3us)
	
	// TxMCR controla el comportamiento al llegar al tiempo marcado
	T0MCR = 0x1 | 0x2; // Bit 0 habilita interrupciones para MR0
											 // Bit 1 reinicia TC al llegar a MR0
	
	// Asigna a VIC 0 la rutina de servicio
	VICVectAddr0 = (uint32_t) temporizador_RSI;

	// Indica qué periférico provoca interrupciones y habilita VIC 0
	VICVectCntl0 = 0x20 | 0x4; // Bit 5 habilita VIC
														 // Bit 4-0 indican periférico (Timer0 es 4)

}

// Rutina de servicio
void temporizador_RSI (void) __irq {
	timer_int_count++;
	T0IR = 1;						// Baja interrupción
	VICVectAddr = 0;		// Indica interrupción tratada
}

// Inicia un temporizador
void temporizador_empezar() {
		
	// TxTCR habilita o deshabilita un temporizador
	T0TCR = 1;
	
	// Habilita interrupciones en VIC
	VICIntEnable = VICIntEnable | 0x10;
}

// Devuelve el tiempo transcurrido desde la última interrupción
uint32_t temporizador_leer() {
	uint32_t aux = timer_last_int - timer_int_count;
	timer_last_int = timer_int_count;
	return aux;
}

// Detiene temporizador y devuelve tiempo transcurrido desde su último inicio
uint32_t temporizador_parar() {
	T0TCR = 0; // Apaga temporizador
	
	VICIntEnable = VICIntEnable & ~0x10; // Deshabilita interrupciones
	
	return timer_int_count - timer_last_start;
	
}