/*
    Proyecto Hardware
    Práctica 2

    Fichero:
        G_IO.c

    Autores:
        Dorian Boleslaw Wozniak (817570@unizar.es)
        Pablo Latre Villacampa (778043@unizar.es)

    Descripción: 
        Implementación de un gestor de entrada/salida, que ofrece funciones
        de alto nivel para manejar los puertos de GPIO
*/

#include "G_IO.h"
#include "gpio.h"
#include "cola_msg.h"
#include "G_Alarm.h"

enum {
    PIN_JUGADOR_1   = 2,
    PIN_JUGADOR_2   = 3,
    PIN_COL1        = 3,
    PIN_REALIZADA   = 16,
    PIN_VALIDA      = 17,
    PIN_VICTORIA    = 18,
    PIN_OVERFLOW    = 30,
    PIN_LATIDO      = 31,
};

/*
    Pre: ---
    Post: Inicializa el gestor de entrada-salida, incluyendo el GPIO
*/
void IO_iniciar() {
    GPIO_iniciar();             
    GPIO_marcar_salida(PIN_JUGADOR_1, 2);
    GPIO_marcar_salida(PIN_REALIZADA, 3);
    GPIO_marcar_salida(30, 2);
}


/*
    Pre: ---
    Post: Altera el estado de GPIO para una nueva partida
*/
void IO_nueva_partida() {
    GPIO_escribir(PIN_JUGADOR_1, 2, 0x1);  // Indica que inicia jugador 1
    GPIO_escribir(PIN_REALIZADA, 3, 0x0);  // Limpia estado partida
}

/*
    Pre: ---
    Post: Si la entrada ha cambiado desde la última entrada, reinicia el
            alarma para dormirse. Devuelve la columna introducida o error
*/
uint8_t IO_leer_entrada() {
    return GPIO_leer(PIN_COL1, 7);
}

// Alterna el estado del latido
void IO_cambiar_latido() {
    if (GPIO_leer(PIN_LATIDO, 1))
        GPIO_escribir(PIN_LATIDO, 1, 0);
    else GPIO_escribir(PIN_LATIDO, 1, 1);
}

// Apaga el indicador de latido
void IO_bajar_latido() {
    GPIO_escribir(PIN_LATIDO, 1, 0);
}

// Apaga el indicador de jugada inválida
void IO_indicar_jugada_valida() {
    GPIO_escribir(PIN_VALIDA, 1, 0);
}

// Enciende el indicador de jugada inválida
void IO_indicar_jugada_invalida() {
    GPIO_escribir(PIN_VALIDA, 1, 1);
}

// Alterna jugador y marca jugada realizada
void IO_jugada_realizada() {
    if (GPIO_leer(PIN_JUGADOR_1, 2) == 0x1) GPIO_escribir(PIN_JUGADOR_1, 2, 2);
    else GPIO_escribir(PIN_JUGADOR_1, 2, 1);
    
    GPIO_escribir(PIN_REALIZADA, 1, 1);
}

// Apaga el indicador de jugada realizada
void IO_bajar_jugada_realizada() {
    GPIO_escribir(PIN_REALIZADA, 1, 0);
}

// Indica estado de victoria
void IO_victoria() {
    GPIO_escribir(PIN_REALIZADA, 1, 1);
    GPIO_escribir(PIN_VALIDA, 1, 1);
}

// Indica estado de empate
void IO_empate() {
    GPIO_escribir(PIN_JUGADOR_1, 2, 3);
    GPIO_escribir(PIN_REALIZADA, 1, 1);
    GPIO_escribir(PIN_VICTORIA, 1, 1);
}

// Enciende el indicador de desborde
void IO_marcar_overflow() {
    GPIO_escribir(PIN_OVERFLOW, 1, 1);
}
