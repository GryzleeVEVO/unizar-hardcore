/*
    Proyecto Hardware
    Práctica 2

    Fichero:
        cola_msg.c

    Autores:
        Dorian Boleslaw Wozniak (817570@unizar.es)
        Pablo Latre Villacampa (778043@unizar.es)

    Descripción: 
        Implementación de un TAD Cola circular que almacena mensajes junto al 
        instante de tiempo al encolar. Permite encolar, desencolar y comprobar
        si está vacía
*/

#include "cola_msg.h"
#include "tiempo.h"
#include "G_IO.h"

// Constantes
enum {
    TAM_COLA = 32
};

// Booleanos
enum {
    TRUE    = 1,
    FALSE   = 0
};

// Elemento de la cola
typedef struct ElementoCola {
    msg         unMsg;          // Mensaje almacenado 
    uint32_t    tiempo;         // Tiempo al encolar (us)
} elemento ;


// Cola
static volatile elemento cola[TAM_COLA];

static volatile uint8_t numElem = 0;    // Índice al primer elemento de la cola
static volatile uint8_t primero = 0;    // Índice al último elemento de la cola
static volatile uint8_t ultimo = 0;     // Cuenta de elementos en la cola

/*
    Pre:  numElem < TAM_COLA
    Post: Se encola un nuevo elemento {{ID_msg, mensaje}, <tiempo actual>} al 
            final de la cola.
          Si numElem >= TAM_COLA, se detiene el programa
*/
void cola_msg(uint8_t ID_msg, uint32_t mensaje) {
    
    // Añade a la cola
    cola[ultimo].unMsg.ID_msg   = ID_msg;
    cola[ultimo].unMsg.mensaje  = mensaje;    
    cola[ultimo].tiempo         = temporizador_leer();

    // Avanza índice
    if (++ultimo >= TAM_COLA) ultimo = 0;

    // Actualiza cuenta elementos. Si desborda se detiene.
    
    if (++numElem > TAM_COLA) {
        IO_marcar_overflow();
        while (1); 
    }
}

/* 
    Pre:  ---
    Post: Devuelve TRUE (1) si hay eventos en la cola sin tratar. 
          Devuelve FALSE (0) en caso contrario 
*/
uint8_t cola_hay_msg() { return (numElem > 0) ? TRUE : FALSE; }

/*
    Pre:  numElem > 0, @m != nil
    Post: m contiene el mensaje almacenado por el primer elemento de la cola
*/
void cola_obtener_sig_msg(msg *m) {
    
    // Obtiene el evento
    *m = cola[primero].unMsg;
    
    // Avanza índice
    if (++primero >= TAM_COLA) primero = 0;

    // Reduce cuenta elementos
    numElem--;
}
