/*
    Proyecto Hardware
    Práctica 3

    Fichero:
        gpio.c

    Autores:
        Dorian Boleslaw Wozniak (817570@unizar.es)
        Pablo Latre Villacampa (778043@unizar.es)

    Descripción:
        Implementación de una serie de funciones para iniciar e interactuar
        con el General Purpouse Input/Output
*/

#include <LPC210x.h>

#include "gpio.h"

/*
    Devuelve una máscara para leer/escribir en GPIO.

    bit_inicial:    Primer bit desde donde empieza la máscara
    num_bits:       Longitud de la máscara

    La máscara puede quedar truncada.
*/
uint32_t GPIO_mascara(uint8_t bit_inicial, uint8_t num_bits)
{
    uint32_t mask = 0;

    for (uint8_t i = num_bits; i > 0; i--)
    {
        mask <<= 1;
        mask |= 0x1;
    }

    return mask << bit_inicial;
}

/*
    Inicializa todos los pines de GPIO
*/
void GPIO_iniciar() { IOPIN = 0x0; }

/*
    Lee una serie de pines de GPIO.

    bit_inicial:    Primer bit desde donde empieza la lectura
    num_bits:       Número de bits a leer
*/
uint32_t GPIO_leer(uint8_t bit_inicial, uint8_t num_bits)
{
    return (IOPIN & GPIO_mascara(bit_inicial, num_bits)) >> bit_inicial;
}

/*
    Escribe en una serie de pines de GPIO.

    bit_inicial:    Primer bit desde donde empieza la escritura
    num_bits:       Número de bits sobre los que escribir
    valor:          Valor a escribir

    No se escribirá sobre pines de entrada.
*/
void GPIO_escribir(uint8_t bit_inicial, uint8_t num_bits, uint32_t valor)
{
    IOCLR |= (GPIO_mascara(0, num_bits) & ~valor) << bit_inicial;
    IOSET |= (GPIO_mascara(0, num_bits) & valor) << bit_inicial;
}

/*
    Marca una serie de pines de GPIO como entrada.

    bit_inicial:    Primer bit desde donde empezar a marcar
    num_bits:       Número de bits marcados

    El usuario podrá cambiar el estado del pin, pero no el programa.
*/
void GPIO_marcar_entrada(uint8_t bit_inicial, uint8_t num_bits)
{
    IODIR &= ~GPIO_mascara(bit_inicial, num_bits);
}

/*
    Marca una serie de pines de GPIO como salida.

    bit_inicial:    Primer bit desde donde empezar a marcar
    num_bits:       Número de bits marcados

    El usuario no podrá cambiar el estado del pin, pero el programa si.
*/
void GPIO_marcar_salida(uint8_t bit_inicial, uint8_t num_bits)
{
    IODIR |= GPIO_mascara(bit_inicial, num_bits);
}
