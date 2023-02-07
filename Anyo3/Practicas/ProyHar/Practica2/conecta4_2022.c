/*
    Proyecto Hardware
    Práctica 2

    Fichero: conecta4_2022.c

    Autores:
        Dorian Boleslaw Wozniak   (817570@unizar.es)
        Pablo Latre Villacampa    (778043@unizar.es)

    Descripción: Implementación de un juego conecta 4, adaptado para poder jugar
        con periféricos, con lógica del juego gestionada por un planificador
        basado en eventos
*/
#include "tableros.h"           // tablero = cuadricula[][]
#include "conecta4_2022.h"
#include "celda.h"
#include "cola_msg.h"

// Estado del juego


static uint8_t color = 1;      // Jugador actual
static uint8_t fin = 0;        // Fin de juego
static uint8_t columna = 0;    // Columna a insertar
static uint8_t fila = 0;       // Fila a insertar


// Introduce una ficha de color val en @cuadricula[fila][columna]
void C4_actualizar_tablero(uint8_t f, uint8_t c)
    { celda_poner_valor(&cuadricula[(f)][(c)], color); }



// Devuelve verdad sólo si el tablero está lleno sin que ningún jugador
// haya ganado
int C4_comprobar_empate() {
    for (uint8_t i = 1; i <= NUM_COLUMNAS; i++)
        if (celda_vacia(cuadricula[NUM_FILAS][i]))
            return FALSE;
    return TRUE;
}


uint8_t conecta4_buscar_alineamiento(uint8_t f, uint8_t c, 
    int8_t delta_fila, int8_t delta_columna) {

    // Acaba si la posición está fuera del tablero
    if (!C4_fila_valida(f) || !C4_columna_valida(c)) return 0;

    // Acaba si la celda es vacía o de otro color
    if (celda_vacia(cuadricula[f][c]) 
        || (celda_color(cuadricula[f][c]) != color))
        return 0;

    // Avanza índices
    uint8_t nueva_fila = f + delta_fila;
    uint8_t nueva_columna = c + delta_columna;

    // Añade 1 y llamada recursiva
    return 1 + conecta4_buscar_alineamiento(nueva_fila, nueva_columna, delta_fila, delta_columna);
}

uint8_t conecta4_hay_linea(uint8_t f, uint8_t c) {
    // Deltas
    int8_t deltas_fila[4] = {0, -1, -1, 1};
    int8_t deltas_columna[4] = {-1, 0, -1, -1};

    // Variables
    unsigned int i = 0;     // Índice
    uint8_t linea = FALSE;  // Se ha encontrado línea
    uint8_t long_linea = 0; // Tamaño de línea

    for (i = 0; (i < 4) && (linea == FALSE); ++i) {
        // Comprueba en dirección de las deltas
        long_linea = conecta4_buscar_alineamiento(
                        f, c, 
                        deltas_fila[i], deltas_columna[i]);

        linea = long_linea >= 4;
        if (linea) continue;

        // Comprueba en dirección contraria, suma a la línea encontrada antes
        long_linea += 
            conecta4_buscar_alineamiento
                (f - deltas_fila[i], c - deltas_columna[i],
                -deltas_fila[i], -deltas_columna[i]);

        linea = long_linea >= 4;
    }

    return linea;
}

/*
    Pre: ---
    Post: Inicializa el estado del juego
*/
void conecta4_iniciar() {
    // Limpia el tablero
    for (uint8_t i = 1; i <= NUM_FILAS; i++)
        for (uint8_t j = 1; j <= NUM_COLUMNAS; j++)
            celda_borrar_valor(&cuadricula[i][j]);
    
    color = 1; fin = 0; columna = 0; fila = 0;
}

/*
    Pre: 1 <= columna <= NUM_COLUMNAS
    Post: Devuelve si se inserta o no una ficha en la columna dada
*/
uint8_t conecta4_comprobar_columna(uint8_t c) {
    uint8_t f = 1;
    
    if ((c < 1) || (c > NUM_COLUMNAS)) return FALSE; 

    while ((f <= NUM_FILAS) 
            && (celda_vacia(cuadricula[f][c]) == FALSE))
        f++;
    
    if (f <= NUM_FILAS) {
        fila = f;
        columna = c;
        return TRUE;
    } else { return FALSE; }
}

/*
    Pre: 1 <= fila <= NUM_FILAS, 1 <= columna <= NUM_COLUMNAS
    Post: Realiza la jugada dados la fila y columna donde insertar una ficha.
            Devuelve el estado del juego tras la introducción 
            (pasar turno / victoria blancas / victoria negras / empate)
*/
uint8_t conecta4_jugar() {
    
    C4_actualizar_tablero(fila, columna);
    
    if (conecta4_hay_linea(fila, columna)) {
        fin = 1;
        return color;
    } 
    else if (C4_comprobar_empate()) {
        fin = 1;
        return C4_EMPATE;
    }
    else {
        color = C4_alternar_color(color);
        return C4_SEGUIR;
    }
}

uint8_t conecta4_detenido() { return fin; }
