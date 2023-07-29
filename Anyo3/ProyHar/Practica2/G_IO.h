/*
    Proyecto Hardware
    Práctica 2

    Fichero:
        G_IO.h

    Autores:
        Dorian Boleslaw Wozniak (817570@unizar.es)
        Pablo Latre Villacampa (778043@unizar.es)

    Descripción: 
        Declaraciones de funciones de un gestor de entrada/salida, que ofrece 
        funciones de alto nivel para manejar los puertos de GPIO
*/


#ifndef G_IO_H
#define G_IO_H

#include <inttypes.h>

/*
    Pre: ---
    Post: Inicializa el gestor de entrada-salida, incluyendo el GPIO
*/
void IO_iniciar(void);

/*
    Pre: ---
    Post: Altera el estado de GPIO para una nueva partida
*/
void IO_nueva_partida(void);

/*
    Pre: ---
    Post: Si la entrada ha cambiado desde la última entrada, reinicia el
            alarma para dormirse. Devuelve la columna introducida o error

*/
uint8_t IO_leer_entrada(void);

// Alterna el estado del latido
void IO_cambiar_latido(void);

// Apaga el indicador de latido
void IO_bajar_latido(void);

// Apaga el indicador de jugada inválida
void IO_indicar_jugada_valida(void);

// Enciende el indicador de jugada inválida
void IO_indicar_jugada_invalida(void);

// Alterna jugador y marca jugada realizada
void IO_jugada_realizada(void);

// Apaga el indicador de jugada realizada
void IO_bajar_jugada_realizada(void);

// Indica estado de victoria
void IO_victoria(void);

// Indica estado de empate
void IO_empate(void);

// Enciende el indicador de desborde
void IO_marcar_overflow(void);

#endif // G_IO_H
