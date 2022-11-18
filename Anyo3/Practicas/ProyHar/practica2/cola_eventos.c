/*
    Proyecto Hardware
    Práctica 2

    Fichero:
        cola_eventos.c

    Autores:
        Dorian Boleslaw Wozniak (817570@unizar.es)
        Pablo Latre Villacampa (778043@unizar.es)

    Descripción: 
        Implementación de un TAD Cola circular que almacena eventos junto al 
        número de veces que ha ocurrido. Permite encolar, desencolar y 
        comprobar si está vacía
*/

#include "cola_eventos.h"

// Constantes
enum {
    TAM_COLA = 32 // Número de elementos encolados máximo
};

// Booleanos
enum {
    TRUE    = 1,
    FALSE   = 0
};

// Elemento de la cola
typedef struct ElementoCola {
    evento      unEvento;       // Evento almacenado
    uint32_t    veces;          // Numero de evento
} elemento ;


// Cola
static volatile elemento cola[TAM_COLA];

static volatile uint8_t primero = 0;    // Índice al primer elemento en la cola
static volatile uint8_t ultimo  = 0;    // Índice al último elemento encolado
static volatile uint8_t numElem = 0;    // Cuenta de elementos en la cola

/*
    Pre:  numElem < TAM_COLA
    Post: Se encola un nuevo elemento {{ID_evento, auxData}, veces} al final
            de la cola.
          Si numElem >= TAM_COLA, se detiene el programa
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
    Pre:  ---
    Post: Devuelve TRUE (1) si hay eventos en la cola sin tratar. 
          Devuelve FALSE (0) en caso contrario 
*/
uint8_t cola_hay_eventos() { return (numElem > 0) ? TRUE : FALSE; }

/*
    Pre:  numElem > 0, @e != nil
    Post: e contiene el evento almacenado por el primer elemento de la cola
*/
void cola_obtener_sig_evento(evento *e) {
    // Obtiene el evento
    *e = cola[primero].unEvento;
    
    // Avanza índice
    (primero < TAM_COLA) ? (primero++) : (primero = 0);

    // Reduce cuenta elementos
    numElem--;
}
