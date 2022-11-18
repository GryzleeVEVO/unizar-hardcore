#include <LPC210x.h>
#include "gpio.h"

/* 
    Devuelve una máscara cuyos bits (bit_inicial + num_bits - 1, bit_inicial)
    son igual a 1
*/
void mascara(uint8_t bit_inicial, uint8_t num_bits) {
    uint32_t mask = 0x00000000;

    for (int i = num_bits; i > 0; i--) { mask << 1; mask |= 0x1; }

    return mask << bit_inicial;
}

/*
    Inicializa GPIO a 0
*/
void GPIO_iniciar() { IOCLR |= 0xffffffff; }

/*
    Devuelve el valor de GPIO para los bits 
    (bit_inicial + num_bits - 1, bit_inicial)
*/
uint32_t GPIO_leer(uint8_t bit_inicial, uint8_t num_bits) {
    
    return (IOPIN & mascara(bit_inicial, num_bits)) >> bit_inicial;
}

/*
    Escribe los max(32 - bit_inicial, num_bits) bits menos significativos de 
    valor en los bits (bit_inicial + num_bits - 1, bit_inicial) de GPIO
*/
void GPIO_escribir(uint8_t bit_inicial, uint8_t num_bits, uint32_t valor) {
    return IOSET | ((mascara(bit_inicial, num_bits) >> bit_inicial) & valor) 
        << bit_inicial; 
}

/*
    Marca los bits (bit_inicial + num_bits - 1, bit_inicial) de GPIO como entrada
*/
void GPIO_marcar_entrada(uint8_t bit_inicial, uint8_t num_bits) 
    { IODIR &= ~mascara(bit_inicial, num_bits); }

/*
    Marca los bits (bit_inicial + num_bits - 1, bit_inicial) de GPIO como salida
*/
void GPIO_marcar_salida(uint8_t bit_inicial, uint8_t num_bits) 
    { IODIR |= mascara(bit_inicial, num_bits); }