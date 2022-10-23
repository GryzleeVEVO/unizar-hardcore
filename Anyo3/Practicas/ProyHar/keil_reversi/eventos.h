#ifndef EVENTOS_H
#define EVENTOS_H

#include <inttypes.h>

// Identificadores únicos para cada evento
enum {
	BOTON = 0,
	TIMER0 = 1,
	TIMER1 = 2
};

// TAD de un evento
typedef struct evento_asyn {
	uint8_t 	ID_evento;		// Identifica quién provoca el evento
	uint32_t 	veces;				// Número del evento del mismo tipo
	uint32_t 	auxData;			// Información adicional (definido por cada tipo)
} evento ;

#endif // EVENTOS_H
