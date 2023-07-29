/*
    Proyecto Hardware
    Práctica 3

    Fichero:
        uart.h

    Autores:
        Dorian Boleslaw Wozniak (817570@unizar.es)
        Pablo Latre Villacampa (778043@unizar.es)

    Descripción:
        Interfaz de una serie de funciones para interactuar con la línea
        serie del sistema. Ofrece funcionalidades para conversión de caracteres
        a enteros y viceversa, lectura y escritura de datos cualquiera en
        la línea, y mensajes predefinidos a transmitir.
*/

#ifndef G_SERIE_H
#define G_SERIE_H

#include <inttypes.h>

/*
    Convierte un digito a caracter

    i: Entero a convertir
*/
char serie_itoa(uint8_t i);

/*
    Inicializa la línea serie
*/
void serie_iniciar(void);

/*
    Interpreta el caracter obtenido de la línea serie

    data: Caracter obtenido
*/
void serie_leer(uint32_t data);

/*
    Escribe el siguiente caracter del búfer de la línea serie
*/
void serie_escribir(void);

/*
    Genera un mensaje de bienvenida al iniciar
*/
void serie_pantalla_bienvenida(void);

/*
    Genera un mensaje final.

    t_juego: Tiempo tomado en ejecutar el programa
    t_medio: Tiempo medio de procesamiento de mensajes
*/
void serie_mensaje_reinicio(uint32_t t_juego, uint32_t t_medio);

/*
    Envía una cadena cualquiera a la línea serie.

    linea: Cadena a enviar. Debe acabar en NULL

    Se añade un salto de línea a la línea añadida
*/
void serie_print(char *linea);

#endif // G_SERIE_H
