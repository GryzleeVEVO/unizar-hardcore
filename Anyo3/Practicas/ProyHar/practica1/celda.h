/*
    Proyecto Hardware
    Práctica 1

    Fichero: celda.h

    Autores:
        Dorian Boleslaw Wozniak   (817570@unizar.es)
        Pablo Latre Villacampa    (778543@unizar.es)

    Descripción: Definición de un TAD que representa una celda de un tablero
                    de conecta 4. Una celda puede ser vacía o contener una ficha
                    blanca o una ficha negra
*/

#ifndef CELDA_H
#define CELDA_H

#include <inttypes.h>

/* 
    Cada celda se codifica en 8 bits. Sólo se usan los 3 menos significativos
    - bit 0: 1: ficha blanca; 0 : no hay ficha blanca
    - bit 1: 1: ficha negra; 0 : no hay ficha negra
    - bit 2: 0: posición vacía; 1: ocupada

    En memoria, eso significa que:
    - Las celdas vacías tienen valor 0
    - Las celdas con ficha blanca tienen valor 5
    - Las celdas con ficha negra tienen valor 6
*/

typedef uint8_t CELDA;

/* --- FUNCIONES PARA MODIFICAR VALOR DE CELDA --- */

/*
    Vuelve la celda dada en @celdaptr en una celda vacía (= 0)

    Sólo está definida si al compilar se define que se deshaga 
    una jugada ganadora y continuar el juego
*/
__inline static void
celda_borrar_valor(CELDA *celdaptr) { *celdaptr = 0x00; }

/*
    Vuelve la celda en @celdaptr en una celda ocupada (= 4) que contiene una
    ficha blanca (val = 1) o negra (val = 2)
*/
__inline static void
celda_poner_valor(CELDA *celdaptr, uint8_t val) 
  { if ((val == 1) || (val == 2 )) *celdaptr = 0x04 + val; }

// Devuelve 1 solo si la celda es vacía (bit 2 = 0)
__inline static uint8_t
celda_vacia(CELDA celda) { return (celda & 0x4) == 0; }

// Devuelve el color de la celda (0 si vacía, 1 si blanca, 2 si negra)
__inline static uint8_t
celda_color(CELDA celda) { return (celda & 0x03); }

// Devuelve distinto de 0 si la celda contiene una ficha blanca
__inline static uint8_t
celda_blanca(CELDA celda) { return celda & 0x05; }

// Devuelve distinto de 0 si la celda contiene una ficha negra
__inline static uint8_t
celda_negra(CELDA celda) { return celda & 0x06; }

#endif // CELDA_H
