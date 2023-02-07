/*
    Proyecto Hardware
    Práctica 2

    Fichero:
        eventos.h

    Autores:
        Dorian Boleslaw Wozniak (817570@unizar.es)
        Pablo Latre Villacampa (778043@unizar.es)

    Descripción: 
        Declaración de un tipo evento, los cuales son producidos por los 
        periféricos del procesador. Un evento almacena un ID y datos auxiliares
        según el tipo de evento. También contiene los ID de evento disponibles.
*/

#ifndef EVENTOS_H
#define EVENTOS_H

#include <inttypes.h>

// Tipos de eventos disponibles
enum {
    T0_PERIODO  = 1, // El timer0 ha alcanzado el periodo programado
    T1_OVERFLOW = 2, // El contador timer1 ha desbordado
    BOTON1      = 3, // Se ha pulsado el boton 1
    BOTON2      = 4  // Se ha pulsado el boton 2
};

// Tipo evento
typedef struct EventoHW {
    uint8_t ID_evento;      // ID de evento
    uint32_t auxData;       // Datos auxiliares (depende del tipo)

} evento ;

#endif // EVENTOS_H
