#ifndef COLA_ASYN_H
#define COLA_ASYN_H

#include "eventos.h"

enum {
	TAM_COLA = 32,
	
	TRUE = 1,
	FALSE = 0,
	ERROR = -1
};

static evento cola_asyn[TAM_COLA]; 		// Cola circular
static uint8_t numElem = 0;						// Total de elementos encolados
static uint8_t first = 0;							// Posición del primer elemento añadido
static uint8_t last = 0;							// Posición del último elemento añadido
static uint8_t overflow = FALSE;

// Añade un evento a la cola
void cola_encolar_eventos(uint8_t ID_evento, uint32_t veces, uint32_t auxData);

// Planificador obtiene siguiente evento para tratarlo
evento cola_obtener_sig_evento();

// Devuelve si se ha desbordado la cola
uint8_t cola_desbordada();

#endif // COLA_ASYN_H
