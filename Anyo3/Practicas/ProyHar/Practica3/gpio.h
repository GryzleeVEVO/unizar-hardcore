/*
    Proyecto Hardware
    Práctica 3

    Fichero:
        gpio.h
        
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
    Inicializa todos los pines de GPIO
*/
void GPIO_iniciar(void);

/*
    Lee una serie de pines de GPIO.
    
    bit_inicial:    Primer bit desde donde empieza la lectura
    num_bits:       Número de bits a leer
*/
uint32_t GPIO_leer(uint8_t bit_inicial, uint8_t num_bits);

/*
    Escribe en una serie de pines de GPIO.
    
    bit_inicial:    Primer bit desde donde empieza la escritura
    num_bits:       Número de bits sobre los que escribir
    valor:          Valor a escribir
    
    No se escribirá sobre pines de entrada.
*/
void GPIO_escribir(uint8_t bit_inicial, uint8_t num_bits, uint32_t valor);

/*
    Marca una serie de pines de GPIO como entrada.
    
    bit_inicial:    Primer bit desde donde empezar a marcar
    num_bits:       Número de bits marcados
    
    El usuario podrá cambiar el estado del pin, pero no el programa.
*/
void GPIO_marcar_entrada(uint8_t bit_inicial, uint8_t num_bits);

/*
    Marca una serie de pines de GPIO como salida.
    
    bit_inicial:    Primer bit desde donde empezar a marcar
    num_bits:       Número de bits marcados
    
    El usuario no podrá cambiar el estado del pin, pero el programa si.
*/
void GPIO_marcar_salida(uint8_t bit_inicial, uint8_t num_bits);

#endif // GPIO_H
