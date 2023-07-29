/*
    Proyecto Hardware
    Práctica 3

    Fichero:
        eventos.h

    Autores:
        Dorian Boleslaw Wozniak (817570@unizar.es)
        Pablo Latre Villacampa (778043@unizar.es)

    Descripción:
        Declaración de un tipo evento, que almacena eventos asíncronos junto a
        información adicional, y un conjunto de identificadores de tipos de
        eventos conocidos.
*/

#ifndef EVENTOS_H
#define EVENTOS_H

#include <inttypes.h>

// Tipos de eventos disponibles
enum
{
    EV_INDEFINIDO = 0, // Evento indefinido

    T0_PERIODO = 1,  // Timer0 alcanza un periodo
    T1_OVERFLOW = 2, // Timer1 desborda

    BOTON1 = 3, // Se pulsa el botón 1
    BOTON2 = 4, // Se pulsa el botón 2

    UART0_LEER = 5,    // Se ha leido un nuevo caracter de la UART
                       // auxData: Caracter leido
    UART0_ESCRIBIR = 6 // Se ha escrito el caracter en la UART
};

// Definición de un evento
typedef struct EventoHW
{
    uint8_t ID_evento; // Identificador de evento
    uint32_t auxData;  // Datos auxiliares del evento
} evento;

#endif // EVENTOS_H
