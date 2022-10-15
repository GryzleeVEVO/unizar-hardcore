/*
    Proyecto Hardware
    Práctica 1

    Fichero: conecta4_2022.h

    Autores:
        Dorian Boleslaw Wozniak   (817570@unizar.es)
        Pablo Latre Villacampa    (778543@unizar.es)

    Descripción: Declaraciónes para un juego de conecta 4, principalmente:
		
		- Constantes
		- Funciones internas
		- buscar_alineamiento y hay_linea, tanto versiones en C como en ARM
		- Funciones auxiliares
*/


#ifndef CONECTA4_H_2022
#define CONECTA4_H_2022

#include <inttypes.h>
#include "celda.h"

/* --- CONSTANTES --- */

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

/* --- FUNCIONES INTERNAS --- */

uint8_t C4_calcular_fila(CELDA cuadricula[TAM_FILS][TAM_COLS], uint8_t columna);

void C4_actualizar_tablero(CELDA cuadricula[TAM_FILS][TAM_COLS], 
							uint8_t fila, uint8_t columna, uint8_t val);

int C4_comprobar_empate(CELDA cuadricula[TAM_FILS][TAM_COLS]);

int C4_verificar_4_en_linea(CELDA cuadricula[TAM_FILS][TAM_COLS], 
							uint8_t fila, uint8_t columna, uint8_t color);

/* --- FUNCIONES BUSCAR_ALINEAMIENTO Y HAY_LINEA --- */

uint8_t conecta4_buscar_alineamiento_c(CELDA cuadricula[TAM_FILS][TAM_COLS], 
								uint8_t fila, uint8_t columna, uint8_t color, 
								int8_t delta_fila, int8_t delta_columna);

uint8_t conecta4_buscar_alineamiento_arm(CELDA cuadricula[TAM_FILS][TAM_COLS], 
								uint8_t fila, uint8_t columna, uint8_t color, 
								int8_t delta_fila, int8_t delta_columna);

								

uint8_t conecta4_hay_linea_c_c(CELDA cuadricula[TAM_FILS][TAM_COLS], 
								uint8_t fila, uint8_t columna, uint8_t color);

uint8_t conecta4_hay_linea_c_arm(CELDA cuadricula[TAM_FILS][TAM_COLS], 
								uint8_t fila, uint8_t columna, uint8_t color);

uint8_t conecta4_hay_linea_arm_c(CELDA cuadricula[TAM_FILS][TAM_COLS], 
								uint8_t fila, uint8_t columna, uint8_t color);

uint8_t conecta4_hay_linea_arm_arm(CELDA cuadricula[TAM_FILS][TAM_COLS], 
								uint8_t fila, uint8_t columna, uint8_t color);


/* --- FUNCIÓN PRICIPAL --- */
void conecta4_jugar(void);


/* --- FUNCIONES INTERNAS --- */

// Cambia el color del jugador (1 es blancas, 2 es negras)
static inline uint8_t C4_alternar_color(uint8_t colour)
	{ return colour == 1 ? 2 : 1; }

// Comprueba que la columna esté dentro del tablero
static inline uint8_t C4_columna_valida(uint8_t columna)
	{ return (columna >= 1) && (columna <= NUM_COLUMNAS); }

// Comprueba que la fila esté dentro del tablero
static inline uint8_t C4_fila_valida(uint8_t fila)
	{ return (fila >= 1) && (fila <= NUM_FILAS); }

#endif /* CONECTA4_H_2022 */
