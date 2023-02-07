/*
    Proyecto Hardware
    Práctica 2

    Fichero:
        tiempo.h

    Autores:
        Dorian Boleslaw Wozniak (817570@unizar.es)
        Pablo Latre Villacampa (778043@unizar.es)

    Descripción: 
        Interfaz de métodos para inicializar y utilizar los timers timer0
        (encola un mensaje periodicamente) y timer1 (contador de tiempo en us)
*/

#ifndef TIEMPO_H
#define TIEMPO_H

#include <inttypes.h>

/*
    Inicialilza y prepara el contador
*/
void temporizador_iniciar(void);

/*
    Inicia el contador
*/
void temporizador_empezar(void);

/*
    Devuelve el tiempo desde que el contador empezó
*/
uint32_t temporizador_leer(void);

/*
    Detiene el contador y devuelve el tiempo desde que el contador empezó
*/
uint32_t temporizador_parar(void);

/*
    Inicializa y arranca un temporizador que cada periodo ms encolará un evento
*/
void temporizador_reloj(uint32_t periodo);

#endif // TIEMPO_H
