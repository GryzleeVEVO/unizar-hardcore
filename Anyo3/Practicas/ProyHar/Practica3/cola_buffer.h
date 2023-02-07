/*
    Proyecto Hardware
    Práctica 3

    Fichero:
        cola_buffer.c

    Autores:
        Dorian Boleslaw Wozniak (817570@unizar.es)
        Pablo Latre Villacampa (778043@unizar.es)

    Descripción: 
        Interfaz de un TAD Cola circular que actúa de búfer de 
        caracteres. Permite añadir cadenas de caracteres al búfer, comprobar
        si está vacío, y obtener el siguiente carácter a desencolar.
*/


#ifndef COLA_BUFFER_H
#define COLA_BUFFER_H

#include <inttypes.h>

/*
    Añade una cadena de caracteres a la cola

    c: Cadena de caracteres a introducir

    Si se desborda, para el programa. Las cadenas deben estar terminadas en NULL
*/
void buffer_anyadir(char* c);

/*
    Devuelve si el búfer está vacío
*/
uint8_t buffer_vacio(void);

/*
    Devuelve el siguiente caracter almacenado en el búfer

    El búfer no debe estar vacío al extraer
*/
int8_t buffer_siguiente(void);

#endif // COLA_BUFFER_H
