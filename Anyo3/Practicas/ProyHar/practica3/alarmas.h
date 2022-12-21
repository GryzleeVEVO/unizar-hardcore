#ifndef ALARMAS_H
#define ALARMAS_H

#include "msg.h"

enum {
    PERIODICO       = (0x1 << 23),
    NO_PERIODICO    = (0x0 << 23)
};

// Mensajes predefinidos para alarmas
// Formato: MSG (31, 24) | PERIODICO (23) | MS (22, 0)
enum {
    // Comprueba botones cada 10ms
    ALARM_CHK_BOTON1_ON_MSG     = (CHK_BOTON1 << 24)    | PERIODICO     | 10,
    ALARM_CHK_BOTON2_ON_MSG     = (CHK_BOTON2 << 24)    | PERIODICO     | 10,
    
    // Apaga alarmas para botones
    ALARM_CHK_BOTON1_OFF_MSG    = (CHK_BOTON1 << 24)    | NO_PERIODICO  | 0,
    ALARM_CHK_BOTON2_OFF_MSG    = (CHK_BOTON2 << 24)    | NO_PERIODICO  | 0,
    
     // Comprueba valor entrada cada 100ms
    ALARM_CHK_ENT_MSG           = (CHK_ENTRADA << 24)   | PERIODICO     | 100,
    
    // Alterna estado latido cada 250ms (parpadea cada 0,5 segundos)
    ALARM_PARPADEAR_MSG         = (PARPADEAR << 24)     | PERIODICO     | 250,
    
    // Apaga estado de jugada realizado tras 2 segundos
    ALARM_OFF_REALIZAR_MSG      = (OFF_REALIZAR << 24)  | NO_PERIODICO  | 2000,
    
     // Ordena dormir procesador tras 10s
    ALARM_GO_SLEEP_MSG          = (GO_SLEEP << 24)      | NO_PERIODICO  | 10000,
    
    // Ordena realimenter el watchdog cada 100ms
    ALARM_FEED_WD_MSG           = (FEED_WD << 24)       | PERIODICO     | 100
};

#endif // ALARMAS_H
