/*
    Proyecto Hardware
    Práctica 3

    Fichero:
        uart.c

    Autores:
        Dorian Boleslaw Wozniak (817570@unizar.es)
        Pablo Latre Villacampa (778043@unizar.es)

    Descripción:
        Implementación de una serie de funciones para interactuar con la línea
        serie del sistema. Ofrece funcionalidades para conversión de caracteres
        a enteros y viceversa, lectura y escritura de datos cualquiera en
        la línea, y mensajes predefinidos a transmitir.
*/

#include <inttypes.h>

#include "G_serie.h"
#include "cola_msg.h"
#include "uart.h"

enum
{
    BUFSIZE = 3
};

/******************************************************************************/
/* ENTERO <---> CARACTER */

/*
    Convierte un digito a caracter

    i: Entero a convertir
*/
char serie_itoa(uint8_t i) { return i + '0'; }

/*
    Convierte a caracter y envía a la línea serie un entero.

    num: Entero a convertir
*/
void serie_itoa_wide(uint32_t num)
{
    char buffer[32] = {0};

    uint8_t index = 31;
    uint32_t aux = num;

    do
    {
        buffer[index--] = serie_itoa(aux % 10);
    } while ((aux /= 10) > 0);

    index++;

    for (uint8_t i = 0; index < 32; i++)
    {
        buffer[i] = buffer[index];
        buffer[index++] = '\0';
    }

    uart0_enviar_array(buffer);
}

/*
    Convierte un caracter a entero

    i: Caracter a convertir. Debe ser un caracter entre '0' y '9'
*/
uint8_t serie_atoi(char i) { return i - '0'; }

/******************************************************************************/
/* EXPORTADO */

/*
    Inicializa la línea serie
*/
void serie_iniciar()
{
    uart0_init();
}

/*
    Interpreta el caracter obtenido de la línea serie

    data: Caracter obtenido
*/
void serie_leer(uint32_t data)
{
    static uint8_t buffer[BUFSIZE] = {0};
    static uint8_t index = 0;
    static uint8_t esperando_delim = 1;

    uint8_t ch = data & 0xff;

    uart0_echo(ch);

    if (ch == '#')
    {
        esperando_delim = 0;
        for (int i = 0; i < BUFSIZE; i++)
        {
            buffer[i] = 0;
        }
        index = 0;
    }

    else if (!esperando_delim)
    {
        if (ch == '!')
        {
            uart0_enviar_array("\n\n");
            if (buffer[0] == 'E' && buffer[1] == 'N' && buffer[2] == 'D')
            {
                cola_msg(ACABAR, 3);
            }
            else if (buffer[0] == 'N' && buffer[1] == 'E' && buffer[2] == 'W')
            {
                cola_msg(INICIAR, 0);
            }
            else if ('1' <= buffer[0] && buffer[0] <= '7' && buffer[1] == 0 && buffer[2] == 0)
            {
                cola_msg(CHK_ENTRADA, serie_atoi(buffer[0]));
            }
            else
            {
                serie_print("Comando incorrecto\n");
            }

            esperando_delim = 1;
        }
        else if (index < 3)
        {
            buffer[index] = ch;
            index++;
        }
        else
        {
            esperando_delim = 1;
        }
    }
}

/*
    Escribe el siguiente caracter del búfer de la línea serie
*/
void serie_escribir()
{
    uart0_continuar_envio();
}

/*
    Genera un mensaje de bienvenida al iniciar
*/
void serie_pantalla_bienvenida()
{
    serie_print("\nBienvenido a Conecta4.\n");
    serie_print("Para jugar, pulse cualquier boton o escriba #NEW!");
    serie_print("Para seleccionar una columna, escriba #C!, donde C es un numero de 1 a 7.");
    serie_print("Tienes 1 segundo para cancelar la jugada. Pulse el boton 1 para cancelarla.");
    serie_print("Pulse el boton 2 o escriba #END! para reiniciar la partida\n");
}

/*
    Genera un mensaje final.

    t_juego: Tiempo tomado en ejecutar el programa
    t_medio: Tiempo medio de procesamiento de mensajes
*/
void serie_mensaje_reinicio(uint32_t t_juego, uint32_t t_medio)
{
    uart0_enviar_array("La partida ha durado ");
    serie_itoa_wide(t_juego);
    serie_print(" segundos");
    uart0_enviar_array("Se ha tardado ");
    serie_itoa_wide(t_medio);
    serie_print(" microsegundos de media para procesar un mensaje");
    serie_print("\nPara jugar, pulse cualquier boton o escriba #NEW!");
}

/*
    Envía una cadena cualquiera a la línea serie.

    linea: Cadena a enviar. Debe acabar en NULL

    Se añade un salto de línea a la línea añadida
*/
void serie_print(char *linea)
{
    uart0_enviar_array(linea);
    uart0_enviar_array("\n");
}
