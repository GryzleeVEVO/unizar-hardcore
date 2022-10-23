#include "cola_asyn.h"

// Añade un evento a la cola
void cola_encolar_eventos(uint8_t ID_evento, uint32_t veces, uint32_t auxData){	
	
	// Añade a cola
	cola_asyn[last].ID_evento = ID_evento;
	cola_asyn[last].veces = veces;
	cola_asyn[last].auxData = auxData;

	// Avanza indice del último insertado
	if (last < TAM_COLA) last++;
	else last = 0;
	
	// Commprueba si hay desborde
	numElem++;
	if (numElem > TAM_COLA) overflow = TRUE;
}

// Planificador obtiene siguiente evento para tratarlo
uint8_t cola_obtener_sig_evento(evento* e) {
	
	// Si no hay nada a desencolar devuelve ERROR
	if (numElem == 0) return ERROR;
	
	// Obtiene evento
	e -> ID_evento  = cola_asyn[first].ID_evento;
	e -> veces = cola_asyn[first].veces;
	e -> auxData = cola_asyn[first].auxData;
	
	// Avanza índice del primer elemento
	if (first < TAM_COLA) first++;
	else first = 0;
		
	// Elimina evento de pendientes
	numElem--;
}

// Devuelve si se ha desbordado la cola
uint8_t cola_desbordada() { return overflow; }


