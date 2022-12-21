#include <inttypes.h>

#include "G_serie.h"
#include "cola_msg.h"
#include "uart.h"

enum{
    BUFSIZE = 3    
};

void serie_iniciar() {
    uart0_init();
}

void serie_leer(uint32_t data) {
    static uint8_t buffer[BUFSIZE] = {0};
    static uint8_t index = 0;
    static uint8_t esperando_delim = 1;
    
    uint8_t ch = data & 0xff;
    
    uart0_echo(ch);
    
    if (ch == '#') {
        esperando_delim = 0; 
        for (int i = 0; i < BUFSIZE; i++) {
            buffer[i] = 0;
        }
        index = 0;
    }
    
    else if (!esperando_delim) {
        if (ch == '!') { 
            uart0_enviar_array("\n\n");
            if (buffer[0] == 'E' && buffer[1] == 'N' && buffer[2] == 'D') {
                cola_msg(ACABAR, 3);
            }
            else if (buffer[0] == 'N' && buffer[1] == 'E' && buffer[2] == 'W') {
                cola_msg(INICIAR, 0);
            } 
            else if ('1' <= buffer[0] && buffer[0] <= '7' && buffer[1] == 0 && buffer[2] == 0) {
                cola_msg(CHK_ENTRADA, serie_atoi(buffer[0]));
            }
            else {
                uart0_enviar_array("Comando incorrecto\n");
            }
            
            esperando_delim = 1;
        }
        else if (index < 3) {
            buffer[index] = ch;
            index++;
        } 
        else {
            esperando_delim = 1;
        }
    }
}

void serie_escribir() {
    uart0_continuar_envio();
}

void serie_pantalla_bienvenida() {
    serie_print("\nBienvenido a Conecta4.\n");
    serie_print("Para jugar, pulse cualquier boton o escriba #NEW!");
    serie_print("Para seleccionar una columna, escriba #C!, donde C es un numero de 1 a 7.");
    serie_print("Tienes 1 segundo para cancelar la jugada. Pulse el boton 1 para cancelarla.");
    serie_print("Pulse el boton 2 o escriba #END! para reiniciar la partida\n");
}

void serie_mensaje_reinicio() {
    serie_print("Para jugar, pulse cualquier boton o escriba #NEW!");
}

void serie_print(char* linea) {
    uart0_enviar_array(linea);
    uart0_enviar_array("\n");
}

char serie_itoa(uint8_t i) { return i + 48; }

uint8_t serie_atoi(char i) { return i - 48; }
