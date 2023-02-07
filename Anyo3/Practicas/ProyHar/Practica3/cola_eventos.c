/*
    Proyecto Hardware
    Práctica 3

    Fichero:
        cola_eventos.c

    Autores:
        Dorian Boleslaw Wozniak (817570@unizar.es)
        Pablo Latre Villacampa (778043@unizar.es)

    Descripción: 
        Implementación de un TAD Cola circular que almacena eventos (asíncronos,
        generados por interrupciones) junto al número de veces que ha ocurrido.
        Permite encolar, desencolar y comprobar si está vacía.
*/

#include "cola_eventos.h" 
#include "irq_control.h"
#include "G_IO.h"           // Solo para indicar desbordes

// Constantes
enum {
    TAM_COLA = 32   // Tamaño de la cola
};

// Definición de un elemento de la cola
typedef struct ElementoCola {
    evento      unEvento;       // Evento almacenado
    uint32_t    veces;          // Numero de evento
} elemento ;

static volatile elemento cola[TAM_COLA];    // Cola de eventos
static volatile uint8_t primero = 0;        // Índice al primer elemento
static volatile uint8_t ultimo  = 0;        // Índice al último elemento

/*
    Encola un evento en la cola generado por una IRQ. 
    
    ID_evento: Identificador numérico del evento (ver eventos.h)
    veces:     Número de veces que ha ocurrido el evento
    auxData:   Datos adicionales dados por el evento
    
    Las escrituras se realizan en exclusión mútua, deshabilitando interrupciones
    rápidas. 
    
    Si se encolan TAM_COLA + 1 elementos, la cola queda desbordada y se para el
    programa
*/
void cola_encolar_eventos(uint8_t ID_evento, uint32_t veces, uint32_t auxData) {
    // Inicio sección crítica
    uint8_t fiq = read_FIQ_bit() ; if (!fiq) disable_fiq();
    
    // Si desborda se detiene.
    if ((ultimo + 1) % TAM_COLA == primero) {
        IO_marcar_overflow();
        while (1); 
    }
    
    // Añade a la cola
    cola[ultimo].unEvento.ID_evento = ID_evento;
    cola[ultimo].unEvento.auxData   = auxData;    
    cola[ultimo].veces              = veces;

    // Avanza índice
    ultimo = (ultimo + 1) % TAM_COLA;
    
    // Fin sección crítica
    if (!fiq) enable_fiq();
}

/*
    Encola un evento en la cola generado por una FIQ. 
    
    ID_evento: Identificador numérico del evento (ver eventos.h)
    veces:     Número de veces que ha ocurrido el evento
    auxData:   Datos adicionales dados por el evento
    
    Las escrituras se realizan en exclusión mútua (FIQ lo hace por defecto).
    
    Si se encolan TAM_COLA + 1 elementos, la cola queda desbordada y se para el
    programa
*/
void cola_encolar_eventos_fiq(uint8_t ID_evento, uint32_t veces, uint32_t auxData) {
    // Si desborda se detiene.
    if ((ultimo + 1) % TAM_COLA == primero) {
        IO_marcar_overflow();
        while (1); 
    }
    
    // Añade a la cola
    cola[ultimo].unEvento.ID_evento = ID_evento;
    cola[ultimo].unEvento.auxData   = auxData;    
    cola[ultimo].veces              = veces;

    // Avanza índice
    ultimo = (ultimo + 1) % TAM_COLA;
}

/*
    Devuelve si la cola está vacía
*/
uint8_t cola_hay_eventos() { return ultimo != primero; }

/*
    Devuelve el siguiente evento de la cola.

    e:  Dirección donde almacenar el evento obtenido

    La cola de eventos no puede estar vacía, en caso contrario el resultado 
    queda indefinido. Las lecturas se realizan en exclusión mútua, 
    deshabilitando interrupciones. 
*/
void cola_obtener_sig_evento(evento *e) {
    // Inicio sección crítica
    uint8_t irq = read_IRQ_bit(), fiq = read_FIQ_bit();
    if (!irq) disable_irq_fiq(); else if (!fiq) disable_fiq();
    
    // Obtiene el evento
    *e = cola[primero].unEvento;
    
    // Avanza índice
    primero = (primero + 1) % TAM_COLA;
    
    // Final sección crítica
    if (!irq) enable_irq_fiq(); else if (!fiq) enable_fiq();
}
