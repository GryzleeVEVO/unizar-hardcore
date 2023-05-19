/*
    Proyecto Hardware
    Práctica 3

    Fichero:
        cola_msg.c

    Autores:
        Dorian Boleslaw Wozniak (817570@unizar.es)
        Pablo Latre Villacampa (778043@unizar.es)

    Descripción:
        Implementación de un TAD Cola circular que almacena mensajes (síncronos,
        generados por módulos) junto al instante de tiempo al encolar (en us).
        Permite encolar, desencolar y comprobar si está vacía.
*/

#include "cola_msg.h"
#include "tiempo.h"
#include "G_IO.h"

// Constantes
enum
{
    TAM_COLA = 32 // Tamaño de la cola
};

// Definición de un elemento de la cola
typedef struct ElementoCola
{
    msg unMsg;       // Mensaje almacenado
    uint32_t tiempo; // Instante al encolar (us)
} elemento;

static volatile elemento cola[TAM_COLA]; // Cola de mensajes
static volatile uint8_t primero = 0;     // Índice al primer elemento
static volatile uint8_t ultimo = 0;      // Índice al último elemento

/*
    Encola un mensaje en la cola.

    ID_evento: Identificador numérico del tipo de mensaje
    mensaje:   Contenido del mensaje enviado

    Si se encolan TAM_COLA + 1 elementos, la cola queda desbordada y se para el
    programa
*/
void cola_msg(uint8_t ID_msg, uint32_t mensaje)
{
    // Si desborda se detiene.
    if ((ultimo + 1) % TAM_COLA == primero)
    {
        IO_marcar_overflow();
        while (1)
            ;
    }

    // Añade a la cola
    cola[ultimo].unMsg.ID_msg = ID_msg;
    cola[ultimo].unMsg.mensaje = mensaje;
    cola[ultimo].tiempo = clock_getus();

    // Avanza índice
    ultimo = (ultimo + 1) % TAM_COLA;
}

/*
    Devuelve si la cola está vacía.
*/
uint8_t cola_hay_msg() { return ultimo != primero; }

/*
    Devuelve el siguiente mensaje de la cola.

    m:  Dirección donde almacenar el mensaje obtenido

    La cola de eventos no puede estar vacía, en caso contrario el resultado
    queda indefinido.
*/
void cola_obtener_sig_msg(msg *m)
{
    // Obtiene el evento
    *m = cola[primero].unMsg;

    // Avanza índice
    primero = (primero + 1) % TAM_COLA;
}
