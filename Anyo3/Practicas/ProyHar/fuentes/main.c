#include <LPC210x.h>

#include "cola_eventos.h"
#include "cola_msg.h"

int main(void)
{
    // Inicializa periféricos
    
    // Structs para almacenar lo desencolado
    evento e; mensaje m;

    // Bucle del planificador
    while (1) {
        // Mira si hay eventos pendientes
        if (cola_hay_eventos()) {
            cola_obtener_sig_evento(&e);
            
        }
        // Mira si hay mensajes pendientes
        else if (cola_hay_msg()) {
            cola_obtener_sig_msg(&m);

        }
        // Si no hay nada que hacer, entra en idle
        else power_idle();
    }
}