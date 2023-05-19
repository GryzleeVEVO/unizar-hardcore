/*
    Proyecto Hardware
    Práctica 2

    Fichero: conecta4_2022.h

    Autores:
        Dorian Boleslaw Wozniak   (817570@unizar.es)
        Pablo Latre Villacampa    (778043@unizar.es)

    Descripción: Implementación de la lógica de un juego Conecta4. El juego
        permite recibir movimientos, los cuales trata internamente.

        El propio juego es para dos jugadores. Estos por turnos apilan fichas
        en un tablero, y cuando uno de los dos jugadores crea una línea de
        cuatro o más fichas, gana. El juego es un empate si se llena el tablero
        y no hay ganador
*/

#ifndef CONECTA4_H_2022
#define CONECTA4_H_2022

#include <inttypes.h>

/*
    Inicializa el tablero para una nueva partida
*/
void conecta4_iniciar(void);

/*
    Dada una columna, determina una jugada y, si se puede insertar, almacena la
    próxima jugada y muestra la posición de la ficha en el tablero.

    c: Valor de la columna donde insertar una ficha
*/
void conecta4_comprobar_entrada(uint32_t c);

/*
    Si hay una jugada pendiente de realizar, la compromete al tablero y actúa
    según si hay línea, empate o debe cambiar de jugador
*/
void conecta4_realizar_jugada(void);

/*
    Si hay una jugada pendiente por realizar, la cancela
*/
void conecta4_cancelar_jugada(void);

/*
    Informa del fin del juego, sea voluntario o realizado por el juego

    razon: Motivo por la que se acaba el juego
*/
void conecta4_detener(uint32_t razon);

#endif // CONECTA4_H_2022
