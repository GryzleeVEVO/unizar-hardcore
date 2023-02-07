/*
    Proyecto Hardware
    Práctica 3

    Fichero:
        uart.c
        
    Autores:
        Dorian Boleslaw Wozniak (817570@unizar.es)
        Pablo Latre Villacampa (778043@unizar.es)

    Descripción: 
        Implementación de una serie de funciones para gestionar el periférico
        UART0.
*/

#include "uart.h"

#include <LPC210X.h>
#include "cola_eventos.h"
#include "cola_buffer.h"

// Constantes para configurar la UART
enum {
    PIN_0_0 = (0x3 << 0),
    PIN_0_1 = (0x1 << 0),
    
    PIN_1_0 = (0x3 << 2),
    PIN_1_1 = (0x1 << 2),
    
    LCR_8BIT = 0x03,        // Caracteres de 8 bits
    LCR_DLAB = 0x80,        // Activar latch de tasa de símbolos
    
    IER_EN_RDA  = 0x1,      // Habilitar iterrupciones por datos disponibles
    IER_EN_THRE = 0x2,      // Habilitar iterrupciones por registro de 
                            // transmisión vacío
    
    UART0_BAUDRATE = 97,    // Tasa de baudios para la frecuencia dada
    
    IIR_INT_ID = 0xe,       // Máscara para identificación de interrupciones
    IIR_RDA = 0x5,          // Interrupcion de recepción
    IIR_THRE = 0x2          // Interrupión de fin de transmisión
};

// Constantes para configurar el VIC
enum {
    POS_UART0 = 6,
    INT_UART0 = (0x1 << POS_UART0),
    
    VIC_CNTL_ENABLE = 0x20
};


/*
    Rutina de servicio para UART0
*/
void uart0_RSI (void) __irq {
    static volatile uint32_t uart0_read_count = 0;
    static volatile uint32_t uart0_write_count = 0;
    
    uint32_t flag = U0IIR & IIR_INT_ID;
    
    // Lectura de UART
    if (flag & IIR_RDA) {
        uart0_read_count++;
        uint32_t data = U0RBR;
        cola_encolar_eventos(UART0_LEER, uart0_read_count, U0RBR);
    }
    // Escritura a UART
    else if (flag & IIR_THRE) {
        uart0_write_count++;
        cola_encolar_eventos(UART0_ESCRIBIR, uart0_write_count, 0);
    }
    
    VICVectAddr = 0;
}

/*
    Inicializa el UART0
*/
void uart0_init() {
    PINSEL0 &= ~PIN_0_0;
    PINSEL0 |= PIN_0_1;
    PINSEL0 &= ~PIN_1_0;
    PINSEL0 |= PIN_1_1;
    
    U0LCR |= LCR_8BIT | LCR_DLAB;
    U0DLL = UART0_BAUDRATE;
    U0LCR &= ~LCR_DLAB;
        
    VICVectAddr0 = (uint32_t) uart0_RSI;
    VICVectCntl0 |= VIC_CNTL_ENABLE | POS_UART0; 
    
    U0IER |= IER_EN_RDA |IER_EN_THRE;
    VICIntEnable |= INT_UART0;
}

/*
    Escribe un caracter inmediatamente en la UART0

    ch: caracter a escribir
*/
void uart0_echo(uint8_t ch) {
    U0THR = ch;
}

/*
    Encola una cadena de caracteres al búfer asociado a la UART0. Si el búfer\
    estaba vacío al añadir, se escribe el primer caracter a transmitir

    ch: Cadena de caracteres. Debe terminar en NULL
*/
void uart0_enviar_array(char* ch) {
    uint8_t estabaVacio = buffer_vacio();
    buffer_anyadir(ch);
    if (estabaVacio) U0THR = buffer_siguiente();
}

/*
    Si el bufer no está vacío, transmite a UART0 el siguiente caracter
*/
void uart0_continuar_envio() {
    if (!buffer_vacio()) {
        U0THR = buffer_siguiente(); 
    }
}

