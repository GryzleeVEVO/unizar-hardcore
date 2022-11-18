/*
    Proyecto Hardware
    Práctica 2

    Fichero:
        gpio.c

    Autores:
        Dorian Boleslaw Wozniak (817570@unizar.es)
        Pablo Latre Villacampa (778043@unizar.es)

    Descripción: 
        Interfaz de métodos para inicializar y utilizar los pines de GPIO
*/

#ifndef GPIO_H
#define GPIO_H

#include <inttypes.h>

/*
    Pre: ---
    Post: GPIO = 0
*/
void GPIO_iniciar(void);

/*
    Pre: ---
    Post: Devuelve GPIO (bit_inicial + num_bits - 1, bit_inicial)
*/
uint32_t GPIO_leer(uint8_t bit_inicial, uint8_t num_bits);

/*
    Pre: ---
    Post: Devuelve GPIO (bit_inicial + num_bits - 1, bit_inicial)
*/
void GPIO_escribir(uint8_t bit_inicial, uint8_t num_bits, uint32_t valor);

/*
    Pre:
    Post: GPIO (bit_inicial + num_bits - 1, bit_inicial) = entrada
*/
void GPIO_marcar_entrada(uint8_t bit_inicial, uint8_t num_bits);

/*
    Pre:
    Post: GPIO (bit_inicial + num_bits - 1, bit_inicial) = salida
*/
void GPIO_marcar_salida(uint8_t bit_inicial, uint8_t num_bits);

#endif // GPIO_H
