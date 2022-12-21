/*
    Proyecto Hardware
    Práctica 2

    Fichero: conecta4_2022.h

    Autores:
        Dorian Boleslaw Wozniak   (817570@unizar.es)
        Pablo Latre Villacampa    (778043@unizar.es)

    Descripción: Declaraciónes para un juego de conecta 4 adaptado para la 
        práctica 2.
*/

#ifndef CONECTA4_H_2022
#define CONECTA4_H_2022

#include <inttypes.h>

/*
    Pre: ---
    Post: Inicializa el tablero de juego y prepara el sistema para el juego
*/
void conecta4_iniciar(void);

void conecta4_mostrar_tablero(uint8_t mostrar_marcada);


/*
    Pre: 1 <= columna <= NUM_COLUMNAS
    Post: Devuelve la fila en la que se puede introducir una ficha, o ERROR
            si la fila o columna dada se encuentran fuera del rango
*/
void conecta4_comprobar_entrada(uint32_t c);
    
/*
    Pre: 1 <= fila <= NUM_FILAS, 1 <= columna <= NUM_COLUMNAS
    Post: Realiza la jugada dados la fila y columna donde insertar una ficha.
            Devuelve el estado del juego tras la introducción 
            (pasar turno / victoria blancas / victoria negras / empate)
*/
void conecta4_realizar_jugada(void);

void conecta4_cancelar_jugada(void);

void conecta4_detener(uint32_t razon);

#endif // CONECTA4_H_2022

