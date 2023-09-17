/*
    Proyecto Hardware
    Práctica 3

    Fichero:
        msg.h

    Autores:
        Dorian Boleslaw Wozniak (817570@unizar.es)
        Pablo Latre Villacampa (778043@unizar.es)

    Descripción:
        Declaración de un tipo mensaje, que almacena mensajes síncronos junto a
        su contenido, y un conjunto de identificadores de tipos de eventos
        conocidos.
*/

#ifndef MSG_H
#define MSG_H

#include <inttypes.h>

// Tipos de mensajes disponibles
enum
{
    MSG_INDEFINIDO = 0, // Mensaje indefinido

    SET_ALARMA = 1, // Añadir alarma

    CHK_BOTON1 = 2,  // Comprobar si botón 1 esta pulsado
    CHK_BOTON2 = 3,  // Comprobar si botón 2 esta pulsado
    CHK_ENTRADA = 4, // Preparar jugada si es válida

    PARPADEAR = 5,    // Alternar LED latido
    OFF_REALIZAR = 6, // Apagar LED jugada realizada

    INICIAR = 7,   // Iniciar nueva partida
    SIGUIENTE = 8, // Comprometer jugada
    ACABAR = 9,    // Terminar partida

    GO_SLEEP = 10, // Dormir procesador
    FEED_WD = 11   // Alimentar WD para evitar reinicio
};

// Definición de un evento
typedef struct MensajeG
{
    uint8_t ID_msg;   // Identificador de mensaje
    uint32_t mensaje; // Contenido
} msg;

#endif // MSG_H