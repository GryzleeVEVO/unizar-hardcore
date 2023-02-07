/*
    Proyecto Hardware
    Práctica 3

    Fichero:
        main.c

    Autores:
        Dorian Boleslaw Wozniak (817570@unizar.es)
        Pablo Latre Villacampa (778043@unizar.es)

    Descripción: 
        Implementación de un planificador que gestiona eventos síncronos, 
        generados por módulos asociados al planificador; y eventos asíncronos,
        generados por periféricos.
        
        El planificador está asociado a un juego de Conecta 4. Al iniciar, 
        informa sobre cómo jugar. Al iniciar partida, gestiona el comportamiento
        de los periféricos y el paso de mensajes. 
        
        Además, al finalizar, muestra el tiempo medio en procesar un mensaje
        en microsegundos, y el tiempo en segundos dedicado al juego.
*/

#include <inttypes.h>

// Programa principal
#include "conecta4_2022.h"

// Colas
#include "cola_eventos.h"
#include "cola_msg.h"

// Gestores
#include "G_Alarm.h"
#include "G_Energia.h"
#include "G_Boton.h"
#include "G_IO.h"
#include "G_serie.h"

#include "tiempo.h"             // Para SWI y WD

// Constantes globales para el planificador
enum {
    ALARM_PERIODO   = 10, // Periodo de actualización de timer0/alarma (ms)
    WD_FEED_PERIODO = 1   // Periodo de alimentación de watchdog (s)
};

static uint8_t dormido = 0; // Indica si el procesador se ha dormido
static uint8_t jugando = 0; // Indica si hay una partida en curso

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
        
        // Boton 1 -> Cancela jugada (o inicia partida si no ha empezado)
        case BOTON1:
            cola_msg(SET_ALARMA, ALARM_GO_SLEEP_MSG);
            boton1_pulsado();
            if (!dormido) {
                if (jugando) conecta4_cancelar_jugada();  
                else cola_msg(INICIAR, 0);
            } else dormido = 0;
            break;
        
        // Boton 2 -> Detiene la partida (o inicia partida si no ha empezado)
        case BOTON2:
            cola_msg(SET_ALARMA, ALARM_GO_SLEEP_MSG);
            boton2_pulsado();
            if (dormido == 0) {
                if (jugando) {
                    cola_msg(ACABAR, 3);
                } else cola_msg(INICIAR, 0);
            } else dormido = 1;
            break;
        
        // UART0 obtiene un caracrer -> Interpreta lo obtenido
        case UART0_LEER:
            cola_msg(SET_ALARMA, ALARM_GO_SLEEP_MSG);
            serie_leer(e -> auxData);
            break;
        
        // UART1 termina de escribir un caracter -> Envia siguiente carácter 
        case UART0_ESCRIBIR:
            serie_escribir();
            break;
    }
}

/* Trata el mensaje recibido */
void tratar_mensaje (msg* m) {
    // Estadísticas de la última partida
    static uint32_t tiempo_total_msg = 0;    // Tiempo total tratando mensajes
    static uint32_t mensajes_procesados = 0; // Mensajes totales procesados
    static uint32_t tiempo_juego = 0;        // Tiempo de inicio del juego

    uint32_t start = clock_getus(); // Empieza a medir siguiente tratamiento  
    
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
        
        // Lee entrada obtenida y prepara acción si fuese necesario
        case CHK_ENTRADA:
            if (jugando) conecta4_comprobar_entrada(m -> mensaje);
            break;
        
        // Alternar estado del latido
        case PARPADEAR:
            IO_cambiar_latido();
            break;
        
        // Baja pin de jugada realizada
        case OFF_REALIZAR:
            IO_bajar_jugada_realizada();
            break;
        
        // Inicia una nueva partida
        case INICIAR:
            if (!jugando) {
                conecta4_iniciar();
                jugando = 1;
                tiempo_total_msg = 0;
                mensajes_procesados = 0;
                tiempo_juego = clock_gettime();
            }
            break;
        
        // Compromete la jugada realizada si no es cancela
        case SIGUIENTE:
            conecta4_realizar_jugada();
            break;
        
        // Actúa si se acaba la partida
        case ACABAR:
            if (jugando) {
                conecta4_detener(m -> mensaje);
                
                // Calcula tiempo de juego y QoS
                tiempo_total_msg += clock_getus() - start;
                tiempo_juego = clock_gettime() - tiempo_juego;
                serie_mensaje_reinicio(tiempo_juego, tiempo_total_msg / mensajes_procesados);
                jugando = 0;
            }
            break;
        
        // Mandar el procesador a dormir
        case GO_SLEEP:      
            dormido = 1;
            energia_dormir();
            break;
           
        // Alimenta Watchdog para no reiniciar
        case FEED_WD:
            WD_feed();
            break;
    }
    
    tiempo_total_msg += clock_getus() - start; mensajes_procesados++;
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
    cola_msg(SET_ALARMA, ALARM_PARPADEAR_MSG);
    cola_msg(SET_ALARMA, ALARM_FEED_WD_MSG);

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
