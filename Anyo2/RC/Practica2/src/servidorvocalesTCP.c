// importación de funciones, constantes, variables, etc.
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>

#include "comun.h"

// definición de constantes
#define EOT 4               // carácter ASCII end of transmision
#define BUFF_SIZE 1000      // tamaño del buffer


// función que crea la conexión y espera conexiones entrantes
static int establecer_servicio(struct addrinfo *servinfo, char f_verbose)
{
    int sock;

    printf("\nSe usará ÚNICAMENTE la primera dirección de la estructura\n");

    // crea un extremo de la comunicación y devuelve un descriptor
    if (f_verbose)
    {
        printf("Creando el socket (socket)... ");
        fflush(stdout);
    }
    sock = socket(servinfo -> ai_family, servinfo -> ai_socktype, servinfo -> ai_protocol);
    if (sock < 0)
    {
        perror("Error en la llamada socket: No se pudo crear el socket");
        /* muestra por pantalla el valor de la cadena suministrada por el programador, dos puntos y un mensaje de error que detalla la causa del error cometido */
        exit(1);
    }
    if (f_verbose) printf("hecho\n");
    
    // asocia el socket con un puerto
    if (f_verbose)
    {
        printf("Asociando socket a puerto (bind)... ");
        fflush(stdout);
    }
    if (bind(sock, servinfo -> ai_addr, servinfo -> ai_addrlen) < 0)
    {
        perror("Error asociando el socket");
        exit(1);
    }
    if (f_verbose) printf("hecho\n");
    
    // espera conexiones en un socket
    if (f_verbose)
    {
        printf("Permitiendo conexiones entrantes (listen)... ");
        fflush(stdout);
    }
    listen(sock, 5); // 5 es el número máximo de conexiones pendientes en algunos sistemas 
    if (f_verbose) printf("hecho\n");
    
    return sock;
}

// función que cuenta vocales
static uint32_t countVowels(char msg[], size_t s)
{
    uint32_t result = 0;
    size_t i;
    for (i = 0; i < s; i++)
    {
        if (msg[i] == 'a' || msg[i] == 'A' ||
            msg[i] == 'e' || msg[i] == 'E' ||
            msg[i] == 'i' || msg[i] == 'I' ||
            msg[i] == 'o' || msg[i] == 'O' ||
            msg[i] == 'u' || msg[i] == 'U') result++;
    }
    return result;
}

/***************************** MAIN ******************************/
int main(int argc, char * argv[])
{
    // declaración de variables propias del programa principal (locales a main)
    char f_verbose = 1; // flag, 1: imprimir información extra
    struct addrinfo * servinfo; // dirección propia (servidor)
    int sock, conn; // descriptores de socket 
    char msg[BUFF_SIZE]; // espacio para almacenar los datos recibidos
    ssize_t readbytes; // numero de bytes recibidos
    uint32_t num, netNum; // contador de vocales en formato local y de red
    struct sockaddr_storage caddr; // dirección del cliente
    socklen_t clen; // longitud de la dirección 

    // verificación del número de parámetros:
    if (argc != 2)
    {
        printf("Número de parámetros incorrecto \n");
        printf("Uso: %s puerto\n", argv[0]);
        exit(1);
    }

    // obtiene estructura de direccion
    servinfo = obtener_struct_direccion(NULL, argv[1], f_verbose);

    // crea un extremo de la comunicación. Devuelve el descriptor del socket
    sock = establecer_servicio(servinfo, f_verbose);

    // cuando ya no se necesite, hay que liberar la memoria dinámica usada para la dirección
    freeaddrinfo(servinfo);
    servinfo = NULL; // como ya no tenemos la memoria, dejamos de apuntarla para evitar acceder a ella por error

    // bucle infinito para atender conexiones una tras otra
    while (1)
    {
        printf("\nEsperando conexión (pulsa <Ctrl+c> para finalizar la ejecución)...\n");

        // acepta la conexión
        clen = sizeof caddr;
        if ((conn = accept(sock, (struct sockaddr *)&caddr, &clen)) < 0)
        {
            perror("Error al aceptar una nueva conexión");
            exit(1);
        }
    
        // imprime la dirección obtenida
        printf("Aceptada conexión con cliente:\n");
        printsockaddr(&caddr);

        // bucle de contar vocales hasta recibir marca de fin
        num = 0;
        do {
            if ((readbytes = recv(conn, msg, BUFF_SIZE,0)) < 0)
            {
                perror("Error de lectura en el socket");
                exit(1);
            }
            printf("Mensaje recibido: "); fflush(stdout);
            write(1, msg, readbytes); // muestra en pantalla (salida estándar 1) el mensaje recibido
            // evitamos usar printf por si lo recibido no es texto o no acaba con \0
            num += countVowels(msg, readbytes);
            printf("Vocales contadas hasta el momento: %d\n",num);
          
        // condición de final: haber recibido 0 bytes o que el último carácter leído sea EOT
        } while ((readbytes > 0) && (msg[readbytes - 1] != EOT)); 

        printf("\nMarca de fin recibida\n");
        printf("Contadas %d vocales\n", num);  // muestra las vocales recibidas
        netNum = htonl(num);  // convierte el entero largo sin signo hostlong desde el orden de bytes del host al de la red
        // envia al cliente el número de vocales recibidas:
        if (send(conn, &netNum,sizeof netNum,0) < 0)
        {
            perror("Error de escritura en el socket");
            exit(1);
        }
        if (f_verbose) printf("Enviado el número de vocales contadas\n");
        
        // cierra la conexión con el cliente
        close(conn);
        if (f_verbose) printf("Cerrada la conexión con el cliente\n");
    }
    exit(0);
}

