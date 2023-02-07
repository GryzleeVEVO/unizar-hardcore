/*
    Proyecto Hardware
    Práctica 3

    Fichero:
        cola_buffer.c

    Autores:
        Dorian Boleslaw Wozniak (817570@unizar.es)
        Pablo Latre Villacampa (778043@unizar.es)

    Descripción: 
        Implementación de un TAD Cola circular que actúa de búfer de 
        caracteres. Permite añadir cadenas de caracteres al búfer, comprobar
        si está vacío, y obtener el siguiente carácter a desencolar.
*/

#include "cola_buffer.h"
#include "G_IO.h"

// Constantes
enum {
    TAM_COLA = 1024   // Tamaño de la cola
};

static int8_t cola[TAM_COLA];       // Cola
static uint32_t primero = 0;        // Índice al primer elemento
static uint32_t ultimo  = 0;        // Índice al último elemento

/*
    Añade una cadena de caracteres a la cola

    c: Cadena de caracteres a introducir

    Si se desborda, para el programa. Las cadenas deben estar terminadas en NULL
*/
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

/*
    Devuelve si el búfer está vacío
*/
uint8_t buffer_vacio() { return ultimo == primero; }

/*
    Devuelve el siguiente caracter almacenado en el búfer

    El búfer no debe estar vacío al extraer
*/
int8_t buffer_siguiente() {
    // Obtiene el evento
    uint8_t c = cola[primero];
    
    // Avanza índice
    primero = (primero + 1) % TAM_COLA;
    
    return c;
}
