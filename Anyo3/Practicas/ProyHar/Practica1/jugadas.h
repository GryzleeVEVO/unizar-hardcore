/*
		Proyecto Hardware
		Practica 1
		
		Autores:
				Dorian Boleslaw Wozniak (817570)
				Pablo Latre ... (...)
	
	Descripción:
				El fichero contiene una serie de jugadas 
*/

#ifndef __JUGADAS_H__
#define __JUGADAS_H__

#include "conecta4_2022.h" // Para las constantes del tamaño del tablero

/*
		En cada partida se pueden realizar como máximo (asumiendo que el tablero 
		sea vacío) COLUMNAS x FILAS jugadas
		
		Las posiciones pares corresponden a fichas blancas, las impares a fichas negras
		
		Empiezan blancas. El valor de partida_1[i] es donde coloca la ficha
*/


/*
		Test 1: Se va llenando el tablero tal que al final de las jugadas se haya probado
		que en ningún momento detecta 4 en raya.
		
		
*/
/*
static uint8_t j_test_1[NUM_FILAS * NUM_COLUMNAS] = 
{ 
	1, 6,
	1, 7,
	2, 7,
	2, 6,
	3, 7,
	3, 6,
	3, 5,
	1, 5,
	2, 5,
	7, 2,
	6, 2,
	7, 1,
	6, 1,
	7, 3,
	5, 3,
	5, 1,
	5, 3,
	6, 2
}; 
*/
/*
Test 2: Prueba que se detecten bien fichas de color opuesto
*/

/*
static uint8_t j_test_2[NUM_FILAS * NUM_COLUMNAS] = 
{
	2, 6
}
*/

/*
static uint8_t j_test_3[NUM_FILAS * NUM_COLUMNAS] = 
{
	2, 2,
	6, 6,
	2, 6,
	6, 2,
	1, 2,
	7, 6
};
*/

/*
static uint8_t j_test_4[NUM_FILAS * NUM_COLUMNAS] = 
{
	2, 2,
	7, 2,
	7, 5,
	5, 7,
	5
};
*/

/*
static uint8_t j_test_5[NUM_FILAS * NUM_COLUMNAS] =
{
	2, 2,
	5, 5,
	2, 4,
	1, 1,
	5, 5,
	3
};
*/

static uint8_t j_test_perf[NUM_FILAS * NUM_COLUMNAS] = 
{
4,
4,
4,
4,
4,
1,
5,
6,
6,
6,
6,
1,
4,
2,
2,
2,
2,
1,
1,
1,
2,
1,
2,
3,
6,
3,
6,
3,
3,
5,
5,
7,
3,
3,
7,
7,
7,
7,
5,
5,
7
};

#endif
