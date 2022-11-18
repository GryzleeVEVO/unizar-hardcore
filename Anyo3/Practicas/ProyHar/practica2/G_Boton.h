/*
    Proyecto Hardware
    Práctica 2

    Fichero:
        G_Boton.c

    Autores:
        Dorian Boleslaw Wozniak (817570@unizar.es)
        Pablo Latre Villacampa (778043@unizar.es)

    Descripción: 
        Interfaz de un gestor de botones, que permite comprobar si un
        botón está pulsado
*/

#ifndef G_BOTON_H
#define G_BOTON_H

/*
    Pre: Recibido evento BOTON1
    Post: Encola una peticíon para programar una alarma periódica que compruebe
            si el botón 1 está pulsado
*/
void boton1_pulsado(void);

/*  
    Pre: Recibido mensaje CHK_BOTON1
    Post: Si el botón 1 no está pulsado, rehabilita interrupciones y 
            encola una alarma para detener las comprobaciones
*/
void boton1_comprobar(void);

/*
    Pre: Recibido evento BOTON2
    Post: Encola una peticíon para programar una alarma periódica que compruebe
            si el botón 2 está pulsado
*/
void boton2_pulsado(void);

/*  
    Pre: Recibido mensaje CHK_BOTON2
    Post: Si el botón 2 no está pulsado, rehabilita interrupciones y 
            encola una alarma para detener las comprobaciones
*/
void boton2_comprobar(void);


#endif // G_BOTON_H
