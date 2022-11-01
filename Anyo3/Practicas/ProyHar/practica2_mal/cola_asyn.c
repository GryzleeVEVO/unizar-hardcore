
#include "cola_asyn.h"

enum {
	TAM_COLA = 32, // Tamaño de la cola
	
	// Códigos
	TRUE = 1,			
	FALSE = 0,
	SUCCESS = 0,
	FAILURE = 1,
	ERROR = -1
};

// Registro evento
typedef struct elem_cola {
	evento		eventoLocal;
	uint32_t 	veces;
} elemento;


static volatile evento cola_asyn[TAM_COLA]; 		// Cola circular
static uint8_t numElem = 0;						// Total de elementos encolados
static uint8_t first = 0;							// Posición del primer elemento añadido
static uint8_t last = 0;							// Posición del último elemento añadido

// Añade un evento a la cola. Si hay desborde el programa acaba
void cola_encolar_eventos(uint8_t ID_evento, uint32_t veces, uint32_t auxData){	
	
	// Añade a cola


	// Avanza indice del último insertado
	if (last < TAM_COLA) last++;
	else last = 0;
	
	// Si hay desborde se bloquea
	numElem++;
	if (numElem > TAM_COLA) while (1);
}

// Planificador obtiene siguiente evento para tratarlo
uint8_t cola_obtener_sig_evento(evento* e) {
	
	// Si no hay nada a desencolar devuelve ERROR
	if (numElem == 0) return ERROR;
	
	// Obtiene evento

	
	// Avanza índice del primer elemento
	if (first < TAM_COLA) first++;
	else first = 0;
		
	// Elimina evento de pendientes
	numElem--;
}
