/*
    Proyecto Hardware
    Práctica 1

    Fichero: jugadas.h

    Autores:
        Dorian Boleslaw Wozniak   (817570@unizar.es)
        Pablo Latre Villacampa    (778543@unizar.es)

    Descripción: Implementación de la automatización del juego conecta 4 para
					depuración y medicion del rendimiento
*/

#ifndef JUGADAS_H
#define JUGADAS_H

#include "conecta4_2022.h" // Solo se usan constantes

/*
	Seleccionar jugadas a realizar aquí. Las jugadas disponibles son:

	- TEST_1: Comprueba que no se detectan celdas vacías, fuera de rango y
				de otro color como una celda del mismo color. Al acabar no
				debería detectar una jugada ganadora. Utilizar con tabla
				TABLA_TEST_1
	- TEST_2: Comprueba que no se detectan celdas de color diferente como 
				celdas del mismo color. Utilizar con tabla TABLA_TEST_2
	- TEST_3: Comprueba que detecta correctamente victoria al colocar una pieza
				en el extemo de una diagonal. Utilizar con TABLA_TEST_3
	- TEST_4: Comprueba que detecta correctamente victoria al colocar una pieza
				en medio de una diagonal. Utilizar con TABLA_TEST_4
	- TEST_5: Comprueba que detecta correctamente victoria al colocar una pieza
				en un extremo y en medio de una línea horizontal, y al colocarla
				en una línea vertical. Utilizara con TABLA_TEST_5
	- TEST_PERF: Partida normal para medir rendimiento. Usar con TABLA_VACIA
*/
#define TEST_3

/* 
	Algunas jugadas también definiran un símbolo UNDO_IF_WIN si se está 
	comprobando que se detecte una línea ganadora correctamente
*/

#ifdef TEST_1
#define JUGADA j_test_1
static uint8_t j_test_1[NUM_FILAS * NUM_COLUMNAS] = { 
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
	6, 2 }; 
#endif // TEST_1

#ifdef TEST_2
#define JUGADA j_test_2
static uint8_t j_test_2[NUM_FILAS * NUM_COLUMNAS] = {
	2, 6 };
#endif // TEST_2

#ifdef TEST_3
#define JUGADA j_test_3
#define UNDO_IF_WIN
static uint8_t j_test_3[NUM_FILAS * NUM_COLUMNAS] = {
	2, 2,
	6, 6,
	2, 6,
	6, 2,
	1, 2,
	7, 6 };
#endif // TEST_3

#ifdef TEST_4
#define JUGADA j_test_4
#define UNDO_IF_WIN
static uint8_t j_test_4[NUM_FILAS * NUM_COLUMNAS] = {
	2, 2,
	7, 2,
	7, 5,
	5, 7,
	5     };
#endif // TEST_4


#ifdef TEST_5
#define JUGADA j_test_5
#define UNDO_IF_WIN
static uint8_t j_test_5[NUM_FILAS * NUM_COLUMNAS] =
{
	2, 2,
	5, 5,
	2, 4,
	1, 1,
	5, 5,
	3     };
#endif // TEST_5

#ifdef TEST_PERF
#define JUGADA j_test_perf
static uint8_t j_test_perf[NUM_FILAS * NUM_COLUMNAS] = {
	4,4,
	4,4,
	4,1,
	5,6,
	6,6,
	6,1,
	4,2,
	2,2,
	2,1,
	1,1,
	2,1,
	2,3,
	6,3,
	6,3,
	3,5,
	5,7,
	3,3,
	7,7,
	7,7,
	5,5,
	7 };
#endif // TEST_PERF

#endif 	// JUGADAS_H
