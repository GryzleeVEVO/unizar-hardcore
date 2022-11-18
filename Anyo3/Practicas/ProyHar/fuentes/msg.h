#ifndef MSG_H
#define MSG_H

#include <inttypes.h>

// Tipos de mensajes disponibles
enum {
    SET_ALARMA = 0
};

// Un mensaje contiene un ID y el contenido del mensaje (según tipo)
typedef struct MensajeG {
    uint8_t ID_msg;
    uint32_t mensaje;

} msg ;


#endif // MSG_H
