/*
    Proyecto Hardware
    Práctica 3

    Fichero:
        tiempo.c

    Autores:
        Dorian Boleslaw Wozniak (817570@unizar.es)
        Pablo Latre Villacampa (778043@unizar.es)

    Descripción: 
        Implementación de una serie de funciones para iniciar e interactuar con
        una serie de periféricos relacionados con el tiempo:
        
        - Timer0:   Interrumpe periodicamente dado un tiempo en milisegundos
        - Timer1:   Cuenta el tiempo con precisión de microsegundos
        - RTC:      Contador en tiempo real, devuelve tiempo en segundos
        - WD:       Reinicia el programa tras un tiempo en segundos dado.
                    Alimentarlo reinicia el contador.
*/

#include <LPC210X.h>

#include "tiempo.h"
#include "cola_eventos.h"
#include "irq_control.h"

// Constantes para registros de los timers
enum {
    TIME_SEC    = 15000000,     // 60 MHz -> 1s     = 1 / (1/PCLK * 4)
    TIME_MS     = 15000 - 1,    // 60 Mhz -> 1ms    = (10 ^-3) / (1/PCLK * 4)
    TIME_US     = 15 - 1,       // 60 Mhz -> 1us    = (10 ^-6) / (1/PCLK * 4)
    
    // Timer0/1
    MR0_INT     = 0x1,
    MR0_RESET   = 0x2,
   
    TCR_ENABLE  = 0x1,
    TCR_RESET   = 0x2,
   
    // RTC 
    PREINT_60MHZ    = 1830, // 60 MHz -> (PCLK / 32768) - 1,
    PREFRAC_60MHZ   = 1792, // 60 MHz -> (PCLK) - (32768 * (PREINT + 1)),
    
    CCR_ENABLE  = 0x1,
    CCR_RESET   = 0x2,
    
    // WD
    WD_ENABLE   = 0x1,
    WD_RESET    = 0x2
};

// Constantes para interrupciones vectorizadas
enum {
    // Timer0
    POS_TIMER0 = 4, 
    INT_TIMER0 = (0x1 << POS_TIMER0),  
    
    // Timer1
    POS_TIMER1 = 5,
    INT_TIMER1 = (0x1 << POS_TIMER1),
    
    // VIC
    VIC_CNTL_ENABLE = 0x20
};

/******************************************************************************/
/* TIMER 0 */

/*
    Rutina de servicio para Timer0. Se interrumpe cada vez que vence el periodo
    dado, y encola un evento T0_PERIODO. La interrupción es FIQ.
*/
void timer0_RSI (void) __irq {
    // Contador de interrupciones Timer0
    static volatile uint32_t timer0_count = 0;
    
    // Trata la interrupción
    timer0_count++;
    cola_encolar_eventos_fiq(T0_PERIODO, timer0_count, 0);
    
    // Baja la interrupción
    T0IR = 1;
}

/*
    Inicia el periférico Timer0 con el periodo dado.

    periodo:    Tiempo en milisegundos tras el cual se realiza una interrupción

    Se configura como FIQ, el handler se define en el fichero Startup.s.
*/
void temporizador_reloj(uint32_t periodo) {
    // Ajusta la cuenta para contar ms e interrumpir cuando alcance periodo
    T0PR = TIME_MS;
    T0MR0 = periodo - 1;
    T0MCR |= MR0_INT | MR0_RESET;

    // Reinicia TC
    T0TCR |= TCR_ENABLE | TCR_RESET;
    T0TCR &= ~TCR_RESET;
    
    // Habilita FIQ
    VICIntSelect |= INT_TIMER0;
    VICIntEnable |= INT_TIMER0;
}

/******************************************************************************/
/* TIMER 1 */

static volatile uint32_t timer1_count = 0; // Contador de interrupciones Timer1

/*
    Rutina de servicio para Timer1. Se interrumpe cada vez desborda el contador,
    y encola un evento T1_OVERFLOW.
*/
void timer1_RSI (void) __irq {
    // Trata la interrupción
    timer1_count++;
    
    // Evento encolado para efectos de depuración
    cola_encolar_eventos(T1_OVERFLOW, timer1_count, 0);
    
    // Baja la interrupción
    T1IR = 1;
    VICVectAddr = 0;
}

/*
    Inicia el periférico Timer1. El timer queda asociado a VIC[2].
*/
void temporizador_iniciar() {
    // Ajusta la cuenta para contar en us e interrumpir cuando desborde
    T1PR = TIME_US;
    T1MR0 = 0xffffffff - 1;
    T1MCR |= MR0_INT | MR0_RESET;

    // Configura VIC[1] para tratar timer1
    VICVectAddr1 = (uint32_t) timer1_RSI;
    VICVectCntl1 |= VIC_CNTL_ENABLE | POS_TIMER1;
}

/*
    Inicia a contar el tiempo mediante timer1.
*/
void temporizador_empezar() {
    // Reinicia la cuenta de interrupciones
    timer1_count = 0;

    // Reinicia y arranca TC
    T1TCR |= TCR_ENABLE | TCR_RESET;
    T1TCR &= ~TCR_RESET;

    // Habilita interrupciones para timer1
    VICIntEnable |= INT_TIMER1;
}

/*
    Lee el instante de tiempo del Timer1
*/
uint32_t temporizador_leer() {
    // Lee tiempo actual
    uint32_t time = T1TC;
    
    // Añade las veces que se ha desbordado (cada ~71 min)
    for (int i = timer1_count; i > 0; i--) time += 0xffffffff;

    return time;
}

/*
    Devuelve el tiempo transcurrido en microsegundos
*/
uint32_t __SWI_0 (void) {
    return temporizador_leer();
}

/*
    Detiene el contador Timer1 junto al insante tiempo al detenerse
*/
uint32_t temporizador_parar() {    
    // Detiene timer
    T1TCR &= ~TCR_ENABLE;
    VICIntEnClr |= INT_TIMER1;

    return temporizador_leer();
}

/******************************************************************************/
/* RTC */

/*
    Inicia el contador RTC
*/
void RTC_init() {
    // Ajusta velocidad de cuenta para 60Mhz
    PREINT |= PREINT_60MHZ;
    PREFRAC |= PREFRAC_60MHZ;
    
    // Inicia timer
    CCR |= CCR_ENABLE | CCR_RESET;
    CCR &= ~CCR_RESET;
}

/*
    Devuelve los minutos y segundos almacenados por el RTC
*/
void RTC_leer(uint8_t *min, uint8_t *seg) {
    *min = MIN;
    *seg = SEC;
}

/*
    Devuelve el tiempo transcurrido en segundos
*/
uint32_t __SWI_1 (void) {
    uint8_t min, seg;
    RTC_leer(&min, &seg);
    
    return 60 * min + seg;
}

/******************************************************************************/
/* WD */

/*
    Inicia el contador Watchdog con el tiempo dado para reiniciar.

    sec:    Número de segundos a transcurrir antes de reiniciar el programa
*/
void WD_init(uint32_t sec) {
    // Establece el tiempo máximo antes de reiniciar
    WDTC = TIME_SEC * sec;
    WDMOD = WD_ENABLE | WD_RESET;
    
    // Inicia cuenta atrás
    WD_feed();
}

/*
    Alimenta al WD para reiniciar la cuenta atrás para reiniciar el programa.

    La secuencia de alimentación se debe realizar en exclusión mútua
*/
void WD_feed() {
    // Inicio sección crítica
    uint8_t irq = read_IRQ_bit(), fiq = read_FIQ_bit();
    if (!irq) disable_irq_fiq(); else if (!fiq) disable_fiq();
    
    // Secuencia de alimentación
    WDFEED = 0xaa;
    WDFEED = 0x55;
        
    // Final sección crítica
    if (!irq) enable_irq_fiq(); else if (!fiq) enable_fiq();
}
