/****************************************************************************/
/* Plantilla para implementación de funciones del cliente (rcftpclient)     */
/* $Revision$ */
/* Aunque se permite la modificación de cualquier parte del código, se */
/* recomienda modificar solamente este fichero y su fichero de cabeceras asociado. */
/****************************************************************************/

/**************************************************************************/
/* INCLUDES                                                               */
/**************************************************************************/
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/uio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>
#include <signal.h>
#include "rcftp.h" // Protocolo RCFTP
#include "rcftpclient.h" // Funciones ya implementadas
#include "multialarm.h" // Gestión de timeouts
#include "vemision.h" // Gestión de ventana de emisión
#include "misfunciones.h"

/**************************************************************************/
/* VARIABLES GLOBALES                                                     */
/**************************************************************************/
// elegir 1 o 2 autores y sustituir "Apellidos, Nombre" manteniendo el formato
char* autores="Autor: Wozniak, Dorian Boleslaw"; // un solo autor
//char* autores="Autor: Apellidos, Nombre\nAutor: Apellidos, Nombre" // dos autores

// variable para indicar si mostrar información extra durante la ejecución
// como la mayoría de las funciones necesitaran consultarla, la definimos global
extern char verb;


// variable externa que muestra el número de timeouts vencidos
// Uso: Comparar con otra variable inicializada a 0; si son distintas, tratar un timeout e incrementar en uno la otra variable
extern volatile const int timeouts_vencidos;


/**************************************************************************/
/* Obtiene la estructura de direcciones del servidor */
/**************************************************************************/
struct addrinfo* obtener_struct_direccion(char *dir_servidor, char *servicio, char verb){

	// hints es una variable que especifica la solicitud
	// servinfo es un puntero para la respuesta de getaddrinfo()
	// direccion es un puntero para recorrer la lista de direcciones de servinfo

	struct addrinfo hints, *servinfo, *direccion;

	// status indica la finalización correcta/incorrecta de getaddrinfo()
	// numdir cuenta estructuras de direcciones en la lista de direcciones de servinfo

	int status, numdir = 1;

	memset(&hints, 0, sizeof hints); // Sobreescribe estructura con ceros

	/* Genera una estructura de dirección con especificaciones de la solicitud */
	
	// Tipo de protocolo de red

	if(verb){
        printf("1 - Especificando detalles de la estructura de direcciones a solicitar... \n");
        fflush(stdout);
	}

	hints.ai_family = AF_UNSPEC; // AF_UNSPEC = IPv4 o IPv6

	if(verb){
        printf("\tFamilia de direcciones/protocolos: ");
        switch (hints.ai_family)
        {
            case AF_UNSPEC: printf("IPv4 e IPv6\n"); break;
            case AF_INET:   printf("IPv4)\n"); break;
            case AF_INET6:  printf("IPv6)\n"); break;
            default:        printf("No IP (%d)\n", hints.ai_family); break;
        }
        fflush(stdout);
	}

	// Tipo de socket

	hints.ai_socktype = SOCK_DGRAM; // SOCK_DGRAM = UDP

	if(verb){
		printf("\tTipo de comunicación: ");
        switch (hints.ai_socktype)
        {
            case SOCK_STREAM: printf("flujo (TCP)\n"); break;
            case SOCK_DGRAM:  printf("datagrama (UDP)\n"); break;
            default:          printf("no convencional (%d)\n", hints.ai_socktype); break;
        }
        fflush(stdout);
	}

	// Dirección destino

	if(dir_servidor != NULL){
		if(verb){
			printf("\tNombre/dirección del equipo: %s\n", dir_servidor); 
		}
	}

	else{	// Dirección no especificada
		if(verb){
			printf("\tNombre/dirección: equipo local\n"); 
		}

		hints.ai_flags = AI_PASSIVE; // AI_PASSIVE = Dirección local (este equipo como servidor)
	}

	if(verb){
		printf("\tServicio/puerto: %s\n", servicio);
	}

	/* Obtiene la estructura de direcciones solicitada*/

	if(verb){
		printf("2 - Solicitando la estructura de direcciones con getaddrinfo()... ");
        fflush(stdout);
	}

	// Llamada a getaddrinfo()

	status = getaddrinfo(dir_servidor, servicio, &hints, &servinfo);

	if(status != 0){
		fprintf(stderr,"Error en la llamada getaddrinfo(): %s\n", gai_strerror(status));
        exit(1);
	}

	if(verb){
		printf("hecho\n");
	}

	if(verb){
		printf("3 - Analizando estructura de direcciones devuelta... \n");
        direccion = servinfo;
		while(direccion != NULL){ // Recorre lista de direcciones 
			printf("    Dirección %d:\n", numdir);
			printsockaddr((struct sockaddr_storage*) direccion->ai_addr);
			direccion = direccion->ai_next;
            numdir++;
		}
	}
	
	return servinfo;
}

/**************************************************************************/
/* Imprime una direccion */
/**************************************************************************/
void printsockaddr(struct sockaddr_storage * saddr) {

	// Punteros a estructuras de dirección IPv4 e IPv6, respectivamente

	struct sockaddr_in *saddr_ipv4; 
	struct sockaddr_in6 *saddr_ipv6;

	// Puntero a dirección (void para no interpretar dirección)

	void *addr;

	// Cadena para dirección en formato texto

	char ipstr[INET6_ADDRSTRLEN]; 

	// port almacena el número de puerto al analizar la estructura devuelta

	int port;

	if(saddr == NULL){
        printf("La dirección está vacía\n");
	}

	else{
        printf("\tFamilia de direcciones: ");
        fflush(stdout);

		if(saddr -> ss_family == AF_INET6){ // Dirección IPv6
            printf("IPv6\n");
            saddr_ipv6 = (struct sockaddr_in6 *)saddr;
			addr = &(saddr_ipv6 -> sin6_addr);
			port = ntohs(saddr_ipv6 -> sin6_port);			
		}

		else if(saddr -> ss_family == AF_INET){ // Dirección Ipv4
			printf("IPv4\n");
            saddr_ipv4 = (struct sockaddr_in *)saddr;
            addr = &(saddr_ipv4 -> sin_addr);
            port = ntohs(saddr_ipv4 -> sin_port);
		}

		else{
			fprintf(stderr, "familia desconocida\n");
            exit(1);
		}

		// Conversión de la dirección a cadena de caracteres

		inet_ntop(saddr -> ss_family, addr, ipstr, sizeof ipstr);
        printf("\tDirección (interpretada según familia): %s\n", ipstr);
        printf("\tPuerto (formato local): %d\n", port);
	}
}

/**************************************************************************/
/* Configura el socket, devuelve el socket y servinfo */
/**************************************************************************/
int initsocket(struct addrinfo *servinfo, char f_verbose){
	int sock;

    printf("\nSe usará ÚNICAMENTE la primera dirección de la estructura\n");

	if(verb){
		printf("Creando el socket (socket)... ");
        fflush(stdout);
	}

	// Crea un extremo de la comunicación y devuelve un descriptor

	sock = socket(servinfo -> ai_family, servinfo -> ai_socktype, servinfo -> ai_protocol);

	if(sock < 0){
		perror("Error en la llamada socket: No se pudo crear el socket");
		exit(1);
	}

	if(verb){
		printf("hecho\n");
	}

	return sock;
}

/******************************************************************************/
/* Contrucción de mensajes RCFTP											  */
/******************************************************************************/
void construirMensajeRCFTP(struct rcftp_msg* mensaje, int len, long int numSec, char datos[RCFTP_BUFLEN]){
	
	/* Introducir versión (siempre RCFTP_VERSION_1) */
	mensaje -> version = RCFTP_VERSION_1;

	/* Introducir flags adecuadas (F_FIN si es último mensaje, F_NOFLAGS en caso contrario) */
	if(len < RCFTP_BUFLEN){
		mensaje -> flags = F_FIN;
	}

	else{
		mensaje -> flags = F_NOFLAGS;
	}

	/* Introducir número de secuencia (0 al no esperar respuesta) */
	mensaje -> next = htons(0);
	
	/* Introducir siguiente num. secuencia esperado a recibir */
	mensaje -> numseq = htonl(numSec);

	/* Introducir largo de los datos enviados */
	mensaje -> len = htons(len);

	/* Introducir datos a enviar */

	int i;
	for(i = 0; i < RCFTP_BUFLEN; i++){
		mensaje -> buffer[i] = datos[i]; 
	}

	/* Introducir suma de comprobación */
	mensaje -> sum = 0;
	mensaje -> sum = xsum((char*) mensaje, sizeof(*mensaje));
}

void reconstruirMensajeRCFTP(struct rcftp_msg* mensaje, int len, long int numSec, char datos[RCFTP_BUFLEN]){
	
	/* Introducir versión (siempre RCFTP_VERSION_1) */
	mensaje -> version = RCFTP_VERSION_1;

	/* Introducir flags adecuadas (F_FIN si es último mensaje, F_NOFLAGS en caso contrario) */

	mensaje -> flags = F_NOFLAGS;

	/* Introducir número de secuencia (0 al no esperar respuesta) */
	mensaje -> next = htons(0);
	
	/* Introducir siguiente num. secuencia esperado a recibir */
	mensaje -> numseq = htonl(numSec);

	/* Introducir largo de los datos enviados */
	mensaje -> len = htons(len);

	/* Introducir datos a enviar */

	int i;
	for(i = 0; i < RCFTP_BUFLEN; i++){
		mensaje -> buffer[i] = datos[i]; 
	}

	/* Introducir suma de comprobación */
	mensaje -> sum = 0;
	mensaje -> sum = xsum((char*) mensaje, sizeof(*mensaje));
}


/******************************************************************************/
/* Verificación de mensajes recibidos */
/******************************************************************************/

int esMensajeValido(struct rcftp_msg* respuesta){
	int valido = 1;

	/* Comprueba la versión del mensaje */ 
	if((respuesta -> version) != RCFTP_VERSION_1){
		valido = 0;

		if(verb){
			fprintf(stderr, "Error: El mensaje no es válido: recibido version %d\n", respuesta -> version);
		}
	}

	/* Verifica su suma de comprobación */
	if(issumvalid(respuesta, sizeof *respuesta) == 0){
		valido = 0;

		if(verb){
			fprintf(stderr, "Error: El mensaje no es válido: checksum incorrecto\n");
		}
	}

	return valido;
}

/******************************************************************************/

int esLaRespuestaEsperada(struct rcftp_msg* respuesta, struct rcftp_msg* mensaje){
	int esperado = 1;
	
	/* Comprueba que el campo next de la respuesta sea igual a numseq + len del mensaje */

	if(ntohl(respuesta -> next) != ntohl(mensaje -> numseq) + ntohs(mensaje -> len)){
		esperado = 0;

		if(verb){
			fprintf(stderr, "Error: Respuesta inesperada: secuencia esperada incorrecta: es %d pero debería ser %d\n", ntohl(respuesta -> next), ntohl(mensaje -> numseq) + ntohs(mensaje -> len));
		}
	}

	/* Comprueba que no haya flags de ocupado/abortar */

	if(respuesta -> flags == F_BUSY || respuesta -> flags == F_ABORT){
		esperado = 0;

		if(verb){
			fprintf(stderr, "Error: Respuesta inesperada: Recibido flag %d\n", respuesta -> flags);
		}
	}

	/* Comprueba que si mensaje -> flags tiene marca de fin, respuesta -> flags tambien */
	
	if(mensaje -> flags == F_FIN){
		if(respuesta -> flags != F_FIN){
			esperado = 0;

			if(verb){
				fprintf(stderr, "Error: Respuesta inesperada: Recibido flag %d pero se esperaba flag %d\n", respuesta -> flags, mensaje -> flags);
			}
		}
	}
	
	return esperado;
}

/******************************************************************************/

int esLaRespuestaEsperadaGBN(struct rcftp_msg* respuesta, int loNumSec, int hiNumSec){
	int esperado = 1;

	/* Comprueba que next - 1 se encuentre en la ventana de emisión */

	if(!(ntohl(respuesta -> next) - 1 >= loNumSec && ntohl(respuesta -> next) - 1 <= hiNumSec)){
		esperado = 0;

		if(verb){
			fprintf(stderr, "Error: Respuesta inesperada: secuencia esperada incorrecta: es %d pero debería estar entre (%d,%d)\n", ntohl(respuesta -> next), loNumSec, hiNumSec);
		}
	}

	/* Comprueba que no haya flags de ocupado/abortar */

	if(respuesta -> flags == F_BUSY || respuesta -> flags == F_ABORT){
		esperado = 0;

		if(verb){
			fprintf(stderr, "Error: Respuesta inesperada: Recibido flag %d\n", respuesta -> flags);
		}
	}

	return esperado;
}


/**************************************************************************/
/*  algoritmo 1 (basico)  */
/**************************************************************************/
void alg_basico(int socket, struct addrinfo *servinfo) {
	printf("Comunicación con algoritmo básico\n");

	struct rcftp_msg mensaje, respuesta; 					/* structs de mensaje y respuesta */
	int ultimoMensaje = 0, ultimoMensajeConfirmado = 0; 	/* Variables de test de EOF*/
	char datos[RCFTP_BUFLEN];								/* Datos a enviar */
	long int numSec = 0;

	/* Lee de entrada estandar */
	int len = readtobuffer(datos, RCFTP_BUFLEN);

	/* Prueba si solo es un mensaje */
	if(len < RCFTP_BUFLEN){
		ultimoMensaje = 1;
	}

	if(verb){
		printf("\n");
	}

	construirMensajeRCFTP(&mensaje, len, numSec, datos);	/* Construye mensaje */

	while(!ultimoMensajeConfirmado){

		/* Envia datos */
		sendto(socket, &mensaje, sizeof mensaje, 0, servinfo -> ai_addr, servinfo -> ai_addrlen);

		/* Espera una respuesta de confirmación */
		recvfrom(socket, &respuesta, sizeof respuesta, 0, servinfo -> ai_addr, &(servinfo -> ai_addrlen));

		/* Si es el mensaje incorrecto, reenvia*/
		if(esMensajeValido(&respuesta) == 1 && esLaRespuestaEsperada(&respuesta, &mensaje) == 1){
			// Obtiene el número de secuencia para el siguiente mensaje
			numSec = numSec + len; 							
			
			/* Confirma si el último mensaje es correcto */
			if(ultimoMensaje){								
				ultimoMensajeConfirmado = 1;
			}

			/* Obtiene siguiente mensaje */
			else{											
				len = readtobuffer(datos, RCFTP_BUFLEN);

				if(len < RCFTP_BUFLEN){
					ultimoMensaje = 1;
				}

				construirMensajeRCFTP(&mensaje, len, numSec, datos);
			}
		}

		else{
			if(verb){
				fprintf(stderr, "Reenviando mensaje...\n");

			}
		}
		
		if(verb){
			printf("\n");
		}
	}
}

/**************************************************************************/
/*  algoritmo 2 (stop & wait)  */
/**************************************************************************/
void alg_stopwait(int socket, struct addrinfo *servinfo) {

	printf("Comunicación con algoritmo stop&wait\n");

	/* Configura el comportamiento de las alarmas */

	signal(SIGALRM, handle_sigalrm);

	/* Configura socket para que no sea bloqueante */
	
	int sockflags = fcntl(socket, F_GETFL, 0);
	fcntl(socket, F_SETFL, sockflags | O_NONBLOCK);

	struct rcftp_msg mensaje, respuesta; 					/* structs de mensaje y respuesta */
	int ultimoMensaje = 0, ultimoMensajeConfirmado = 0; 	/* Variables de test de EOF*/
	char datos[RCFTP_BUFLEN];								/* Datos a enviar*/
	long int numSec = 0;
	int wait = 0, numDatosRecibidos = 0, timeouts_procesados = 0; /* Vars alg s&w*/	

	/* Lee de entrada estandar */
	int len = readtobuffer(datos, RCFTP_BUFLEN); 			

	/* Prueba si solo es un mensaje */
	if(len < RCFTP_BUFLEN){									
		ultimoMensaje = 1;
	}
	
	/* Construye mensaje */
	construirMensajeRCFTP(&mensaje, len, numSec, datos);	

	while(ultimoMensajeConfirmado == 0){
		/* Envia datos */
		sendto(socket, &mensaje, sizeof mensaje, 0, servinfo -> ai_addr, servinfo -> ai_addrlen);

		if(verb){
			printf("Enviado\n");
		}
		
		/* Inicia timeout */
		addtimeout();

		/* Espera una respuesta de confirmación */
		wait = 1;

		while(wait){
			/* Prueba si ha recibido respuesta (no bloqueante, prueba múltiples veces) */
			numDatosRecibidos = recvfrom(socket, &respuesta, sizeof respuesta, 0, servinfo -> ai_addr, &(servinfo -> ai_addrlen));

			/* Ha recibido respuesta */
			if(numDatosRecibidos > 0){	
				canceltimeout();
				wait = 0;
			}

			/* El timeout ha vencido */
		 	if(timeouts_procesados != timeouts_vencidos){ 
				wait = 0;
				timeouts_procesados++;
			}
		}

		/* Si es el mensaje incorrecto, reenvia*/
		if(numDatosRecibidos > 0 && esMensajeValido(&respuesta) == 1 && esLaRespuestaEsperada(&respuesta, &mensaje) == 1){
			/* Obtiene el número de secuencia para el siguiente mensaje */
			numSec = numSec + len; 							

			/* Confirma si el último mensaje es correcto */
			if(ultimoMensaje == 1){							
				ultimoMensajeConfirmado = 1;
			}
			
			/* Obtiene siguiente mensaje */
			else{											
				len = readtobuffer(datos, RCFTP_BUFLEN);

				if(len < RCFTP_BUFLEN){
					ultimoMensaje = 1;
				}

				construirMensajeRCFTP(&mensaje, len, numSec, datos);
			}

			if(verb){
			printf("Recibida respuesta del servidor\n");
			}
		}

		else{
			if(verb){
				fprintf(stderr, "Reenviando mensaje...\n");
			}
		}

		if(verb){
			printf("\n");
		}
	}
}

/**************************************************************************/
/*  algoritmo 3 (ventana deslizante)  */
/**************************************************************************/
void alg_ventana(int socket, struct addrinfo *servinfo,int window) {

	printf("Comunicación con algoritmo go-back-n\n");

	/* Configura el comportamiento de las alarmas */

	signal(SIGALRM, handle_sigalrm);

	/* Configura el socket para que sea no bloqueante */

	int sockflags = fcntl(socket, F_GETFL, 0);
	fcntl(socket, F_SETFL, sockflags | O_NONBLOCK);

	/* Ajusta el tamaño de la ventana */

	setwindowsize(window);

	/* Variables*/

	struct rcftp_msg mensaje, respuesta;
	int lenLastRead, lenResend;
	int numDatosRecibidos;
	int loNumSec = 0, hiNumSec = 0, numSecResend;
	int timeouts_procesados = 0;
	int ultimoMensajeConfirmado = 0, ultimoMensaje = 0;
	char datos[RCFTP_BUFLEN];

	while(ultimoMensajeConfirmado == 0){

		/* BLOQUE DE ENVÍO */
		if(getfreespace() >= RCFTP_BUFLEN && ultimoMensaje == 0){
			/* Obtiene datos del buffer */
			lenLastRead = readtobuffer(datos, RCFTP_BUFLEN);

			/* Se detiene el bloque si fin de fichero */
			if(lenLastRead < RCFTP_BUFLEN){
				ultimoMensaje = 1;
			}

			/* Construye y envía el mensaje */
			construirMensajeRCFTP(&mensaje, lenLastRead, hiNumSec, datos);
			sendto(socket, &mensaje, sizeof mensaje, 0, servinfo -> ai_addr, servinfo -> ai_addrlen);

			/* Añade datos a ventana de emisión */
			addtimeout();
			addsentdatatowindow(datos, lenLastRead);

			/* Actualiza cota superior de ventana ocupada */
			hiNumSec = hiNumSec + lenLastRead;
		}

		/* BLOQUE DE RECEPCIÓN */

		/* Obtiene una respuesta */
		numDatosRecibidos = recvfrom(socket, &respuesta, sizeof respuesta, 0, servinfo -> ai_addr, &(servinfo -> ai_addrlen));

		/* Respuesta válida */
		if(numDatosRecibidos > 0 && esMensajeValido(&respuesta) && esLaRespuestaEsperadaGBN(&respuesta, loNumSec, hiNumSec)){
			canceltimeout();
			freewindow(loNumSec = ntohl(respuesta.next));

			if(ultimoMensaje && respuesta.flags == F_FIN){
				ultimoMensajeConfirmado = 1;
			}
		}

		/* BLOQUE DE PROCESADO DE TIMEOUTS */
		if(timeouts_procesados != timeouts_vencidos){
			lenResend = RCFTP_BUFLEN;
			numSecResend = getdatatoresend(datos, &lenResend);

			reconstruirMensajeRCFTP(&mensaje, lenResend, numSecResend, datos);

			sendto(socket, &mensaje, sizeof mensaje, 0, servinfo -> ai_addr, servinfo -> ai_addrlen);
			addtimeout();

			if(verb){
				fprintf(stderr, "Reenviando mensaje...\n");
			}
		}
	}
}


