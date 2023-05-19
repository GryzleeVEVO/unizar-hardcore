/*
    Proyecto Hardware
    Práctica 3

    Fichero:
        power.c

    Autores:
        Dorian Boleslaw Wozniak (817570@unizar.es)
        Pablo Latre Villacampa (778043@unizar.es)

    Descripción:
        Implementación de una serie de funciones para gestionar el estado
        de energía del procesador
*/

#include <LPC210X.h>
#include "power.h"

enum
{
    PCON_IDLE = 0x1,
    PCON_SLEEP = 0x2,
    WAKE_EINT_1_2 = (0x3 << 1)
};

/*
    Reinicia el PLL tras salid del modo dormir
*/
void reiniciar_pll(void);

/*
    Coloca el procesador en modo reposo
*/
void power_idle()
{
    // Entra en reposo
    PCON |= PCON_IDLE;
}

/*
    Coloca el procesador en modo dormir
*/
void power_down()
{
    // Permite despertar procesador al acitvar EINT1 o EINT2
    EXTWAKE |= WAKE_EINT_1_2;
    // Entra en power-down
    PCON |= PCON_SLEEP;
    reiniciar_pll();
}
