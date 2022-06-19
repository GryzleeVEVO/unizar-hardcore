// el preprocesador sustituye cada include por contenido del fichero referenciado
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

/* argc indica el número de argumentos que se han usado en el la línea de comandos.
 * argv es un vector de cadenas de caracteres.
 * argv[0] apunta al nombre del programa y así sucesivamente*/
int main(int argc, char * argv[])
{
    char f_verbose = 1;  // flag, 1: imprimir información por pantalla
    struct addrinfo* direccion; // puntero (no inicializado!) a estructura de dirección

    // verificación del número de parámetros:
    if ((argc != 2) && (argc != 3))
    {
        printf("Número de parámetros incorrecto \n");
        printf("Uso: %s [servidor] <puerto/servicio>\n", argv[0]);
        exit(1); // finaliza el programa indicando salida incorrecta (1)
    }

    if (argc == 2)
    {
        // devuelve la estructura de dirección del servicio solicitado asumiendo que vamos a actuar como servidor
        direccion = obtener_struct_direccion(NULL, argv[1], f_verbose);
    }
    else // if (argc == 3)
    {
        // devuelve la estructura de dirección al equipo y servicio solicitado
        direccion = obtener_struct_direccion(argv[1], argv[2], f_verbose);
    }
    
    // cuando ya no se necesite, hay que liberar la memoria dinámica obtenida en getaddrinfo() mediante freeaddrinfo()
    if (f_verbose)
    {
        printf("Devolviendo al sistema la memoria usada por servinfo (ya no se va a usar)... ");
        fflush(stdout);
    }
    freeaddrinfo(direccion);
    if (f_verbose) printf("hecho\n");
    direccion = NULL; // como ya no tenemos la memoria, dejamos de apuntarla para evitar acceder a ella por error

    // finaliza el programa indicando salida correcta (0)
    exit(0);
}

