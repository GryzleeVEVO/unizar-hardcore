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
#include "G_serie.h"

#include "tiempo.h"

// Constantes globales para el planificador
enum {
    ALARM_PERIODO = 10,      // Periodo de actualización de timer0/alarma
    WD_FEED_PERIODO = 1000   // Periodo de alimentación de watchdog
};

// Indica si el procesador se ha dormido
static uint8_t dormido = 0;

/* Trata el evento recibido */
void tratar_evento(evento* e) {    
    switch (e -> ID_evento) {
        // Timer 0 -> Comprobar alarmas
        case T0_PERIODO:
            alarma_refrescar(ALARM_PERIODO); 
            break;
        
        // Timer 1 -> Nada
        case T1_OVERFLOW:
            break;
        
        // Boton 1 -> Realizar jugada
        case BOTON1:
            cola_msg(SET_ALARMA, ALARM_GO_SLEEP_MSG);
            boton1_pulsado();
            if (dormido == 0) {
                conecta4_realizar_jugada(); 
            } else dormido = 0;
            break;
        
        // Boton 2 -> Reinicia la partida
        case BOTON2:
            cola_msg(SET_ALARMA, ALARM_GO_SLEEP_MSG);
            boton2_pulsado();
            if (dormido == 0) {
                IO_nueva_partida(); 
                conecta4_iniciar();
            } else dormido = 1;
            break;
        
        case UART0_LEER:
            serie_leer(e -> auxData);
            break;
        
        case UART0_ESCRIBIR:
            serie_escribir();
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
            if (conecta4_comprobar_entrada()) {
                cola_msg(SET_ALARMA, ALARM_GO_SLEEP_MSG);
            }
            break;
        
        // Alternar estado latido
        case PARPADEAR:
            IO_cambiar_latido();
            break;
        
        // Baja pin jugada realizada
        case OFF_REALIZAR:
            IO_bajar_jugada_realizada();
            break;
        
        //case SERIE_ACCION:
         //   serie_escribir();
          //  break;
        
        // Mandar el procesador a dormir
        case GO_SLEEP:      
            dormido = 1;
            energia_dormir();
            break;
           
        case FEED_WD:
            WD_feed();
            break;
    }
}


int main(void) {
    // Inicializa periféricos
    serie_iniciar();    
    IO_iniciar();
    boton_iniciar();
    alarma_iniciar(ALARM_PERIODO, WD_FEED_PERIODO);
    
    // Structs para almacenar lo desencolado
    evento e = {0, 0}; 
    msg m = {0, 0};
    
    // Prepara la partida
    cola_msg(SET_ALARMA, ALARM_GO_SLEEP_MSG);
    cola_msg(SET_ALARMA, ALARM_CHK_ENT_MSG);
    cola_msg(SET_ALARMA, ALARM_PARPADEAR_MSG);
    cola_msg(SET_ALARMA, ALARM_FEED_WD_MSG);
    
    IO_nueva_partida(); // Configura GPIO
    conecta4_iniciar(); // Inicializa Conecta 4
    serie_pantalla_bienvenida();
    serie_pantalla_bienvenida();
    serie_pantalla_bienvenida();


        
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