/*
    Proyecto Hardware
    Práctica 2

    Fichero: conecta4_2022.h

    Autores:
        Dorian Boleslaw Wozniak   (817570@unizar.es)
        Pablo Latre Villacampa    (778043@unizar.es)

    Descripción: Declaraciónes para un juego de conecta 4 adaptado para la 
        práctica 2.
*/

#ifndef CONECTA4_H_2022
#define CONECTA4_H_2022

#include <inttypes.h>

/* Tamaños de la cuadricula */
enum {
    NUM_FILAS = 6,
    PADDING_FIL = 1,
    TAM_FILS = PADDING_FIL + NUM_FILAS,
    NUM_COLUMNAS = 7,
    PADDING_COL = 1,
    TAM_COLS = PADDING_COL + NUM_COLUMNAS
};

/* Definiciones para valores muy utilizados */
enum {
    FALSE = 0,
    TRUE = 1,
    ERROR = 0xFF
};

/* Estados tras realizar jugada */
enum {
    C4_SEGUIR       = 0,
    C4_VICTORIA_B   = 1,
    C4_VICTORIA_N   = 2,
    C4_EMPATE       = 3
};

// Cambia el color del jugador (1 es blancas, 2 es negras)
static inline uint8_t C4_alternar_color(uint8_t colour)
    { return colour == 1 ? 2 : 1; }

// Comprueba que la columna esté dentro del tablero
static inline uint8_t C4_columna_valida(uint8_t columna)
    { return (columna >= 1) && (columna <= NUM_COLUMNAS); }

// Comprueba que la fila esté dentro del tablero
static inline uint8_t C4_fila_valida(uint8_t fila)
    { return (fila >= 1) && (fila <= NUM_FILAS); }

/*
    Pre: ---
    Post: Inicializa el tablero de juego y prepara el sistema para el juego
*/
void conecta4_iniciar(void);

/*
    Pre: 1 <= columna <= NUM_COLUMNAS
    Post: Devuelve la fila en la que se puede introducir una ficha, o ERROR
            si la fila o columna dada se encuentran fuera del rango
*/
uint8_t conecta4_comprobar_columna(uint8_t columna);
    
/*
    Pre: 1 <= fila <= NUM_FILAS, 1 <= columna <= NUM_COLUMNAS
    Post: Realiza la jugada dados la fila y columna donde insertar una ficha.
            Devuelve el estado del juego tras la introducción 
            (pasar turno / victoria blancas / victoria negras / empate)
*/
uint8_t conecta4_jugar(void);

uint8_t conecta4_detenido(void);

#endif // CONECTA4_H_2022

