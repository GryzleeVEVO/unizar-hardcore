/*
    Proyecto Hardware
    Práctica 2

    Fichero:
        G_Alarm.c

    Autores:
        Dorian Boleslaw Wozniak (817570@unizar.es)
        Pablo Latre Villacampa (778043@unizar.es)

    Descripción: 
        Implementación de un gestor de alarmas, que permite programar, 
        reprogramar y cancelar hasta 8 alarmas, y encola mensajes 
        cuando estas expiran según el contenido
*/

#include <inttypes.h>

#include "G_Alarm.h"
#include "cola_msg.h"
#include "tiempo.h"

// Constantes
enum {
    NUM_ALARMAS = 8
};

// Booleanos
enum {
    ERROR   = 0xFF
};

// Tipo alarma
typedef struct AlarmaGestor {
    uint8_t     mensaje;    // Mensaje programado
    uint8_t     periodico;  // Alarma periódica
    uint32_t    retardo;    // Retardo programado (ms)
    uint32_t    restante;   // Tiempo restante (ms)
} alarma ;

// Vector de alarmas
static __align(8) alarma alarmas[NUM_ALARMAS];

static uint8_t alarmas_activas = 0; // Cuenta de alarmas activas

/* 
    Pre: ---
    Post: Devuelve (bit_inicial + num_bits - 1, bit_inicial) = 1
*/
uint32_t alarma_mascara(uint8_t bit_inicial, uint8_t num_bits) {
    uint32_t mask = 0;

    for (uint8_t i = num_bits; i > 0; i--) { mask <<= 1; mask |= 0x1; }

    return mask << bit_inicial;
}

/*
    Pre: ---
    Post: Devuelve índice al alarma dado el tipo de mensaje programado
            Devuelve ERROR si no hay alarmas con ese tipo de mensaje
*/
uint8_t alarma_obtener(uint8_t mensaje) {
    for (int8_t i = 0; i < NUM_ALARMAS; i++)
        if (alarmas[i].mensaje == mensaje)
            return i;
    return ERROR;
}


/*
    Pre: ---
    Post: Inicializa el gestor de alarmas, incluyendo los periféricos Timer0 con
            el periodo dado y Timer1
*/
void alarma_iniciar(uint32_t alarm_periodo, uint32_t wd_periodo) {
    // Contador de tiempo
    temporizador_iniciar();
    temporizador_empezar();
    
    // Temporizador
    temporizador_reloj(alarm_periodo);
    
    // RTC
    RTC_init();
    
    // WD
    WD_init(wd_periodo);
}


/*
    Pre:  i < NUM_ALARMAS
    Post: Añade una alarma en el índice i con los parámetros dados
*/
void alarma_crear(uint8_t i, uint8_t mensaje, uint8_t periodico, 
                    uint32_t retardo) {
    alarmas[i].mensaje      = mensaje;
    alarmas[i].periodico    = periodico;
    alarmas[i].retardo      = retardo;
    alarmas[i].restante     = retardo;
}

/*
    Pre: i < NUM_ALARMAS
    Post: Elimina el alaram en el índice i
*/
void alarma_eliminar(uint8_t i) {
    alarmas[i].mensaje      = MSG_INDEFINIDO;
    alarmas[i].periodico    = 0;
    alarmas[i].retardo      = 0;
    alarmas[i].restante     = 0;
}

/*
    Pre: alarmas_activas < NUM_ALARMAS, recibido mensaje SET_ALARMA
    Post: Programa, reprograma o cancela la alarma dada por el mensaje dado:

          Si hay una alarma programada para el mensaje dado:
            - Si retardo = 0 -> Cancela 
            - Si no -> Reprograma con el retardo dado
          En caso contrario, programa una alarma con los parámetros del mensaje
*/
void alarma_programar(uint32_t msg) {
    
    // Obtiene los parámetros del mensaje
    uint8_t mensaje     = ((msg & alarma_mascara(24, 8)) >> 24);
    uint8_t periodico   = ((msg & alarma_mascara(23, 1)) >> 23);
    uint32_t retardo    = (msg & alarma_mascara(0, 23));
    
    // Busca si la alarma ya esta programada
    uint8_t i = alarma_obtener(mensaje);
    if (i != ERROR) {
        // Reprograma la alarma
        if (retardo != 0) {
            alarma_crear(i, mensaje, periodico, retardo);
        }
        // Cancela la alarma
        else {
            alarma_eliminar(i);
            alarmas_activas--;
        }
    }
    
    // Si no esta y hay espacio para mas alarmas, la introduce
    else if (alarmas_activas < NUM_ALARMAS && retardo != 0) {
        alarma_crear(alarma_obtener(0), mensaje, periodico, retardo);
        alarmas_activas++;
    }
}

/*
    Pre:  Recibido evento T0_PERIODO
    Post: Para todas las alarmas programadas, actualiza el tiempo hasta vencer
            la alarma. Si el tiempo a vencer es menor que el periodo 
            transcurrido, encola el mensaje programado
*/
void alarma_refrescar(uint32_t periodo) {  
    // Actualiza todas las alarmas activas
    for (uint8_t i = 0; i < NUM_ALARMAS; i++) if (alarmas[i].mensaje != MSG_INDEFINIDO) {
        
        // El alarma va a vencer, encola el mensaje 
        if (alarmas[i].restante <= periodo) {
            cola_msg(alarmas[i].mensaje, 0);
            
            // Es periodica, reiniciar
            if (alarmas[i].periodico == 1)
                alarmas[i].restante = alarmas[i].retardo;
            // No es periodico, eliminar
            else {
                alarma_eliminar(i);
                alarmas_activas--;
            }
        }
        
        // El alarma no vence, actualiza cuenta
        else { alarmas[i].restante -= periodo; }
    }
}
