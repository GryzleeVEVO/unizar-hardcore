/*
    Proyecto Hardware
    Práctica 3

    Fichero:
        power.c

    Autores:
        Dorian Boleslaw Wozniak (817570@unizar.es)
        Pablo Latre Villacampa (778043@unizar.es)

    Descripción: 
        Interfaz de una serie de funciones para gestionar el estado
        de energía del procesador
*/

#ifndef POWER_H
#define POWER_H

/*
    Coloca el procesador en modo reposo
*/
void power_idle(void);

/*
    Coloca el procesador en modo dormir
*/
void power_down(void);

#endif // POWER_H
