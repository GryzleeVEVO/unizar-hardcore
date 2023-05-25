#include "G_Energia.h"
#include "power.h"

/*
    Ordena al procesador a entrar en modo reposo (idle) en caso de no haber
    ninguna tarea por hacer en el planificador.

    El microprocesador detendá su reloj manteniendo en funcionamiento el de
    los periféricos, que podran reanudar el procesador al interrumpir
*/
void energia_reposo(void)
{
    power_idle();
}

/*
    Ordena al procesador a entrar en modo dormir (power-down) al recibir un
    mensaje Energia_Dormir.

    El microprocesador no ejecutará instrucciones ni procesará instrucciones.
    Se reactivará al recibir una interrupción externa EINT1 o EINT2;
*/
void energia_dormir(void)
{
    power_down();
}
