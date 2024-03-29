/*
    Proyecto Hardware
    Práctica 2

    Fichero:
        G_Boton.c

    Autores:
        Dorian Boleslaw Wozniak (817570@unizar.es)
        Pablo Latre Villacampa (778043@unizar.es)

    Descripción: 
        Implementación de un gestor de botones, que permite comprobar si un
        botón está pulsado
*/

#include "G_Boton.h"
#include "boton.h"
#include "cola_msg.h"

/*
    Pre: ---
    Post: Inicializa el gestor de botones, incluyendo las interrupciones 
            externas EINT1 y EINT2
*/
void boton_iniciar() {
    eint1_iniciar();
    eint2_iniciar();
}

/*
    Pre: Recibido evento BOTON1
    Post: Encola una peticíon para programar una alarma periódica que compruebe
            si el botón 1 está pulsado, y encola un mensaje para realizar una
            jugada
*/
void boton1_pulsado() {
    cola_msg(SET_ALARMA, ((CHK_BOTON1 << 24) | (0x1 << 23) | (10)));
    cola_msg(SET_ALARMA, ((GO_SLEEP << 24) | (0x1 << 23) | (10000)));
}

/*  
    Pre: Recibido mensaje CHK_BOTON1
    Post: Si el botón 1 no está pulsado, rehabilita interrupciones y 
            encola una alarma para detener las comprobaciones
*/
void boton1_comprobar() {
   if (!eint1_leer()) {
       cola_msg(SET_ALARMA, (CHK_BOTON1 << 24) | (0 << 23) | (0));
       eint1_habilitar();
   }
}

/*
    Pre: Recibido evento BOTON2
    Post: Encola una peticíon para programar una alarma periódica que compruebe
            si el botón 2 está pulsado, y encola un mensaje para iniciar la 
            partida
*/
void boton2_pulsado() {
    cola_msg(SET_ALARMA, ((CHK_BOTON2 << 24) | (0x1 << 23) | (10)));
    cola_msg(SET_ALARMA, ((GO_SLEEP << 24) | (0x1 << 23) | (10000)));
}

/*  
    Pre: Recibido mensaje CHK_BOTON2
    Post: Si el botón 2 no está pulsado, rehabilita interrupciones y 
            encola una alarma para detener las comprobaciones
*/
void boton2_comprobar() {
   if (!eint2_leer()) {
       cola_msg(SET_ALARMA, (CHK_BOTON2 << 24) | (0 << 23) | (0));
       eint2_habilitar();
   }
}

#include <inttypes.h>


