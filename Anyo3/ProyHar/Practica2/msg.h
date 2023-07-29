/*
    Proyecto Hardware
    Práctica 2

    Fichero:
        msg.h

    Autores:
        Dorian Boleslaw Wozniak (817570@unizar.es)
        Pablo Latre Villacampa (778043@unizar.es)

    Descripción: 
        Declaración de un tipo mensaje, los cuales son producidos por los 
        gestores de periféricos. Un mensaje almacena un ID y su contenido
        según el tipo de mensaje. También contiene los ID de mensjae 
        disponibles.
*/

#ifndef MSG_H
#define MSG_H

#include <inttypes.h>

// Tipos de mensajes disponibles
enum {
    SET_ALARMA  = 1,  // Añadir/reprogramar/cancelar alarma
    CHK_BOTON1  = 2,  // Comprobar pulsación boton 1
    CHK_BOTON2  = 3,  // Comprobar pulsación boton 2
    GO_SLEEP    = 4,  // Dormir procesador
    CHK_ENTRADA = 6,  // Comrpueba si entrada valida    
    PARPADEAR   = 8,  // Cambia el estado del latido
    APAGAR_REAL = 9   // Apaga la marca de jugada realizada
}; 

// Tipo mensaje
typedef struct MensajeG {
    uint8_t ID_msg;     // ID del mensaje
    uint32_t mensaje;   // Contenido (depende del tipo)

} msg ;


#endif // MSG_H
