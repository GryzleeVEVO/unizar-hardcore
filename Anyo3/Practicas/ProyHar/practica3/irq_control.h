/*
    Proyecto Hardware
    Práctica 3

    Fichero:
        power.h

    Autores:
        Dorian Boleslaw Wozniak (817570@unizar.es)
        Pablo Latre Villacampa (778043@unizar.es)

    Descripción: 
        Interfaz de una serie de llamadas al sistema para leer y gestionar
        el estado de las inhibiciones de interrupciones.
        
        Todas las funciones están implementadas en SWI.s
*/

#ifndef IRQ_CONTROL_H
#define IRQ_CONTROL_H

#include <inttypes.h>

// Habilita interrupciones rápidas
void __swi(0xff) enable_fiq(void);

// Habilita interrupciones rápidas
void __swi(0xfe) disable_fiq(void);

// Habilita interrupciones estandar y rápidas
void __swi(0xfd) enable_irq_fiq(void);

// Habilita interrupciones estandar y rápidas
void __swi(0xfc) disable_irq_fiq(void);

// Devuelve el valor del bit IRQ
uint8_t __swi(0xfb) read_IRQ_bit(void);

// Devuelve el valor del bit FIQ
uint8_t __swi(0xfa) read_FIQ_bit(void);

#endif // IRQ_CONTROL_H
