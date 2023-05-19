/*
    Proyecto Hardware
    Práctica 1

    Fichero: tableros.h

    Autores:
        Dorian Boleslaw Wozniak   (817570@unizar.es)
        Pablo Latre Villacampa    (778043@unizar.es)

    Descripción: Implementación del tablero inicial (vacío)
*/

#ifndef TABLEROS_H
#define TABLEROS_H

#include "celda.h"

static __align(8) CELDA cuadricula[7][8] = {
    0, 0XC1, 0XC2, 0XC3, 0XC4, 0XC5, 0Xc6, 0XC7,
    0XF1, 0, 0, 0, 0, 0, 0, 0,
    0XF2, 0, 0, 0, 0, 0, 0, 0,
    0XF3, 0, 0, 0, 0, 0, 0, 0,
    0XF4, 0, 0, 0, 0, 0, 0, 0,
    0XF5, 0, 0, 0, 0, 0, 0, 0,
    0XF6, 0, 0, 0, 0, 0, 0, 0};

#endif // TABLEROS_H
