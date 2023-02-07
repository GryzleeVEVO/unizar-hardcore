/*
    Proyecto Hardware
    Práctica 2

    Fichero:
        cola_msg.h

    Autores:
        Dorian Boleslaw Wozniak (817570@unizar.es)
        Pablo Latre Villacampa (778043@unizar.es)

    Descripción: 
        Interfaz de un TAD Cola circular que almacena mensajes junto al 
        instante de tiempo al encolar. Permite encolar, desencolar y comprobar
        si está vacía
*/

#ifndef COLA_MSG_H
#define COLA_MSG_H

#include <inttypes.h>
#include "msg.h"

/*
    Pre:  numElem < TAM_COLA
    Post: Se encola un nuevo elemento {{ID_msg, mensaje}, <tiempo actual>} al 
            final de la cola.
          Si numElem >= TAM_COLA, se detiene el programa
*/
void cola_msg(uint8_t ID_msg, uint32_t mensaje);

/* 
    Pre:  ---
    Post: Devuelve TRUE (1) si hay eventos en la cola sin tratar. 
          Devuelve FALSE (0) en caso contrario 
*/
uint8_t cola_hay_msg(void);

/*
    Pre:  numElem > 0, @m != nil
    Post: m contiene el mensaje almacenado por el primer elemento de la cola
*/
void cola_obtener_sig_msg(msg *m);

#endif // COLA_MSG_H
