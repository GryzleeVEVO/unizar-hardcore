#ifndef EVENTOS_H
#define EVENTOS_H

#include <inttypes.h>

// Identificadores �nicos para cada evento
enum {
	BOTON = 0,
	TIMER0 = 1,
	TIMER1 = 2
};

// TAD de un evento
typedef struct evento_asyn {
	uint8_t 	ID_evento;		// Identifica qui�n provoca el evento
	uint32_t 	veces;				// N�mero del evento del mismo tipo
	uint32_t 	auxData;			// Informaci�n adicional (definido por cada tipo)
} evento ;

#endif // EVENTOS_H
