/*
    Proyecto Hardware
    Práctica 2

    Fichero:
        main.c

    Autores:
        Dorian Boleslaw Wozniak (817570@unizar.es)
        Pablo Latre Villacampa (778043@unizar.es)

    Descripción: 
        Fuente principal del proyecto. Implementa un planificador basado en
        eventos. A través de colas de eventos y mensajes permite la comunicación
        entre módulos, periféricos y la implementación del juego Conecta 4.
*/

#include <inttypes.h>

#include "conecta4_2022.h"

#include "cola_eventos.h"
#include "cola_msg.h"

#include "G_Alarm.h"
#include "G_Energia.h"
#include "G_Boton.h"
#include "G_IO.h"

// Constantes globales para el planificador
enum {
    PERIODO = 10 // Periodo de actualización de timer0/alarma
};


/*
    El planificador reinicia el juego y configura alarmas
*/
void reiniciar() {
    
    // Inicia alarmas para eventos periodicos
    cola_msg(SET_ALARMA, ALARM_GO_SLEEP_MSG);
    cola_msg(SET_ALARMA, ALARM_CHK_ENT_MSG);
    cola_msg(SET_ALARMA, ALARM_PARPADEAR_MSG);
    
    IO_nueva_partida(); // Configura GPIO
    conecta4_iniciar(); // Inicializa Conecta 4
}

/*
    El planificador obtiene la entrada actual y comprueba si es válida.
    Si no fuese válida, lo marca en el GPIO
*/
uint8_t comprobar_entrada() {
    uint8_t columna = IO_leer_entrada();
    
    if (conecta4_comprobar_columna(columna)) {
        IO_indicar_jugada_valida();
        return TRUE;
    } 
    else { 
        IO_indicar_jugada_invalida();
        return FALSE; 
    }
}

/*
    El planificador, si tiene una entrada válida, realiza la jugada. Realiza
    una acción según el resultado que le indique el módulo Conecta 4
*/
void realizar_jugada() {
    if (comprobar_entrada()) {
        switch (conecta4_jugar()) {
            case C4_SEGUIR: // La partida no acaba    
                IO_jugada_realizada();
                cola_msg(SET_ALARMA, ALARM_APAGAR_REAL_MSG);
                break;
                    
            case C4_VICTORIA_B:
            case C4_VICTORIA_N:
                IO_victoria();
                
                break;
            
            case C4_EMPATE:
                IO_empate();
        }
    }
}

/* Trata el evento recibido */
void tratar_evento(evento* e) {
    switch (e -> ID_evento) {
        // Timer 0 -> Comprobar alarmas
        case T0_PERIODO:
            alarma_refrescar(PERIODO); 
            break;
        
        // Timer 1 -> Nada
        case T1_OVERFLOW:
            break;
        
        // Boton 1 -> Realizar jugada
        case BOTON1:
            boton1_pulsado();
            if (energia_despertado() && !conecta4_detenido()) {
                realizar_jugada();
            }
            break;
        
        // Boton 2 -> Reinicia la partida
        case BOTON2:
            boton2_pulsado();
            if (energia_despertado()) {
                reiniciar();
            }
            break;
    }
}

/* Trata el mensaje recibido */
void tratar_mensaje (msg* m) {
    switch (m -> ID_msg) {
        // Programar/reprogramar/cancelar alarma
        case SET_ALARMA:
            alarma_programar(m -> mensaje); 
            break;
         
        // Comprobar si boton 1 sigue pulsado
        case CHK_BOTON1:    
            boton1_comprobar(); 
            break;
           
        // Comprobar si boton 2 sigue pulsado
        case CHK_BOTON2:    
            boton2_comprobar(); 
            break;
        
        // Comprobar entrada
        case CHK_ENTRADA:
            comprobar_entrada();
            break;
        
        // Alternar estado latido
        case PARPADEAR:
            IO_cambiar_latido();
            break;
        
        // Baja pin jugada realizada
        case APAGAR_REAL:
            IO_bajar_jugada_realizada();
            break;
        
        // Mandar el procesador a dormir
        case GO_SLEEP:      
            energia_dormir();
            break;
    }
}


int main(void) {
    
    // Inicializa periféricos
    IO_iniciar();
    boton_iniciar();
    alarma_iniciar(PERIODO);
    
    // Structs para almacenar lo desencolado
    evento e = {0, 0}; 
    msg m = {0, 0};
    
    // Prepara la partida
    reiniciar();
    
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
