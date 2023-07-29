/*
    Proyecto Hardware
    Práctica 2

    Fichero:
        boton.h

    Autores:
        Dorian Boleslaw Wozniak (817570@unizar.es)
        Pablo Latre Villacampa (778043@unizar.es)

    Descripción: 
        Interfaz de métodos para inicializar y utilizar los periféricos
        externos eint1 y eint2
*/

#ifndef BOTON_H
#define BOTON_H

#include <inttypes.h>

/*
    Pre:  ---
    Post: Asigna GPIO (14) como interrupción externa 1, y la asigna a VIC[2].
            Habilita las interrupciones de EINT1
*/
void eint1_iniciar(void);
/*
    Pre: eint1_iniciar()
    Post: Habilita interrupciones por EINT1
*/
void eint1_habilitar(void);

/*
    Pre:  eint1_iniciar()
    Post: Devuelve el estado de la interrupción EINT1
*/
uint32_t eint1_leer(void);

/*
    Pre:  ---
    Post: Asigna GPIO (15) como interrupción externa 2, y la asigna a VIC[3].
            Habilita las interrupciones de EINT2
*/
void eint2_iniciar(void);

/*
    Pre: eint2_iniciar()
    Post: Habilita interrupciones por EINT2
*/
void eint2_habilitar(void);

/*
    Pre:  eint2_iniciar()
    Post: Devuelve el estado de la interrupción EINT2
*/
uint32_t eint2_leer(void);

#endif // BOTON_H
