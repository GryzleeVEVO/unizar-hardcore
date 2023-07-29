#ifndef G_ENERGIA_H
#define G_ENERGIA_H

#include <inttypes.h>

/*
    Ordena al procesador a entrar en modo reposo (idle) en caso de no haber
    ninguna tarea por hacer en el planificador.
    
    El microprocesador detendá su reloj manteniendo en funcionamiento el de
    los periféricos, que podran reanudar el procesador al interrumpir
*/
void energia_reposo(void);

/*
    Ordena al procesador a entrar en modo dormir (power-down) al recibir un 
    mensaje Energia_Dormir.
    
    El microprocesador no ejecutará instrucciones ni procesará instrucciones.
    Se reactivará al recibir una interrupción externa EXTWAKE
*/
void energia_dormir(void);

/*
    Devuelve si el procesador no se ha dormido desde la última vez.
    Cambia el estado interno a despierto en caso contrario
*/
uint8_t energia_despertado(void);

#endif // G_ENERGIA_H

