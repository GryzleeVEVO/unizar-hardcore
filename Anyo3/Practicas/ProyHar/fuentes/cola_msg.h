/*
    Proyecto Hardware
    Práctica 2

    Fichero:
        cola_msg.h

    Autores:
        Dorian Boleslaw Wozniak (817570@unizar.es)
        Pablo Latre Villacampa (778043@unizar.es)

    Descripción: 
        Interfaz para una cola de mensajes. Ofrece operaciones para encolar y
        desencolar mensajes, y para comporbar si la cola está vacía. La cola
        también incluye el tiempo en el que se ha encolado el mensaje.
*/

#ifndef COLA_MSG_H
#define COLA_MSG_H

#include <inttypes.h>
#include "msg.h"

/*
    Añade a la cola un mensaje nuevo {ID_msg, mensaje}. Ademas almacena el
    número de veces que se ha producido el msg para depuración
*/
void cola_msg(uint8_t ID_msg, uint32_t mensaje);

/*
    Devuelve TRUE (1) si hay mensaje en la cola sin tratar.
    Devuelve FALSE (0) en caso contrario
*/
uint8_t cola_hay_msg();

/*
    Desencola el primer mensaje sin tratar de la cola. Se asume que hay mensaje
    sin tratar en la cola
*/
void cola_obtener_sig_msg(msg *m);

#endif // COLA_MSG_H