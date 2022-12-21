
#include "cola_buffer.h"
#include "G_IO.h"

// Constantes
enum {
    TAM_COLA = 1024   // Tamaño de la cola
};

static int8_t cola[TAM_COLA];     // Cola
static uint32_t primero = 0;        // Índice al primer elemento
static uint32_t ultimo  = 0;        // Índice al último elemento


void buffer_anyadir(char* c) { 
    for (uint32_t i = 0; c[i] != '\0'; i++) {
        // Si desborda se detiene.
        if ((ultimo + 1) % TAM_COLA == primero) {
            IO_marcar_overflow();
            while (1); 
        }
        
        // Añade a la cola
        cola[ultimo] = c[i];

        // Avanza índice
        ultimo = (ultimo + 1) % TAM_COLA;
    }
}

uint8_t buffer_vacio() { return ultimo == primero; }

int8_t buffer_siguiente() {
    // Obtiene el evento
    uint8_t c = cola[primero];
    
    // Avanza índice
    primero = (primero + 1) % TAM_COLA;
    
    return c;
}
