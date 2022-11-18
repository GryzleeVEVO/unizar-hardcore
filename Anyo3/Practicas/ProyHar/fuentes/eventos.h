#ifndef EVENTOS_H
#define EVENTOS_H

#include <inttypes.h>

// Tipos de eventos disponibles
enum {
    TIEMPO_PERIODO  = 0,
    TIEMPO_CONTADOR = 1
};

// Un evento contiene un ID y datos auxiliares (según tipo)
typedef struct EventoHW {
    uint8_t ID_evento;
    uint32_t auxData;

} evento ;

#endif // EVENTOS_H
