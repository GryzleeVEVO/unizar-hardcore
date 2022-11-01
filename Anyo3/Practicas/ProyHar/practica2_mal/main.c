                  
#include <LPC210x.H>                       /* LPC210x definitions */
#include "timer0.h"
#include "Power_management.h"
#include "boton_eint0.h"

// Cambiar wait() de espera activa a usar idles
void wait (void) {
  unsigned int i;

  i = timer0_read_int_count(); 										// Obtiene n�mero de interrupciones hasta el momento
  while ((i + 10) != timer0_read_int_count());    // Espera 50ms (10 interrupciones)
}

int main (void) {
  unsigned int j; // LED
	 
	// Inicializa EINT0 (bot�n)
	eint0_init(); 				
	
	// Implementar en el conecta4 en GPIO.c
	// Configura salidas de LED y las inicializa a 0
	IODIR 		= 0x00FF0000;
	IOCLR 		= 0x00FF0000;

	// bucle para comprobar el funcionamiento del bot�n. El objetivo es comprobar que se lleva bien la cuenta de pulsaciones
	// con s�lo una interrupci�n EXTINT0 por pulsaci�n
	// en este proyecto no va a funcionar porque la interrupci�n se activa por nivel y no se ha a�adido la gesti�n necesaria para ue s�lo interrumpa una vez.
	
	// Cambiar forma de activaci�n de interrupci�n y a�adir gesti�n para que 
	// solo interrumpa una vez
	
	// Espera a que se haya interrumpido 4 veces pulsando el bot�n
	// En caso contrario duerme
	while (eint0_read_count() < 4){ PM_power_down(); };

		
	// Parpadea un led cada 50ms  
	
	// Inicializa timer0
	timer0_init();
	
	while (1)  {                                  
    for (j = 0x010000; j < 0x800000; j <<= 1) { /* Blink LED 0,1,2,3,4,5,6 */
      // Nota la gesti�n del GPIO vosotros la debe�s hacer en GPIO.c no en el main o en el reversi
			IOSET = j;                               /* Turn on LED */
      wait ();                                  /* call wait function */
      IOCLR = j;                               /* Turn off LED */
    }
    for (j = 0x800000; j > 0x010000; j >>=1 ) { /* Blink LED 7,6,5,4,3,2,1 */
      // Nota la gesti�n del GPIO vosotros la debe�s hacer en GPIO.c no en el main o en el reversi
			IOSET = j;                               /* Turn on LED */
      wait ();                                  /* call wait function */
      IOCLR = j;                               /* Turn off LED */
    }
  }
}


//int main (void) {
//  	
//  reversi8();
//	
//}
