/*
	Conecta 4
*/

#include "conecta4_2022.h"	// Declaraciones y constantes
#include "entrada.h"				// Para control E/S


/*
	Devuelve la fila en la que coloca la ficha solo si
		- La columna está dentro del tablero
		- La fila donde coloca la 
*/
uint8_t 
C4_calcular_fila(CELDA cuadricula[TAM_FILS][TAM_COLS], uint8_t columna) {

	uint8_t fila = 1;

	/* La columna debe estar entre 1 y NUM_COLUMNAS (7) */
	if ((columna < 1) || (columna > NUM_COLUMNAS)) return ERROR; 

	/* Itera hasta encontrar una fila con la celda vacía o llegar al fin del tablero*/
	while ((fila <= NUM_FILAS) && (celda_vacia(cuadricula[fila][columna]) == FALSE))
		fila++;
	
	/* Devuelve la fila donde la ha colocado o error si ha salido del tablero */
	return fila <= NUM_FILAS ? fila : ERROR;
}

/*

	FUNCIONES DE JUEGO EN C

*/

/*
	Obtiene el largo de la línea de fichas del mismo color más larga

	Deltas son desfase de índices para la cada llamada seguida
*/
uint8_t conecta4_buscar_alineamiento_c
	(CELDA cuadricula[TAM_FILS][TAM_COLS],
	uint8_t fila, uint8_t columna, uint8_t color, 
	int8_t delta_fila, int8_t delta_columna) {

	/* Comprueba que la posición actual este dentro del tablero*/
	if (!C4_fila_valida(fila) || !C4_columna_valida(columna)) return 0;

	/* Comprueba que la celda actual contenga una ficha del mismo color */
	if (celda_vacia(cuadricula[fila][columna]) || (celda_color(cuadricula[fila][columna]) != color))
		return 0;

	/* Avanza índices */
	uint8_t nueva_fila = fila + delta_fila;
	uint8_t nueva_columna = columna + delta_columna;

	/* Comprueba siguiente línea aplicando los deltas */
	return 1 + conecta4_buscar_alineamiento_c(cuadricula, nueva_fila, nueva_columna, color, delta_fila, delta_columna);
}



/******************************************************************************/
/* CONECTA_4_HAY_LINEA																												*/
/******************************************************************************/

/*
		Devuelve TRUE si ha encontrado una línea con 4 celdas consecutivas o mas
		del mismo color dada una celda inicial cuadricula[fila][columna]

		En caso contrario devuelve FALSE
*/


// C + C
uint8_t conecta4_hay_linea_c_c(CELDA cuadricula[TAM_FILS][TAM_COLS], 
								uint8_t fila, uint8_t columna, uint8_t color) {
	/*
		Deltas indican las direcciones que comprobará si hay conecta 4

		Estas son:
			- Izquierda
			- Arriba
			- Diagonal inferior izquierda
			- Diagonal superior izquierda

		Luego se calcula invirtiendo los elementos
			- Derecha
			- Abajo
			- Diagonal inferior derecha
			- Diahonal superior derecha
	*/
	int8_t deltas_fila[4] = {0, -1, -1, 1};
	int8_t deltas_columna[4] = {-1, 0, -1, -1};

	/* Itera */
	unsigned int i = 0;
	uint8_t linea = FALSE;
	uint8_t long_linea = 0;

	/* Busca linea en fila, columna y diagonales */
	for (i = 0; (i < 4) && (linea == FALSE); ++i) {

		// Buscar sentido normal
		long_linea = conecta4_buscar_alineamiento_c
			(cuadricula, 
			fila, columna, color, 
			deltas_fila[i], deltas_columna[i]);

		// Guarda camino mas largo y prueba si ya ha encontrado 4 en raya
		linea = long_linea >= 4;
		if (linea) continue;

		// Buscar sentido inverso
		long_linea += conecta4_buscar_alineamiento_c
			(cuadricula, 
			fila - deltas_fila[i], columna - deltas_columna[i], color, 
			-deltas_fila[i], -deltas_columna[i]);

		// Prueba si sumando los caminos mas largos es mayor que 4
		linea = long_linea >= 4;
	}

	return linea;
}
				

/*			C + ASM																															 */
uint8_t conecta4_hay_linea_c_asm(CELDA cuadricula[TAM_FILS][TAM_COLS], 
								uint8_t fila, uint8_t columna, uint8_t color) {

	int8_t deltas_fila[4] = {0, -1, -1, 1};
	int8_t deltas_columna[4] = {-1, 0, -1, -1};

	unsigned int i = 0;
	uint8_t linea = FALSE;
	uint8_t long_linea = 0;

	for (i = 0; (i < 4) && (linea == FALSE); ++i) {

		long_linea = conecta4_buscar_alineamiento_asm
			(cuadricula, 
			fila, columna, color, 
			deltas_fila[i], deltas_columna[i]);

		// Guarda camino mas largo y prueba si ya ha encontrado 4 en raya
		linea = long_linea >= 4;
		if (linea) continue;

		// Buscar sentido inverso
		long_linea += conecta4_buscar_alineamiento_asm
			(cuadricula, 
			fila - deltas_fila[i], columna - deltas_columna[i], color, 
			-deltas_fila[i], -deltas_columna[i]);

		// Prueba si sumando los caminos mas largos es mayor que 4
		linea = long_linea >= 4;
	}

	return linea;
}
								

/*			C + ASM	iterativo																										 */
uint8_t conecta4_hay_linea_c_asm_iter(CELDA cuadricula[TAM_FILS][TAM_COLS], 
								uint8_t fila, uint8_t columna, uint8_t color) {

	int8_t deltas_fila[4] = {0, -1, -1, 1};
	int8_t deltas_columna[4] = {-1, 0, -1, -1};

	unsigned int i = 0;
	uint8_t linea = FALSE;
	uint8_t long_linea = 0;

	for (i = 0; (i < 4) && (linea == FALSE); ++i) {

		long_linea = conecta4_buscar_alineamiento_asm
			(cuadricula, 
			fila, columna, color, 
			deltas_fila[i], deltas_columna[i]);

		// Guarda camino mas largo y prueba si ya ha encontrado 4 en raya
		linea = long_linea >= 4;
		if (linea) continue;

		// Buscar sentido inverso
		long_linea += conecta4_buscar_alineamiento_asm_iter
			(cuadricula, 
			fila - deltas_fila[i], columna - deltas_columna[i], color, 
			-deltas_fila[i], -deltas_columna[i]);

		// Prueba si sumando los caminos mas largos es mayor que 4
		linea = long_linea >= 4;
	}

	return linea;
}

/******************************************************************************/


/* Actualiza el tablero si la jugada dada es válida */
void C4_actualizar_tablero(CELDA cuadricula[TAM_FILS][TAM_COLS], 
							uint8_t fila, uint8_t columna, uint8_t val)

	{ celda_poner_valor(&cuadricula[(fila)][(columna)], val); }

/*
	MODIFICADO

	Devuelve verdad solo si todas las columnas de la última fila tienen ficha
*/
int C4_comprobar_empate(CELDA cuadricula[TAM_FILS][TAM_COLS]) {

	for (uint8_t i = 1; i <= NUM_COLUMNAS; i++)
		if (celda_vacia(cuadricula[NUM_FILAS][i]))
			return 0;
	return 1;
}

/*
	Llama a la función para verificar si hay 4 en raya

	MODIFICAR A QUÉ FUCIÓN LLAMA AQUÍ
*/
int C4_verificar_4_en_linea(CELDA cuadricula[TAM_FILS][TAM_COLS], 
							uint8_t fila, uint8_t columna, uint8_t color)
{
	//uint8_t resultado_c_c = conecta4_hay_linea_c_c(cuadricula, fila, columna, color);
	//uint8_t resultado_c_asm = conecta4_hay_linea_c_asm(cuadricula, fila, columna, color);
	//uint8_t resultado_asm_c = conecta4_hay_linea_asm_c(cuadricula, fila, columna, color);
	//uint8_t resultado_asm_asm = conecta4_hay_linea_asm_asm(cuadricula, fila, columna, color);
	
	uint8_t resultado_c_asm_iter = conecta4_hay_linea_c_asm_iter(cuadricula, fila, columna, color);

	
	//if (resultado_c_c != resultado_c_asm || resultado_c_c != resultado_asm_c || resultado_c_c != resultado_asm_asm)
	//	while(1);
	
	//return resultado_c_c;
	//return resultado_c_asm;
	//return resultado_asm_c;
	//return resultado_asm_asm;
	
	return resultado_c_asm_iter;
}

/* Función principal */
void conecta4_jugar(void)
{

	static volatile uint8_t entrada[8] = {0, 0, 0, 0, 0, 0, 0, 0};

	// Tablero a usar
	#include "tableros.h" 

	// Vector de movimientos
	#include "jugadas.h" 
	
	// Nombre del tablero y jugadas a usar. Solo debe haber un tablero y vector en memoria
	#define CUADRICULA cuadricula_2
	#define PARTIDA j_test_perf
	//#define __DESHACER_JUGADA_GANADORA__

	// Empiezan blancas
	uint8_t column, row, colour;
	colour = 1; 
	
	#ifdef __JUGADAS_H__ 
		uint8_t i = 0; 
	#endif
	
	// Empieza el juego
	while (1) {

		// Esta parte solo se ejecuta si se incluye jugadas.h
		#ifdef __JUGADAS_H__
			// Si hay siguiente movimento (!= 0) lo introduce
			if (PARTIDA[i] != 0) 
				{ entrada[0] = 1; entrada[1] = PARTIDA[i]; i++; }
		
			// En caso contrario, pasa a control manual
		#endif
			
		// Se detiene aquí si se juega manualmente
		while (entrada_nueva(entrada) == 0);

		// Obtiene columna simunistrada
		column = entrada_leer(entrada);

		// Si la columna es correcta y no esta llena, devuelve fila donde colocar la ficha
		row = C4_calcular_fila(CUADRICULA, column);

		// Comprueba que la jugada sea válida
		if (C4_fila_valida(row) && C4_columna_valida(column)) {

			// Actualiza el tablero
			C4_actualizar_tablero(CUADRICULA, row, column, colour);

			/* Si hay 4 en raya, se queda aquí */
			if (C4_verificar_4_en_linea(CUADRICULA, row, column, colour))
					#ifndef __DESHACER_JUGADA_GANADORA__
					while(1);
					#else
					celda_borrar_valor(&CUADRICULA[(row)][(column)]);
					#endif
			
					
			/* Si hay empate, se queda aquí */
			if (C4_comprobar_empate(CUADRICULA))
				while (1);

			/* Si no es fin de juego, pasa turno */
			colour = C4_alternar_color(colour);
		}

		/* Limpia entrada anterior para obtener siguiente movimiento */
		entrada_inicializar(entrada);
	}
}
