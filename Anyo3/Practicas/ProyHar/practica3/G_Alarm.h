/*
    Proyecto Hardware
    Práctica 2

    Fichero:
        G_Alarm.h

    Autores:
        Dorian Boleslaw Wozniak (817570@unizar.es)
        Pablo Latre Villacampa (778043@unizar.es)

    Descripción: 
        Interfaz de un gestor de alarmas, que permite programar, 
        reprogramar y cancelar hasta 8 alarmas, y encola mensajes 
        cuando estas expiran según el contenido
*/

#ifndef G_ALARM_H
#define G_ALARM_H

#include <inttypes.h>
#include "msg.h"
#include "alarmas.h"

/*
    Pre: ---
    Post: Inicializa el gestor de alarmas, incluyendo los periféricos Timer0 con
            el periodo dado y Timer1
*/
void alarma_iniciar(uint32_t alarm_periodo, uint32_t wd_periodo);

/*
    Pre: alarmas_activas < NUM_ALARMAS, recibido mensaje SET_ALARMA
    Post: Programa, reprograma o cancela la alarma dada por el mensaje dado:

          Si hay una alarma programada para el mensaje dado:
            - Si retardo = 0 -> Cancela 
            - Si no -> Reprograma con el retardo dado
          En caso contrario, programa una alarma con los parámetros del mensaje
*/
void alarma_programar(uint32_t msg);

/*
    Pre:  Recibido evento T0_PERIODO
    Post: Para todas las alarmas programadas, actualiza el tiempo hasta vencer
            la alarma. Si el tiempo a vencer es menor que el periodo 
            transcurrido, encola el mensaje programado
*/
void alarma_refrescar(uint32_t periodo);

#endif // G_ALARM_H


