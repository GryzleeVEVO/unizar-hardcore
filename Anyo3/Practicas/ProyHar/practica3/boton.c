/*
    Proyecto Hardware
    Práctica 2

    Fichero:
        boton.c

    Autores:
        Dorian Boleslaw Wozniak (817570@unizar.es)
        Pablo Latre Villacampa (778043@unizar.es)

    Descripción: 
        Implementación de métodos para inicializar y utilizar los periféricos
        externos eint1 y eint2
*/

#include <LPC210X.h>

#include "boton.h"
#include "cola_eventos.h"

// Constantes para interrupciones vectorizadas
enum {
    POS_EINT1 = 15, // Interrupción 15
    POS_EINT2 = 16, // Interrupción 16
    
    INT_EINT1 = (0x1 << POS_EINT1), // Bit 15
    INT_EINT2 = (0x1 << POS_EINT2), // Bit 16
    
    VIC_CNTL_ENABLE = 0x20
};

// Constantes para EINT y PINSEL
enum {
    PIN_14_0 = (0x3 << 28),
    PIN_14_2 = (0x2 << 28),
    
    //PIN_15_0 = (0x3 << 30),
    //PIN_15_2 = (0x2 << 30),
    
    EXTINT_1 = (0x1 << 1),
    EXTINT_2 = (0x1 << 2)
};

// Rutinas de servicio

// eint1 cuando se pulsa botón 1
void eint1_RSI (void) __irq {
    static volatile uint32_t eint1_count = 0;
    
    // Trata la interrupción
    eint1_count++;
    cola_encolar_eventos(BOTON1, eint1_count, 0);
    // Baja interrupción y deshabilita EINT1 temporalmente
    VICIntEnClr |= INT_EINT1;
    EXTINT |= EXTINT_1;
    VICVectAddr= 0;
}

// eint2 cuando se pulsa botón 2
void eint2_RSI (void) __irq {
    static volatile uint32_t eint2_count = 0;

    // Trata la interrupción
    eint2_count++;
    cola_encolar_eventos(BOTON2, eint2_count, 0);
    // Baja interrupción y deshabilita EINT1 temporalmente
    VICIntEnClr |= INT_EINT2;
    EXTINT |= EXTINT_2;
    VICVectAddr= 0;
}

/*
    Pre:  ---
    Post: Asigna GPIO (14) como interrupción externa 1, y la asigna a VIC[2].
            Habilita las interrupciones de EINT1
*/
void eint1_iniciar() {
    // Selecciona GPIO(14) como EINT1
    PINSEL0 &= ~PIN_14_0;
    PINSEL0 |= PIN_14_2;
    
    // Activa EXTINT1 en VIC[2]
    VICVectAddr2 = (uint32_t) eint1_RSI;
    VICVectCntl2 = VIC_CNTL_ENABLE | POS_EINT1 ;
    
    eint1_habilitar();
}

/*
    Pre: eint1_iniciar()
    Post: Habilita interrupciones por EINT1
*/
void eint1_habilitar() {
    VICIntEnable |= INT_EINT1;
}

/*
    Pre:  eint1_iniciar()
    Post: Devuelve el estado de la interrupción EINT1
*/
uint32_t eint1_leer() {
    EXTINT |= EXTINT_1;
    VICVectAddr = 0;
    return ((EXTINT & EXTINT_1) >> 1);
}

/*
    Pre:  ---
    Post: Asigna GPIO (15) como interrupción externa 2, y la asigna a VIC[3].
            Habilita las interrupciones de EINT2
*/
void eint2_iniciar() {
    // Selecciona GPIO(15) como EINT2
    PINSEL0 &= ~(0xc0000000);
    PINSEL0 |= (0x80000000);
    
    // Activa EXTINT3 en VIC[3]
    VICVectAddr3 = (uint32_t) eint2_RSI;
    VICVectCntl3 = VIC_CNTL_ENABLE | POS_EINT2 ;
    
    eint2_habilitar();
}

/*
    Pre: eint2_iniciar()
    Post: Habilita interrupciones por EINT2
*/
void eint2_habilitar() {
    VICIntEnable |= INT_EINT2;
}

/*
    Pre:  eint2_iniciar()
    Post: Devuelve el estado de la interrupción EINT1
*/
uint32_t eint2_leer() {
    EXTINT |= EXTINT_2;
    VICVectAddr = 0;
    return ((EXTINT & EXTINT_2) >> 2);
}
