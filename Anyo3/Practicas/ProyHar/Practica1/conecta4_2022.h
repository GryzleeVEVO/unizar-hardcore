/*
	TAD Tablero
*/

#ifndef CONECTA4_H_2022
#define CONECTA4_H_2022

#include <inttypes.h>
#include "celda.h"


/*
	La representación del tablero en memoria es

    0 C1 C2 C3 C4 C5 C6 C7
	F1 x  x  x  x  x  x  x 
	F2 x  x  x  x  x  x  x
	F3 x  x  x  x  x  x  x
	F4 x  x  x  x  x  x  x
	F5 x  x  x  x  x  x  x
	F6 x  x  x  x  x  x  x
*/ 

/*
 	CONSTANTES 
*/

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

/* 
   Declaración de funciones visibles en el exterior conecta4_
*/

// devuelve la longitud de la línea más larga
uint8_t conecta4_buscar_alineamiento_c(CELDA cuadricula[TAM_FILS][TAM_COLS], uint8_t fila, uint8_t columna, uint8_t color, int8_t delta_fila, int8_t delta_columna);

// devuelve la longitud de la línea más larga
uint8_t conecta4_buscar_alineamiento_arm(CELDA cuadricula[TAM_FILS][TAM_COLS], uint8_t fila, uint8_t columna, uint8_t color, int8_t delta_fila, int8_t delta_columna);

// devuelve true si encuentra una línea de longitud mayor o igual a 4
uint8_t conecta4_hay_linea_c_c(CELDA cuadricula[TAM_FILS][TAM_COLS], uint8_t fila, uint8_t columna, uint8_t color);

// devuelve true si encuentra una línea de longitud mayor o igual a 4
uint8_t conecta4_hay_linea_arm_c(CELDA cuadricula[TAM_FILS][TAM_COLS], uint8_t fila, uint8_t columna, uint8_t color);

// devuelve true si encuentra una línea de longitud mayor o igual a 4
uint8_t conecta4_hay_linea_arm_arm(CELDA cuadricula[TAM_FILS][TAM_COLS], uint8_t fila, uint8_t columna, uint8_t color);

// función principal del juego
void conecta4_jugar(void);

/* 
   Declaración funciones internas C4_
*/

uint8_t C4_calcular_fila(CELDA cuadricula[TAM_FILS][TAM_COLS], uint8_t columna);

void C4_actualizar_tablero(CELDA cuadricula[TAM_FILS][TAM_COLS], uint8_t fila, uint8_t columna, uint8_t val);

int C4_comprobar_empate(CELDA cuadricula[TAM_FILS][TAM_COLS]);

int C4_verificar_4_en_linea(CELDA cuadricula[TAM_FILS][TAM_COLS], uint8_t fila, uint8_t columna, uint8_t color);

static inline uint8_t C4_alternar_color(uint8_t colour)
	{ return colour == 1 ? 2 : 1; /* jugador 1 o jugador 2 */ }

static inline uint8_t C4_columna_valida(uint8_t columna)
	{ return (columna >= 1) && (columna <= NUM_COLUMNAS); }

static inline uint8_t C4_fila_valida(uint8_t fila)
	{ return (fila >= 1) && (fila <= NUM_FILAS); }


	
	/*	FUNCIONES EXTERNAS DE ENSAMBLADOR	*/
	
extern uint8_t conecta4_hay_linea_asm_c(
	CELDA cuadricula[TAM_FILS][TAM_COLS],
	uint8_t fila, uint8_t columna, uint8_t color);	

extern uint8_t conecta4_hay_linea_asm_asm(
	CELDA cuadricula[TAM_FILS][TAM_COLS],
	uint8_t fila, uint8_t columna, uint8_t color);

extern uint8_t conecta4_buscar_alineamiento_asm(
	CELDA cuadricula[TAM_FILS][TAM_COLS],
	uint8_t fila, uint8_t columna, uint8_t color, 
	int8_t delta_fila, int8_t delta_columna);

extern uint8_t conecta4_buscar_alineamiento_asm_iter(
	CELDA cuadricula[TAM_FILS][TAM_COLS],
	uint8_t fila, uint8_t columna, uint8_t color, 
	int8_t delta_fila, int8_t delta_columna);
	

#endif /* CONECTA4_H_2022 */
