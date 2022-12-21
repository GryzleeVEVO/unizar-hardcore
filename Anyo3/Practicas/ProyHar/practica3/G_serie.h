#ifndef G_SERIE_H
#define G_SERIE_H

#include <inttypes.h>

void serie_iniciar(void);

void serie_leer(uint32_t data);

void serie_escribir(void);

void serie_pantalla_bienvenida(void);

void serie_mensaje_reinicio(void);

void serie_print(char* linea);

char serie_itoa(uint8_t i);

uint8_t serie_atoi(char i);

#endif // G_SERIE_H

