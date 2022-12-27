/*
    Proyecto Hardware
    Práctica 2

    Fichero:
        boton.h

    Autores:
        Dorian Boleslaw Wozniak (817570@unizar.es)
        Pablo Latre Villacampa (778043@unizar.es)

    Descripción: 
        Interfaz de una serie de interrupciones para interactuar con los
        periféricos externos EINT1 y EINT2
*/

#ifndef BOTON_H
#define BOTON_H

#include <inttypes.h>

/******************************************************************************/
/* EINT1 */


/*
    Inicializa la interrupción externa EINT1
*/
void eint1_iniciar(void);

/*
    Habilita EINT1 para poder interrumpir
*/
void eint1_habilitar(void);

/*
    Obtiene el estado de la interrupción EINT1
*/
uint32_t eint1_leer(void);

/******************************************************************************/
/* EINT2 */

/*
    Rutina de servicio para EINT2
*/
void eint2_iniciar(void);

/*
    Inicializa la interrupción externa EINT2
*/
void eint2_habilitar(void);

/*
    Obtiene el estado de la interrupción EINT2
*/
uint32_t eint2_leer(void);

#endif // BOTON_H
