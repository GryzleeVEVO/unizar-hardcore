/*
    Proyecto Hardware
    Práctica 2

    Fichero:
        cola_eventos.h

    Autores:
        Dorian Boleslaw Wozniak (817570@unizar.es)
        Pablo Latre Villacampa (778043@unizar.es)

    Descripción: 
        Interfaz para una cola de eventos. Ofrece operaciones para encolar y
        desencolar eventos, y para comporbar si la cola está vacía. La cola
        también incluye el número de veces que ha sucedido un evento.
*/

#ifndef COLA_EVENTOS_H
#define COLA_EVENTOS_H

#include <inttypes.h>
#include "eventos.h"

/*
    Añade a la cola un evento nuevo {ID_evento, auxData}. Ademas almacena el
    número de veces que se ha producido el evento para depuración
*/
void cola_encolar_eventos(uint8_t ID_evento, uint32_t veces, uint32_t auxData);

/* 
    Devuelve TRUE (1) si hay eventos en la cola sin tratar. 
    Devuelve FALSE (0) en caso contrario 
*/
uint8_t cola_hay_eventos();

/*
    Desencola el primer evento sin tratar de la cola. Se asume que hay eventos
    sin tratar en la cola
*/
void cola_obtener_sig_evento(evento *e);

#endif // COLA_EVENTOS_H