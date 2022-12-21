#ifndef UART_H
#define UART_H

#include <inttypes.h>

void uart0_init(void);

void uart0_echo(uint8_t ch);

void uart0_enviar_array(char *ch);

void uart0_continuar_envio(void);

#endif // UART_H

