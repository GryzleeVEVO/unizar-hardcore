#ifndef TEMPORIZADOR_H
#define TEMPORIZADOR_H

#include <inttypes.h>

// Inicialilza el temporizador
void temporizador_iniciar();

// Inicia un temporizador
void temporizador_empezar();

// Devuelve el tiempo transcurrido desde la última interrupción
uint32_t temporizador_leer();

// Detiene temporizador y devuelve tiempo transcurrido desde su último inicio
uint32_t temporizador_parar();

void temporizador_reloj(uint32_t periodo);

#endif // TEMPORIZADOR_H