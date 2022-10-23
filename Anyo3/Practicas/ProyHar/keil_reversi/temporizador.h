#ifndef TEMPORIZADOR_H
#define TEMPORIZADOR_H

#include <LPC210X.h>
#include <inttypes.h>

// Definici�n de interrupci�n
void temporizador_RSI (void) __irq;

// Inicialilza el temporizador
void temporizador_iniciar();

// Inicia un temporizador
void temporizador_empezar();

// Devuelve el tiempo transcurrido desde la �ltima interrupci�n
uint32_t temporizador_leer();

// Detiene temporizador y devuelve tiempo transcurrido desde su �ltimo inicio
uint32_t temporizador_parar();

#endif