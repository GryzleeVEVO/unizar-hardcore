/*
    Proyecto Hardware
    Práctica 2

    Fichero:
        cola_eventos.h

    Autores:
        Dorian Boleslaw Wozniak (817570@unizar.es)
        Pablo Latre Villacampa (778043@unizar.es)

    Descripción: 
        Implementación de un TAD Cola circular que almacena eventos junto al 
        número de veces que ha ocurrido. Permite encolar, desencolar y 
        comprobar si está vacía
*/

#ifndef COLA_EVENTOS_H
#define COLA_EVENTOS_H

#include <inttypes.h>
#include "eventos.h"

/*
    Pre:  numElem < TAM_COLA
    Post: Se encola un nuevo elemento {{ID_evento, auxData}, veces} al final
            de la cola.
          Si numElem >= TAM_COLA, se detiene el programa
*/
void cola_encolar_eventos(uint8_t ID_evento, uint32_t veces, uint32_t auxData);

/* 
    Pre:  ---
    Post: Devuelve TRUE (1) si hay eventos en la cola sin tratar. 
          Devuelve FALSE (0) en caso contrario 
*/
uint8_t cola_hay_eventos(void);

/*
    Pre:  numElem > 0, @e != nil
    Post: e contiene el evento almacenado por el primer elemento de la cola
*/
void cola_obtener_sig_evento(evento *e);

#endif // COLA_EVENTOS_H
