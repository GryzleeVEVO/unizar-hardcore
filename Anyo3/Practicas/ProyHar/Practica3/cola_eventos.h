/*
    Proyecto Hardware
    Práctica 3

    Fichero:
        cola_eventos.h

    Autores:
        Dorian Boleslaw Wozniak (817570@unizar.es)
        Pablo Latre Villacampa (778043@unizar.es)

    Descripción:
        Interfaz de un TAD Cola circular que almacena eventos (asíncronos,
        generados por interrupciones) junto al número de veces que ha ocurrido.
        Permite encolar, desencolar y comprobar si está vacía.
*/

#ifndef COLA_EVENTOS_H
#define COLA_EVENTOS_H

#include <inttypes.h>
#include "eventos.h"

/*
    Encola un evento en la cola generado por una IRQ.

    ID_evento: Identificador numérico del evento (ver eventos.h)
    veces:     Número de veces que ha ocurrido el evento
    auxData:   Datos adicionales dados por el evento

    Las escrituras se realizan en exclusión mútua, deshabilitando interrupciones
    rápidas.

    Si se encolan TAM_COLA + 1 elementos, la cola queda desbordada y se para el
    programa
*/
void cola_encolar_eventos(uint8_t ID_evento, uint32_t veces, uint32_t auxData);

/*
    Encola un evento en la cola generado por una FIQ.

    ID_evento: Identificador numérico del evento (ver eventos.h)
    veces:     Número de veces que ha ocurrido el evento
    auxData:   Datos adicionales dados por el evento

    Las escrituras se realizan en exclusión mútua (FIQ lo hace por defecto).

    Si se encolan TAM_COLA + 1 elementos, la cola queda desbordada y se para el
    programa
*/
void cola_encolar_eventos_fiq(uint8_t ID_evento, uint32_t veces, uint32_t auxData);

/*
    Devuelve si la cola está vacía.
*/
uint8_t cola_hay_eventos(void);

/*
    Devuelve el siguiente evento de la cola.

    e:  Dirección donde almacenar el evento obtenido

    La cola de eventos no puede estar vacía, en caso contrario el resultado
    queda indefinido.
*/
void cola_obtener_sig_evento(evento *e);

#endif // COLA_EVENTOS_H
