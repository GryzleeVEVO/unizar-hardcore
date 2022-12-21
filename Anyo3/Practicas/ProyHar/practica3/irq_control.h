#ifndef IRQ_CONTROL_H
#define IRQ_CONTROL_H

#include <inttypes.h>

// Definidas en SWI.s

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
