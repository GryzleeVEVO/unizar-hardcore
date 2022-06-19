// importación de funciones, constantes, etc.
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#include "comun.h"

// definición de constantes
#define MAX_BUFF_SIZE 1000 // tamaño máximo del buffer

// función que crea la conexión y se conecta al servidor
static int initsocket(struct addrinfo *servinfo, char f_verbose)
{
    int sock;

    printf("\nSe usará ÚNICAMENTE la primera dirección de la estructura\n");

    // crea un extremo de la comunicación y devuelve un descriptor
    if (f_verbose)
    {
        printf("Creando el socket (socket)... ");
        fflush(stdout);
    }
    sock = socket(servinfo -> ai_family, servinfo->ai_socktype, servinfo ->ai_protocol);
    if (sock < 0)
    {
        perror("Error en la llamada socket: No se pudo crear el socket");
        /* muestra por pantalla el valor de la cadena suministrada por el programador, dos puntos y un mensaje de error que detalla la causa del error cometido */
        exit(1);
    }
    if (f_verbose) printf("hecho\n");

    // inicia una conexión en el socket:
    if (f_verbose)
    {
        printf("Estableciendo la comunicación a través del socket (connect)... ");
        fflush(stdout);
    }
    if (connect(sock, servinfo -> ai_addr, servinfo -> ai_addrlen) < 0)
    {
        perror("Error en la llamada connect: No se pudo conectar con el destino");
        exit(1);
    } 
    if (f_verbose) printf("hecho\n");

    return sock;
}

/***************************** MAIN ******************************/
int main(int argc, char * argv[])
{
    // declaración de variables propias del programa principal (locales a main)
    char f_verbose = 1;  // flag, 1: imprimir información extra
    const char fin = 4; // carácter ASCII end of transmission (EOT) para indicar fin de transmisión
    struct addrinfo * servinfo; // puntero a estructura de dirección destino
    int sock; // descriptor del socket
    char msg[MAX_BUFF_SIZE]; // buffer donde almacenar datos para enviar
    ssize_t len, // tamaño de datos leídos por la entrada estándar (size_t con signo)
            sentbytes; // tamaño de datos enviados (size_t con signo)
    uint32_t num; // variable donde anotar el número de vocales

    // verificación del número de parámetros:
    if (argc != 3)
    {
        printf("Número de parámetros incorrecto \n");
        printf("Uso: %s servidor puerto/servicio\n", argv[0]);
        exit(1); // finaliza el programa indicando salida incorrecta (1)
    }

    // obtiene estructura de direccion
    servinfo = obtener_struct_direccion(argv[1], argv[2], f_verbose);

    // crea un extremo de la comunicación con la primera de las direcciones de servinfo e inicia la conexión con el servidor. Devuelve el descriptor del socket
    sock = initsocket(servinfo, f_verbose);

    // cuando ya no se necesite, hay que liberar la memoria dinámica usada para la dirección
    freeaddrinfo(servinfo);
    servinfo = NULL; // como ya no tenemos la memoria, dejamos de apuntarla para evitar acceder a ella por error

    // bucle que lee texto del teclado y lo envía al servidor
    printf("\nTeclea el texto a enviar y pulsa <Enter>, o termina con <Ctrl+d>\n");
    while ((len = read(0, msg, MAX_BUFF_SIZE)) > 0)
    { 
        // read lee del teclado hasta que se pulsa INTRO, almacena lo leído en msg y devuelve la longitud en bytes de los datos leídos
        if (f_verbose) printf("  Leídos %zd bytes\n", len);

        // envía datos al socket
        if ((sentbytes = send(sock, msg, len, 0)) < 0)
        {
            perror("Error de escritura en el socket");
            exit(1);
        }
        else
        {
            if (f_verbose) printf("  Enviados correctamente %zd bytes \n",sentbytes);
        }
        // en caso de que el socket sea cerrado por el servidor,
        // al llamar a send() se genera una señal SIGPIPE,
        // que como en este código no se captura,
        // hace que finalice el programa SIN mensaje de error
        // Las señales se estudian en la asignatura Sistemas Operativos

        printf("Teclea el texto a enviar y pulsa <Enter>, o termina con <Ctrl+d>\n");
    }

    // se envía una marca de finalización:
    if (send(sock, &fin,  sizeof fin, 0) < 0)
    {
        perror("Error de escritura en el socket");
        exit(1);
    }
    if (f_verbose)
    {
      printf("Enviada correctamente la marca de finalización.\nEsperando respuesta del servidor...");
      fflush(stdout);
    }

    // recibe del servidor el número de vocales recibidas:
    if (recv(sock, &num, sizeof num, 0) < 0)
    {
        perror("Error de lectura en el socket");
        exit(1);
    }
    printf(" hecho\nEl texto enviado contenía en total %d vocales\n", ntohl(num));
    // convierte el entero largo sin signo desde el orden de bytes de la red al del host

    close(sock); // cierra la conexión del socket:
    if (f_verbose) printf("Socket cerrado\n");

    exit(0); // finaliza el programa indicando salida correcta (0)
}
