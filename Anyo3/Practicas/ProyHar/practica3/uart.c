#include "uart.h"

#include <LPC210X.h>
#include <inttypes.h>
#include "cola_eventos.h"

enum {
    PIN_0_0 = (0x3 << 0),
    PIN_0_1 = (0x1 << 0),
    
    PIN_1_0 = (0x3 << 2),
    PIN_1_1 = (0x1 << 2),
    
    LCR_8BIT = 0x03,
    LCR_DLAB = 0x80,
    
    IER_EN_RDA  = 0x1,
    IER_EN_THRE = 0x2,
    
    UART0_BAUDRATE = 97,
    
    IIR_INT_ID = 0xe,
    IIR_RDA = 0x5,
    IIR_THRE = 0x2
};

enum {
    POS_UART0 = 6,
    INT_UART0 = (0x1 << POS_UART0),
    
    VIC_CNTL_ENABLE = 0x20
};


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

void uart0_echo(uint8_t ch) {
    U0THR = ch;
}

void uart0_enviar_array(uint8_t ch) {
    U0THR = ch;
}

void uart0_continuar_envio(uint8_t ch) {
    if (ch != 0) {
        U0THR = ch; 
    }
}

