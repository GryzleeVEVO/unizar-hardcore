/*
    Proyecto Hardware
    Práctica 2

    Fichero:
        cola_msg.c

    Autores:
        Dorian Boleslaw Wozniak (817570@unizar.es)
        Pablo Latre Villacampa (778043@unizar.es)

    Descripción: 
        Implementación de una cola circular que almacena mensajes, los cuales
        son generados por los periféricos. Además almacena el valor del tiempo
        al encolar. Ofrece operaciones para encolar y desencolar eventos,
        y para comporbar si la cola está vacía
*/

#include "cola_msg.h"
#include "tiempo.h"

// Constantes de la cola
enum {
    TAM_COLA = 32
};

// Booleanos
enum {
    TRUE    = 1,
    FALSE   = 0
};

// Un elemento de la cola almacena mensajes + instante de tiempo al encolar
typedef struct ElementoCola {
    msg         unMsg;
    uint32_t    tiempo;
} elemento ;


// Cola circular, puede ser modificada por acciones concurrentes
static volatile elemento cola[TAM_COLA];
static volatile uint8_t numElem = 0;
static volatile uint8_t primero = 0;
static volatile uint8_t ultimo = 0;

/*
    Añade a la cola un evento nuevo {ID_msg, auxData}. Ademas almacena el
    número de veces que se ha producido el evento para depuración
*/
void cola_encolar_msg(uint8_t ID_msg, uint32_t mensaje) {
    // Añade a la cola
    cola[ultimo].unMsg.ID_msg   = ID_msg;
    cola[ultimo].unMsg.mensaje  = mensaje;    
    cola[ultimo].tiempo         = temporizador_leer();

    // Avanza índice
    (ultimo < TAM_COLA) ? (ultimo++) : (ultimo = 0);

    // Actualiza cuenta elementos. Si desborda se detiene.
    if (numElem++ > TAM_COLA) while (1); 
}

/* 
    Devuelve TRUE (1) si hay mensajes en la cola sin tratar. 
    Devuelve FALSE (0) en caso contrario 
*/
uint8_t cola_hay_msg() { return (numElem >= 0) ? TRUE : FALSE; }

/*
    Desencola el primer evento sin tratar de la cola. Se asume que hay mensajes
    sin tratar en la cola
*/
void cola_obtener_sig_evento(msg *m) {
    // Obtiene el evento
    *m = cola[primero].unMsg;
    
    // Avanza índice
    (primero < TAM_COLA) ? (primero++) : (primero = 0);

    // Reduce cuenta elementos
    numElem--;
}