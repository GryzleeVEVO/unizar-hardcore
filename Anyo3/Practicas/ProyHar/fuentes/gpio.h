#ifndef GPIO_H
#define GPIO_H

#include <inttypes.h>

/*
    Inicializa GPIO a 0
*/
void GPIO_iniciar();

/*
    Devuelve el valor de GPIO para los bits 
    (bit_inicial + num_bits - 1, bit_inicial)
*/
uint32_t GPIO_leer(uint8_t bit_inicial, uint8_t num_bits);

/*
    Escribe los max(32 - bit_inicial, num_bits) bits menos significativos de 
    valor en los bits (bit_inicial + num_bits - 1, bit_inicial) de GPIO
*/
void GPIO_escribir(uint8_t bit_inicial, uint8_t num_bits, uint32_t valor);

/*
    Marca los bits (bit_inicial + num_bits - 1, bit_inicial) de GPIO como entrada
*/
void GPIO_marcar_entrada(uint8_t bit_inicial, uint8_t num_bits);

/*
    Marca los bits (bit_inicial + num_bits - 1, bit_inicial) de GPIO como salida
*/
void GPIO_marcar_salida(uint8_t bit_inicial, uint8_t num_bits);

#endif // GPIO_H