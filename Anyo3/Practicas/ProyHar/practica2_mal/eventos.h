#ifndef EVENTOS_H
#define EVENTOS_H

#include <inttypes.h>

// Lista de identificadores de eventos
enum {
	TIMER0 = 0,
	TIMER1 = 1
};

// Almacena cada evento en un registro evento
typedef struct evento_asyn {
	uint8_t 	ID_evento;		// Identifica quién provoca el evento
	uint32_t 	auxData;			// Información adicional (depende del tipo)
} evento ;

#endif // EVENTOS_H
