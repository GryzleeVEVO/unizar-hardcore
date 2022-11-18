/*
    Proyecto Hardware
    Práctica 2

    Fichero:
        cola_eventos.c

    Autores:
        Dorian Boleslaw Wozniak (817570@unizar.es)
        Pablo Latre Villacampa (778043@unizar.es)

    Descripción: 
        Implementación de una cola circular que almacena eventos, los cuales
        son generados por los periféricos. Además almacena el número de veces
        que han ocurrido. Ofrece operaciones para encolar y desencolar eventos,
        y para comporbar si la cola está vacía
*/

#include "cola_eventos.h"

// Constantes de la cola
enum {
    TAM_COLA = 32
};

// Booleanos
enum {
    TRUE    = 1,
    FALSE   = 0
};

// Un elemento de la cola almacena eventos + veces que se ejecuta el evento
typedef struct ElementoCola {
    evento      unEvento;
    uint32_t    veces;
} elemento ;


// Cola circular, puede ser modificada por acciones concurrentes
static volatile elemento cola[TAM_COLA];
static volatile uint8_t numElem = 0;
static volatile uint8_t primero = 0;
static volatile uint8_t ultimo = 0;

/*
    Añade a la cola un evento nuevo {ID_evento, auxData}. Ademas almacena el
    número de veces que se ha producido el evento para depuración
*/
void cola_encolar_eventos(uint8_t ID_evento, uint32_t veces, uint32_t auxData) {
    // Añade a la cola
    cola[ultimo].unEvento.ID_evento = ID_evento;
    cola[ultimo].unEvento.auxData   = auxData;    
    cola[ultimo].veces              = veces;

    // Avanza índice
    (ultimo < TAM_COLA) ? (ultimo++) : (ultimo = 0);

    // Actualiza cuenta elementos. Si desborda se detiene.
    if (numElem++ > TAM_COLA) while (1); 
}

/* 
    Devuelve TRUE (1) si hay eventos en la cola sin tratar. 
    Devuelve FALSE (0) en caso contrario 
*/
uint8_t cola_hay_eventos() { return (numElem >= 0) ? TRUE : FALSE; }

/*
    Desencola el primer evento sin tratar de la cola. Se asume que hay eventos
    sin tratar en la cola
*/
void cola_obtener_sig_evento(evento *e) {
    // Obtiene el evento
    *e = cola[primero].unEvento;
    
    // Avanza índice
    (primero < TAM_COLA) ? (primero++) : (primero = 0);

    // Reduce cuenta elementos
    numElem--;
}