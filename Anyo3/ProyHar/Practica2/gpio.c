/*
    Proyecto Hardware
    Práctica 2

    Fichero:
        gpio.c

    Autores:
        Dorian Boleslaw Wozniak (817570@unizar.es)
        Pablo Latre Villacampa (778043@unizar.es)

    Descripción: 
        Implementación de métodos para inicializar y utilizar los pines de GPIO
*/

#include <LPC210x.h>

#include "gpio.h"

/* 
    Pre: ---
    Post: Devuelve (bit_inicial + num_bits - 1, bit_inicial) = 1
*/
uint32_t GPIO_mascara(uint8_t bit_inicial, uint8_t num_bits) {
    uint32_t mask = 0;

    for (uint8_t i = num_bits; i > 0; i--) { mask <<= 1; mask |= 0x1; }

    return mask << bit_inicial;
}

/*
    Pre: ---
    Post: GPIO = 0
*/
void GPIO_iniciar() { IOPIN = 0x0; }

/*
    Pre: ---
    Post: Devuelve GPIO (bit_inicial + num_bits - 1, bit_inicial)
*/
uint32_t GPIO_leer(uint8_t bit_inicial, uint8_t num_bits) 
    { return (IOPIN & GPIO_mascara(bit_inicial, num_bits)) >> bit_inicial; }

/*
    Pre:  ---
    Post: GPIO (bit_inicial + num_bits - 1, bit_inicial) = 1
*/
void GPIO_escribir(uint8_t bit_inicial, uint8_t num_bits, uint32_t valor) { 
    
    IOCLR |= (GPIO_mascara(0, num_bits) & ~valor) << bit_inicial; 
    IOSET |= (GPIO_mascara(0, num_bits) & valor) << bit_inicial; 
}

/*
    Pre: ---
    Post: GPIO (bit_inicial + num_bits - 1, bit_inicial) = entrada
*/
void GPIO_marcar_entrada(uint8_t bit_inicial, uint8_t num_bits) 
    { IODIR &= ~GPIO_mascara(bit_inicial, num_bits); }

/*
    Pre: ---
    Post: GPIO (bit_inicial + num_bits - 1, bit_inicial) = salida
*/
void GPIO_marcar_salida(uint8_t bit_inicial, uint8_t num_bits) 
    { IODIR |= GPIO_mascara(bit_inicial, num_bits); }







