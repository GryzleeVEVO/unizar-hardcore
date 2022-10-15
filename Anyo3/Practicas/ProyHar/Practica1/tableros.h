/*
    Proyecto Hardware
    Práctica 1

    Fichero: tableros.h

    Autores:
        Dorian Boleslaw Wozniak   (817570@unizar.es)
        Pablo Latre Villacampa    (778543@unizar.es)

    Descripción: Implementación de tableros con partidas en diferentes estados
					iniciales
*/

#ifndef TABLEROS_H
#define TABLEROS_H

#include "celda.h"

/*
	Seleccionar tabla inicial aquí. Las tablas disponibles son:

	- TABLA_VACIA
	- TABLA_INICIADA: Tabla igual a la dada en el enunciado
	- TABLERO_TEST_1: Usar con TEST_1 de jugadas.h
	- TABLERO_TEST_2: Usar con TEST_2 de jugadas.h
	- TABLERO_TEST_3: Usar con TEST_3 de jugadas.h
	- TABLERO_TEST_4: Usar con TEST_4 de jugadas.h
	- TABLERO_TEST_5: Usar con TEST_5 de jugadas.h
*/
#define TABLA_VACIA

#ifdef TABLA_INICIADA
#define TABLERO cuadricula_iniciada
static CELDA cuadricula_iniciada[7][8] = {
            0, 0XC1, 0XC2, 0XC3, 0XC4, 0XC5, 0Xc6, 0XC7,
            0XF1, 6, 5, 6, 5, 6, 6, 0,
            0XF2, 5, 6, 5, 5, 0, 5, 0,
            0XF3, 6, 0, 0, 6, 0, 5, 0,
            0XF4, 5, 0, 0, 6, 0, 5, 0,
            0XF5, 6, 0, 0, 5, 0, 0, 0,
            0XF6, 0, 0, 0, 6, 0, 0, 0 };
#endif // TABLA_INICIADA
				
#ifdef TABLA_VACIA
#define TABLERO cuadricula_vacia
static CELDA cuadricula_vacia[7][8] = {
            0, 0XC1, 0XC2, 0XC3, 0XC4, 0XC5, 0Xc6, 0XC7,
            0XF1, 0, 0, 0, 0, 0, 0, 0,
            0XF2, 0, 0, 0, 0, 0, 0, 0,
            0XF3, 0, 0, 0, 0, 0, 0, 0,
            0XF4, 0, 0, 0, 0, 0, 0, 0,
            0XF5, 0, 0, 0, 0, 0, 0, 0,
            0XF6, 0, 0, 0, 0, 0, 0, 0 };
#endif // TABLA_VACIA

#ifdef TABLA_TEST_1
#define TABLERO c_test_1
static CELDA  c_test_1[7][8] = {
			0, 0XC1, 0XC2, 0XC3, 0XC4, 0XC5, 0Xc6, 0XC7,
			0XF1, 0, 0, 0, 0, 0, 0, 0,
			0XF2, 0, 0, 0, 0, 0, 0, 0,
			0XF3, 0, 0, 0, 0, 0, 0, 0,
			0XF4, 0, 0, 0, 0, 0, 0, 0,
			0XF5, 0, 0, 0, 0, 0, 0, 0,
			0XF6, 0, 0, 0, 0, 0, 0, 0 };
#endif // TABLA_TEST_1


#ifdef TABLA_TEST_2
#define TABLERO c_test_2
static CELDA c_test_2[7][8] = {
			0, 0XC1, 0XC2, 0XC3, 0XC4, 0XC5, 0Xc6, 0XC7,
			0XF1, 6, 6, 6, 0, 5, 5, 5,
			0XF2, 6, 0, 6, 0, 5, 0, 5,
			0XF3, 6, 0, 6, 0, 5, 0, 5,
			0XF4, 0, 0, 0, 0, 0, 0, 0,
			0XF5, 0, 0, 0, 0, 0, 0, 0,
			0XF6, 0, 0, 0, 0, 0, 0, 0 };
#endif // TABLA_TEST_2

#ifdef TABLA_TEST_3
#define TABLERO c_test_3
static CELDA c_test_3[7][8] = {
			0, 0XC1, 0XC2, 0XC3, 0XC4, 0XC5, 0Xc6, 0XC7,
			0XF1, 0, 5, 6, 5, 6, 5, 0,
			0XF2, 0, 0, 6, 5, 6, 0, 0,
			0XF3, 0, 0, 5, 6, 5, 0, 0,
			0XF4, 0, 0, 6, 5, 6, 0, 0,
			0XF5, 0, 0, 5, 0, 5, 0, 0,
			0XF6, 0, 0, 0, 0, 0, 0, 0};
#endif // TABLA_TEST_3

#ifdef TABLA_TEST_4
#define TABLERO c_test_4
static CELDA c_test_4[7][8] = {
			0, 0XC1, 0XC2, 0XC3, 0XC4, 0XC5, 0Xc6, 0XC7,
			0XF1, 5, 5, 5, 6, 5, 6, 0,
			0XF2, 6, 0, 6, 5, 0, 6, 0,
			0XF3, 5, 0, 5, 6, 0, 5, 0,
			0XF4, 6, 0, 6, 5, 0, 5, 0,
			0XF5, 0, 0, 0, 0, 0, 0, 0,
			0XF6, 0, 0, 0, 0, 0, 0, 0 };
#endif // TABLA_TEST_4
			
#ifdef TABLA_TEST_5
#define TABLERO c_test_5
static CELDA c_test_5[7][8] = {
			0, 0XC1, 0XC2, 0XC3, 0XC4, 0XC5, 0Xc6, 0XC7,
			0XF1, 5, 0, 5, 5, 0, 5, 0,
			0XF2, 0, 0, 5, 5, 0, 0, 0,
			0XF3, 0, 0, 5, 0, 0, 0, 0,
			0XF4, 0, 0, 0, 0, 0, 0, 0,
			0XF5, 0, 0, 0, 0, 0, 0, 0,
			0XF6, 0, 0, 0, 0, 0, 0, 0 };
#endif // TABLA_TEST_5

#endif // TABLEROS_H
