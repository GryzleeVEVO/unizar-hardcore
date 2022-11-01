#ifndef COLA_ASYN_H
#define COLA_ASYN_H

#include <inttypes.h>
#include <eventos.h>

// Añade un evento a la cola
void cola_encolar_eventos(uint8_t ID_evento, uint32_t veces, uint32_t auxData);

// Planificador obtiene siguiente evento para tratarlo
uint8_t cola_obtener_sig_evento(evento *e);

#endif // COLA_ASYN_H
