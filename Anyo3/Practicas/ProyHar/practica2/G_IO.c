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
    ERROR = 0xFF
};

/*
    Pre: ---
    Post: Inicializa el gestor de entrada-salida, incluyendo el GPIO
*/
void IO_iniciar() {
    GPIO_iniciar();             // Limpia valores de GPIO
    GPIO_marcar_entrada(1, 2);  // Pins 3-9 -> Entrada de columna
    GPIO_marcar_salida(0, 3);   // Pin 1-2 -> Jugadores. 
    // Pin 14-15 -> No disponibles (sustituidos por EINT)
    GPIO_marcar_salida(16, 3); // Pin 16-18 -> Estado partida/jugada
    GPIO_marcar_salida(30, 2); // Pin 30/31 -> Overflow / Latido idle
}


/*
    Pre: ---
    Post: Altera el estado de GPIO para una nueva partida
*/
void IO_nueva_partida() {
    GPIO_escribir(1, 2, 0x1);   // Indica que inicia jugador 1
    GPIO_escribir(16, 3, 0x0);  // Limpia estado partida
    GPIO_escribir(31, 1, 0x0);  // Limpia latido
}

/*
    Pre: ---
    Post: Si la entrada ha cambiado desde la última entrada, reinicia el
            alarma para dormirse. Devuelve la columna introducida o error

*/
uint8_t IO_leer_entrada() {
    
    static uint8_t ultima_entrada = 0;  // Valor de última lectura
    
    uint8_t entrada = GPIO_leer(3, 7); // Lee la entrada
    
    // Si la entrada ha cambiado, reinicia la alarma para dormir
    if (entrada != ultima_entrada) {
        cola_msg(SET_ALARMA, ALARM_GO_SLEEP_MSG);
        cola_msg(SET_ALARMA, ALARM_PARPADEAR_MSG);
        GPIO_escribir(31, 1, 0);
        ultima_entrada = entrada;
    }
    
    // Lee la columna marcada.
    uint8_t i = 1, ret = 0;
    
    while (i <= 7) {
        if ((entrada & 0x1) == 0) {
            if (ret == 0) 
                { ret = i; } // Indica la columna marcada
            else 
                { return ERROR; }    // Hay dos columnas marcadas simultaneamente
        }
        i++; 
        entrada >>= 1;
    }
    
    return ret;
}

// Alterna el estado del latido
void IO_cambiar_latido() {
    if (GPIO_leer(31, 1))
        GPIO_escribir(31, 1, 0);
    else GPIO_escribir(31, 1, 1);
}

// Apaga el indicador de latido
void IO_bajar_latido() {
    GPIO_escribir(31, 1, 0);
}

// Apaga el indicador de jugada inválida
void IO_indicar_jugada_valida() {
    GPIO_escribir(17, 1, 0);
}

// Enciende el indicador de jugada inválida
void IO_indicar_jugada_invalida() {
    GPIO_escribir(17, 1, 1);
}

// Alterna jugador y marca jugada realizada
void IO_jugada_realizada() {
    if (GPIO_leer(1, 2) == 0x1) GPIO_escribir(1, 2, 2);
    else GPIO_escribir(1, 2, 1);
    
    GPIO_escribir(16, 1, 1);
    GPIO_escribir(31, 1, 0);
}

// Apaga el indicador de jugada realizada
void IO_bajar_jugada_realizada() {
    GPIO_escribir(16, 1, 0);
}

// Indica estado de victoria
void IO_victoria() {
    GPIO_escribir(16, 1, 1);
    GPIO_escribir(18, 1, 1);
    GPIO_escribir(31, 1, 0);
}

// Indica estado de empate
void IO_empate() {
    GPIO_escribir(1, 2, 3);
    GPIO_escribir(16, 1, 1);
    GPIO_escribir(18, 1, 1);
    GPIO_escribir(31, 1, 0);
}

// Enciende el indicador de desborde
void IO_marcar_overflow() {
    GPIO_escribir(30, 1, 1);
}
