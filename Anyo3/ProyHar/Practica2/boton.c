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

// Cuenta de interrupciones
static volatile uint32_t eint1_count = 0;
static volatile uint32_t eint2_count = 0;

// Rutinas de servicio

// eint1 cuando se pulsa botón 1
void eint1_RSI (void) __irq {
    // Trata la interrupción
    eint1_count++;
    cola_encolar_eventos(BOTON1, eint1_count, 0);
    // Baja interrupción y deshabilita EINT1 temporalmente
    VICIntEnClr |= (0x1 << 15);
    EXTINT |= (0x1 << 1);
    VICVectAddr= 0;
}

// eint2 cuando se pulsa botón 2
void eint2_RSI (void) __irq {
    // Trata la interrupción
    eint2_count++;
    cola_encolar_eventos(BOTON2, eint2_count, 0);
    // Baja interrupción y deshabilita EINT1 temporalmente
    VICIntEnClr |= (0x1 << 16);
    EXTINT |= (0x1 << 2);
    VICVectAddr= 0;
}

/*
    Pre:  ---
    Post: Asigna GPIO (14) como interrupción externa 1, y la asigna a VIC[2].
            Habilita las interrupciones de EINT1
*/
void eint1_iniciar() {
    // Selecciona GPIO(14) como EINT1
    PINSEL0 &= ~(0x3 << 28);
    PINSEL0 |= (0x2 << 28);
    
    // Activa EXTINT1 en VIC[2]
    VICVectAddr2 = (uint32_t) eint1_RSI;
    VICVectCntl2 = 0x20 | 15 ;
    
    eint1_habilitar();
}

/*
    Pre: eint1_iniciar()
    Post: Habilita interrupciones por EINT1
*/
void eint1_habilitar() {
    VICIntEnable |= (0x1 << 15);
}

/*
    Pre:  eint1_iniciar()
    Post: Devuelve el estado de la interrupción EINT1
*/
uint32_t eint1_leer() {
    EXTINT |= (0x1 << 1);
    VICVectAddr = 0;
    return ((EXTINT & (0x1 << 1)) >> 1);
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
    VICVectCntl3 = 0x20 | 16 ;
    
    eint2_habilitar();
}

/*
    Pre: eint2_iniciar()
    Post: Habilita interrupciones por EINT2
*/
void eint2_habilitar() {
    VICIntEnable |= (0x1 << 16);
}

/*
    Pre:  eint2_iniciar()
    Post: Devuelve el estado de la interrupción EINT1
*/
uint32_t eint2_leer() {
    EXTINT |= (0x1 << 2);
    VICVectAddr = 0;
    return ((EXTINT & (0x1 << 2)) >> 2);
}

