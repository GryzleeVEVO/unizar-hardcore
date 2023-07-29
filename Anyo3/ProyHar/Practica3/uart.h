/*
    Proyecto Hardware
    Práctica 3

    Fichero:
        uart.h

    Autores:
        Dorian Boleslaw Wozniak (817570@unizar.es)
        Pablo Latre Villacampa (778043@unizar.es)

    Descripción:
        Interfaz de una serie de funciones para gestionar el periférico
        UART0.
*/

#ifndef UART_H
#define UART_H

#include <inttypes.h>

/*
    Inicializa el UART0
*/
void uart0_init(void);

/*
    Escribe un caracter inmediatamente en la UART0

    ch: caracter a escribir
*/
void uart0_echo(uint8_t ch);

/*
    Encola una cadena de caracteres al búfer asociado a la UART0. Si el búfer\
    estaba vacío al añadir, se escribe el primer caracter a transmitir

    ch: Cadena de caracteres. Debe terminar en NULL
*/
void uart0_enviar_array(char *ch);

/*
    Si el bufer no está vacío, transmite a UART0 el siguiente caracter
*/
void uart0_continuar_envio(void);

#endif // UART_H
