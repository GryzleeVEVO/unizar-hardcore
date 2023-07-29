#include <LPC210X.h>
#include "power.h"

void reiniciar_pll(void);

/*
    Coloca el procesador en modo reposo
*/
void power_idle() {
    // Entra en reposo
    PCON |= 0x01;
}

/*
    Coloca el procesador en modo dormir
*/
void power_down() {
    // Permite despertar procesador al acitvar EINT1 o EINT2
    EXTWAKE |=  ((0x3) << 1);
    // Entra en power-down
    PCON |= 0x02;
    reiniciar_pll();
}
