#include <inttypes.h>

#include "cola_eventos.h"
#include "cola_msg.h"

#include "G_Alarm.h"
#include "G_Energia.h"
#include "G_Boton.h"
#include "G_IO.h"

#include "tiempo.h"
#include "gpio.h"
#include "power.h"
#include "boton.h"

// Constantes globales para el planificador
enum {
    PERIODO = 1 // Periodo de actualización de timer0/alarma
};

/* Trata el evento recibido */
void tratar_evento(evento* e) {
    switch (e -> ID_evento) {
        
        // Contador periodico expira -> Comprueba alarmas pendientes
        case T0_PERIODO:    alarma_refrescar(PERIODO); break;
        
        // Temporizador desborda -> No hacer nada, solo depuración
        case T1_OVERFLOW:   break;
        
        // Boton 1 -> Tratar y comprobar periodicamente si sigue pulsado
        case BOTON1:        boton1_pulsado(); break;
        
        // Boton 2 -> Tratar y comprobar periodicamente si sigue pulsado
        case BOTON2:        boton2_pulsado(); break;
    }
}

/* Trata el mensaje recibido */
void tratar_mensaje (msg* m) {
    switch (m -> ID_msg) {
        
        // Programar/reprogramar/cancelar alarma
        case SET_ALARMA:    alarma_programar(m -> mensaje); break;
        
        // Comprobar si boton 1 sigue pulsado
        case CHK_BOTON1:    boton1_comprobar(); break;
           
        // Comprobar si boton 2 sigue pulsado
        case CHK_BOTON2:    boton2_comprobar(); break;
            
        // Mandar el procesador a dorimr
        case GO_SLEEP:      energia_dormir(); break;
    }
}


int main(void) {
    
    // Inicializa periféricos
    GPIO_iniciar();
    
    eint1_iniciar();
    eint2_iniciar();
    
    temporizador_iniciar();
    temporizador_empezar();
    
    temporizador_reloj(PERIODO);
    
    // Structs para almacenar lo desencolado
    evento e = {0, 0}; 
    msg m = {0, 0};
    
    // Bucle del planificador
    while (1) {
        // Mira si hay eventos pendientes
        if (cola_hay_eventos()) {
            cola_obtener_sig_evento(&e);
            tratar_evento(&e);
        }
        
        // Mira si hay mensajes pendientes
        else if (cola_hay_msg()) {
            cola_obtener_sig_msg(&m);
            tratar_mensaje(&m);
        }
        // Si no hay nada que hacer, entra en idle
        else { energia_reposo(); }
    }
}
