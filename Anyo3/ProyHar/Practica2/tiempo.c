/*
    Proyecto Hardware
    Práctica 2

    Fichero:
        tiempo.c

    Autores:
        Dorian Boleslaw Wozniak (817570@unizar.es)
        Pablo Latre Villacampa (778043@unizar.es)

    Descripción: 
        Implementación de métodos para inicializar y utilizar los timers timer0
        (encola un mensaje periodicamente) y timer1 (contador de tiempo en us)
*/

#include <LPC210X.h>

#include "tiempo.h"
#include "cola_eventos.h"

// Cuenta de interrupciones
static volatile uint32_t timer0_count = 0;
static volatile uint32_t timer1_count = 0;

// Rutinas de servicio

// timer0 cuando haya vencido periodo ms
void timer0_RSI (void) __irq {
    // Trata la interrupción
    timer0_count++;
    cola_encolar_eventos(T0_PERIODO, timer0_count, 0);
    // Baja la interrupción
    T0IR = 1;
    VICVectAddr = 0;
}

// timer1 cuando haya desbordado el TC
void timer1_RSI (void) __irq {
    // Trata la interrupción
    timer1_count++;
    // Evento encolado para efectos de depuración
    cola_encolar_eventos(T1_OVERFLOW, timer0_count, 0);
    // Baja la interrupción
    T0IR = 1;
    VICVectAddr = 0;
}

/*
    Pre: ---
    Post: timer1 mide tiempo con precisión de us, interrumpiendo cuando el 
            contador desborda. Queda asignado a VIC[1]
*/
void temporizador_iniciar() {
    // Ajusta la cuenta para contar en us e interrumpir cuando desborde
    T1PR = 15 - 1;
    T1MR0 = 0xffffffff - 1;
    T1MCR |= 0x3;

    // Configura VIC[1] para tratar timer1
    VICVectAddr1 = (uint32_t) timer1_RSI;
    VICVectCntl1 |= 0x20 | 5;
}

/*
    Pre: temporizador_iniciar()
    Post: Habilita interrupciones para timer1
*/
void temporizador_empezar() {
    // Reinicia la cuenta de interrupciones
    timer1_count = 0;

    // Reinicia y arranca TC
    T1TCR |= 0x3;
    T1TCR &= ~0x02;

    // Habilita interrupciones para timer1 (bit 5)
    VICIntEnable |= (0x1 << 5);
}

/*
    Pre:  temporizador_empezar()
    Post: Devuelve el valor del contador de timer1
*/
uint32_t temporizador_leer() {
    // Lee tiempo actual
    uint32_t time = T1TC;
    
    // Añade las veces que se ha desbordado (cada ~71 min)
    // NOTA: Devuelve actualmente uint32_t, time quedará desbordado igualmente
    for (int i = timer1_count; i > 0; i--) time += 0xffffffff;

    return time;
}

/*
    Pre:  temporizador_empezar()
    Post: Devuelve el valor del contador de timer1 y deshabilta interrupciones 
            de timer1
*/
uint32_t temporizador_parar() {
    
    // Detiene timer
    T1TCR &= ~0x1;
    VICIntEnClr |= (0x1 << 5);

    return temporizador_leer();
}

/*
    Pre: ---
    Post: timer0 mide tiempo con precisión de ms, interrumpiendo cuando el 
            contador alcanza periodo ms. Queda asignado a VIC[0] y se habilitan
            interrupciones
*/
void temporizador_reloj(uint32_t periodo) {
    timer0_count = 0;

    // Ajusta la cuenta para contar ms e interrumpir cuando alcance periodo
    T0PR = 15000 - 1;
    T0MR0 = periodo - 1;
    T0MCR |= 0x3;

    // Configura VIC[1] para tratar timer1
    VICVectAddr0 = (uint32_t) timer0_RSI;
    VICVectCntl0 |= 0x20 | 4;

    // Reinicia TC y empieza a funcionar (timer0 = bit 4)
    T0TCR |= 0x3;
    T0TCR &= ~0x02;
    VICIntEnable |= (0x1 << 4);
}
