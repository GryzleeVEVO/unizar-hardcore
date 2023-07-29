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
    Encola un mensaje en la cola.

    ID_evento: Identificador numérico del tipo de mensaje
    mensaje:   Contenido del mensaje enviado

    Si se encolan TAM_COLA + 1 elementos, la cola queda desbordada y se para el
    programa
*/
void cola_msg(uint8_t ID_msg, uint32_t mensaje);

/*
    Devuelve si la cola está vacía.
*/
uint8_t cola_hay_msg(void);

/*
    Devuelve el siguiente mensaje de la cola.

    m:  Dirección donde almacenar el mensaje obtenido

    La cola de eventos no puede estar vacía, en caso contrario el resultado
    queda indefinido.
*/
void cola_obtener_sig_msg(msg *m);

#endif // COLA_MSG_H
