/*
    Proyecto Hardware
    Práctica 3

    Fichero:
        tiempo.h

    Autores:
        Dorian Boleslaw Wozniak (817570@unizar.es)
        Pablo Latre Villacampa (778043@unizar.es)

    Descripción:
        Interfaz de una serie de funciones para iniciar e interactuar con
        una serie de periféricos relacionados con el tiempo:

        - Timer0:   Interrumpe periodicamente dado un tiempo en milisegundos
        - Timer1:   Cuenta el tiempo con precisión de microsegundos
        - RTC:      Contador en tiempo real, devuelve tiempo en segundos
        - WD:       Reinicia el programa tras un tiempo en segundos dado.
                    Alimentarlo reinicia el contador.
*/

#ifndef TIEMPO_H
#define TIEMPO_H

#include <inttypes.h>

/******************************************************************************/
/* TIMER 0 */

/*
    Inicia el periférico Timer0 con el periodo dado.

    periodo:    Tiempo en milisegundos tras el cual se realiza una interrupción

    Se configura como FIQ, el handler se define en el fichero Startup.s.
*/
void temporizador_reloj(uint32_t periodo);

/******************************************************************************/
/* TIMER 1 */

/*
    Inicia el periférico Timer1. El timer queda asociado a VIC[2].
*/
void temporizador_iniciar(void);

/*
    Inicia a contar el tiempo mediante timer1.
*/
void temporizador_empezar(void);

/*
    Devuelve el tiempo transcurrido en microsegundos
*/
uint32_t __swi(0) clock_getus(void);

/*
    Detiene el contador Timer1 junto al insante tiempo al detenerse
*/
uint32_t temporizador_parar(void);

/******************************************************************************/
/* RTC */

/*
    Inicia el contador RTC
*/
void RTC_init(void);

/*
    Devuelve el tiempo transcurrido en segundos
*/
uint32_t __swi(1) clock_gettime(void);

/******************************************************************************/
/* WD */

/*
    Inicia el contador Watchdog con el tiempo dado para reiniciar.

    sec:    Número de segundos a transcurrir antes de reiniciar el programa
*/
void WD_init(uint32_t seg);

/*
    Alimenta al WD para reiniciar la cuenta atrás para reiniciar el programa.

    La secuencia de alimentación se debe realizar en exclusión mútua
*/
void WD_feed(void);

#endif // TIEMPO_H
