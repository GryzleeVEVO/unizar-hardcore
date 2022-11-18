/*
    Proyecto Hardware
    Práctica 2

    Fichero:
        tiempo.c

    Autores:
        Dorian Boleslaw Wozniak (817570@unizar.es)
        Pablo Latre Villacampa (778043@unizar.es)

    Descripción: 
        Implementación de operaciones para gestionar los perifericos timer0 y 
        timer1. timer1 actúa como contador de tiempo, mientras que timer0
        interrrumpira de forma periódica para encolar un evento
*/

#include <LPC210X.h>
#include "tiempo.h"
#include "cola_eventos.h"

// Contadores de interrupciones

static volatile timer0_counter = 0;
static volatile timer1_counter = 0;

// Rutinas de servicio

// timer0 cuando haya vencido periodo ms
void timer0_RSI (void) __irq {
    // Trata la interrupción
    timer0_counter++;
    cola_encolar_eventos(PERIODO, timer0_counter, 0);
    
    // Baja la interrupción
    T0IR = 1;
    VICVectAddr = 0;
}

// timer1 cuando haya desbordado el TC
void timer0_RSI (void) __irq {
    // Trata la interrupción
    timer1_counter++;
    cola_encolar_eventos(CONTADOR, timer0_counter, 0);
    
    // Baja la interrupción
    T0IR = 1;
    VICVectAddr = 0;
}

/*
    Inicialilza y prepara el contador
*/
void temporizador_iniciar() {

    // Ajusta la cuenta para contar us e interrumpir cuando desborde
    T1PR = 15 - 1;
    T1MR0 = 0xffffffff - 1;
    T1MCR = 0x2 | 0x1;

    // Configura VIC[1] para tratar timer1
    VICVectAddr1 = (uint32_t) timer1_RSI;
    VICVectCntl1 = 0x20 | 0x5;
}

/*
    Inicia el contador
*/
void temporizador_empezar() {
    // Reinicia la cuenta de interrupciones
    timer1_counter = 0;

    // Reinicia y arranca TC
    T1TCR = 0x3; 

    // Habilita interrupciones VIC[1]
    VICIntEnable |= 0x10;
}

/*
    Devuelve el tiempo desde que el contador empezó
*/
uint32_t temporizador_leer() {
    // Lee tiempo actual
    uint32_t time = T1TC;
    
    // Añade las veces que se ha desbordado (cada ~71 min)
    // NOTA: Se sustituirá por un entero de 8 bytes en el futuro
    for (int i = timer1_counter; i > 0; i--) time += 0xffffffff;

    return time;
}

/*
    Detiene el contador y devuelve el tiempo desde que el contador empezó
*/
uint32_t temporizador_parar() {
    
    // Detiene timer
    T1TCR = 0;
    VICIntEnable &= ~0x10

    return temporizador_leer();
}

/*
    Inicializa y arranca un temporizador que cada periodo ms encolará un evento
*/
void temporizador_reloj(uint32_t periodo) {
    timer0_counter = 0;

    // Ajusta la cuenta para contar ms e interrumpir cuando alcance periodo
    T0PR = 15000;
    T0MR0 = periodo;
    T0MCR = 0x2 | 0x1;

    // Configura VIC[1] para tratar timer1
    VICVectAddr0 = (uint32_t) timer0_RSI;
    VICVectCntl0 = 0x20 | 0x8;

    // Reinicia TC y empieza a funcionar
    T0TCR = 0x3;
    VICIntEnable |= 0x8;
}
