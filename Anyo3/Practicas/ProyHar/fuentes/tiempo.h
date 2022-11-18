/*
    Proyecto Hardware
    Práctica 2

    Fichero:
        tiempo.h

    Autores:
        Dorian Boleslaw Wozniak (817570@unizar.es)
        Pablo Latre Villacampa (778043@unizar.es)

    Descripción: 
        Interfaz para interactuar con los temporizadores timer0 y timer1.
        Ofrece funcionalidad para contar el tiempo pasado y para encolar
        un evento periodicamente
*/

#ifndef TIEMPO_H
#define TIEMPO_H

#include <inttypes.h>

/*
    Inicialilza y prepara el contador
*/
void temporizador_iniciar();

/*
    Inicia el contador
*/
void temporizador_empezar();

/*
    Devuelve el tiempo desde que el contador empezó
*/
uint32_t temporizador_leer();

/*
    Detiene el contador y devuelve el tiempo desde que el contador empezó
*/
uint32_t temporizador_parar();

/*
    Inicializa y arranca un temporizador que cada periodo ms encolará un evento
*/
void temporizador_reloj(uint32_t periodo);

#endif // TIEMPO_H