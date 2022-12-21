#include <inttypes.h>

#include "G_serie.h"
#include "cola_msg.h"
#include "cola_buffer.h"
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
    
    if (esperando_delim && ch == '#') {
        esperando_delim = 0; 
        for (int i = 0; i < BUFSIZE; i++) {
            buffer[i] = 0;
        }
        index = 0;
    }
    
    else {
        if (ch == '!') { 
            if (buffer[0] == 'S' && buffer[1] == 'E' && buffer[2] == 'T') {
                cola_msg(SERIE_ACCION, 0);
            }    
        }
        else if (index < 3) {
            buffer[index] = ch;
            index++;
        } 
        else {
            esperando_delim = 1;
        }
    }
    uart0_echo(ch);
}

void serie_escribir() {
    if (!buffer_vacio()) {
        uart0_continuar_envio(buffer_siguiente());
    }
}

void serie_pantalla_bienvenida() {
    buffer_anyadir("Bienvenido a Conecta4.\n\nPara jugar, pulse cualquier boton o escriba #NEW!\nPara seleccionar una columna, escriba #C!, donde C es un numero de 1 a 7.\nTienes 1 segundo para cancelar la jugada. Pulse el boton 1 para cancelarla.\nPulse el boton 2 o escriba #END! para reiniciar la partida\n", 288);
    
    uart0_enviar_array(buffer_siguiente());
}
