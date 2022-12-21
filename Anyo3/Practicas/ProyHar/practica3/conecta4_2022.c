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
#include "G_IO.h"
#include "G_Alarm.h"


/* Tamaños de la cuadricula */
enum {
    NUM_FILAS       = 6,
    PADDING_FIL     = 1,
    TAM_FILS        = PADDING_FIL + NUM_FILAS,
    NUM_COLUMNAS    = 7,
    PADDING_COL     = 1,
    TAM_COLS        = PADDING_COL + NUM_COLUMNAS
};

// Estado del juego
static uint8_t color = 1;      // Jugador actual
static uint8_t fin = 0;        // Fin de juego
static uint8_t columna = 0;    // Columna a insertar
static uint8_t fila = 0;       // Fila a insertar

// Cambia el color del jugador (1 es blancas, 2 es negras)
static inline uint8_t C4_alternar_color(uint8_t colour)
    { return colour == 1 ? 2 : 1; }

// Comprueba que la columna esté dentro del tablero
static inline uint8_t C4_columna_valida(uint8_t columna)
    { return (columna >= 1) && (columna <= NUM_COLUMNAS); }

// Comprueba que la fila esté dentro del tablero
static inline uint8_t C4_fila_valida(uint8_t fila)
    { return (fila >= 1) && (fila <= NUM_FILAS); }

// Introduce una ficha de color val en @cuadricula[fila][columna]
void C4_actualizar_tablero(uint8_t f, uint8_t c)
    { celda_poner_valor(&cuadricula[(f)][(c)], color); }

// Devuelve verdad sólo si el tablero está lleno sin que ningún jugador
// haya ganado
int C4_comprobar_empate() {
    for (uint8_t i = 1; i <= NUM_COLUMNAS; i++)
        if (celda_vacia(cuadricula[NUM_FILAS][i]))
            return 0;
    return 1;
}

// Lee la entrada y obtiene la columna y fila donde realizar la jugada
uint8_t C4_comprobar_columna(uint8_t entrada) {
    // Obtiene la columna a leer. Si hay dos pines marcados lo indicará
    uint8_t c = 0;
    
    for (uint8_t i = 1; i <= NUM_COLUMNAS; i++) {
        if ((entrada & 0x1) == 0) {
            if (c == 0) { c = i; }
            else { return 0; }
        }
        entrada >>= 1;
    }
    
    uint8_t f = 1;
    
    if (C4_columna_valida(c)) {
        while ((f <= NUM_FILAS) && (!celda_vacia(cuadricula[f][c]))) { f++; }
        
        if (C4_fila_valida(f)) {
            fila = f;
            columna = c;
            return 1;
        }
    }
    
    return 0;
}

// Devuelve el número de celdas con fichas del mismo color seguidas en una 
// línea dada una posición y una dirección (delta)
uint8_t C4_buscar_alineamiento(uint8_t f, uint8_t c, 
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
    return 1 + C4_buscar_alineamiento(nueva_fila, nueva_columna, delta_fila,
        delta_columna);
}

// Devuelve si hay una línea que contenga cuatro o mas fichas del mismo color a
// partir de una posición
uint8_t C4_hay_linea(uint8_t f, uint8_t c) {
    // Deltas
    int8_t deltas_fila[4] = {0, -1, -1, 1};
    int8_t deltas_columna[4] = {-1, 0, -1, -1};

    // Variables
    unsigned int i = 0;     // Índice
    uint8_t linea = 0;  // Se ha encontrado línea
    uint8_t long_linea = 0; // Tamaño de línea

    for (i = 0; (i < 4) && (linea == 0); ++i) {
        // Comprueba en dirección de las deltas
        long_linea = C4_buscar_alineamiento(
                        f, c, 
                        deltas_fila[i], deltas_columna[i]);

        linea = long_linea >= 4;
        if (linea) continue;

        // Comprueba en dirección contraria, suma a la línea encontrada antes
        long_linea += 
            C4_buscar_alineamiento
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

// Devuelve si la entrada ha cambiado a partir de la entrada dada
uint8_t conecta4_comprobar_entrada() {
    static uint8_t ultima_entrada = 0;
    
    uint8_t res = 0, entrada = IO_leer_entrada();
    
    // Mira si la entrada
    if (entrada != ultima_entrada) {
        ultima_entrada = entrada;
        res = 1;
    }
    
    if (C4_comprobar_columna(entrada)) { IO_indicar_jugada_valida(); } 
    else { IO_indicar_jugada_invalida(); }
    
    return res;
}

// Lee la entrada y, si es correcta, realiza la jugada dada
void conecta4_realizar_jugada() {
    if (!fin && C4_comprobar_columna(IO_leer_entrada())) {
        C4_actualizar_tablero(fila, columna);

        if (C4_hay_linea(fila, columna)) {
            fin = 1;
            IO_victoria();
        } 
        else if (C4_comprobar_empate()) {
            fin = 1;
            IO_empate();
        }
        else {
            color = C4_alternar_color(color);
            IO_jugada_realizada();
            cola_msg(SET_ALARMA, ALARM_OFF_REALIZAR_MSG);;
        }  
    }  
}

