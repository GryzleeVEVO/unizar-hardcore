#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

/***************************** printsockaddr ******************************/
/*
 * Imprime una estructura sockaddr_in o sockaddr_in6 almacenada en sockaddr_storage
 */
void printsockaddr(struct sockaddr_storage * saddr)
{
    struct sockaddr_in  *saddr_ipv4; // puntero a estructura de dirección IPv4
    // el compilador interpretará lo apuntado como estructura de dirección IPv4
    struct sockaddr_in6 *saddr_ipv6; // puntero a estructura de dirección IPv6
    // el compilador interpretará lo apuntado como estructura de dirección IPv6
    void *addr; // puntero a dirección. Como puede ser tipo IPv4 o IPv6 no queremos que el compilador la interprete de alguna forma particular, por eso void
    char ipstr[INET6_ADDRSTRLEN]; // string para la dirección en formato texto
    int port; // para almacenar el número de puerto al analizar estructura devuelta

    if (saddr == NULL)
    { 
        printf("La dirección está vacía\n");
    }
    else
    {
        printf("\tFamilia de direcciones: ");
        fflush(stdout);
        if (saddr->ss_family == AF_INET6)
        {   //IPv6
            printf("IPv6\n");
            // apuntamos a la estructura con saddr_ipv6 (el cast evita el warning),
            // así podemos acceder al resto de campos a través de este puntero sin más casts
            saddr_ipv6 = (struct sockaddr_in6 *)saddr;
            // apuntamos a donde está realmente la dirección dentro de la estructura
            addr = &(saddr_ipv6->sin6_addr);
            // obtenemos el puerto, pasando del formato de red al formato local
            port = ntohs(saddr_ipv6->sin6_port);
        }
        else if (saddr->ss_family == AF_INET)
        {   //IPv4
            printf("IPv4\n");
            saddr_ipv4 = (struct sockaddr_in *)saddr;
            addr = &(saddr_ipv4 -> sin_addr);
            port = ntohs(saddr_ipv4 -> sin_port);
        }
        else
        {
            fprintf(stderr, "familia desconocida\n");
            exit(1);
        }
        // convierte la dirección ip a string 
        inet_ntop(saddr->ss_family, addr, ipstr, sizeof ipstr);
        printf("\tDirección (interpretada según familia): %s\n", ipstr);
        printf("\tPuerto (formato local): %d\n", port);
    }
}


/******************** obtener_struct_direccion *********************/
/*
 * Función que devuelve una estructura de direcciones rellenada
 * con al menos una dirección que cumpla los parámetros especificados.
 * El último parámetro controla la verbosidad del programa (información detallada por salida estándar)
 */
struct addrinfo*
obtener_struct_direccion(char *dir_servidor, char *servicio, char f_verbose)
{
    struct addrinfo hints,     // variable para especificar la solicitud
                    *servinfo; // puntero para respuesta de getaddrinfo()
    struct addrinfo *direccion; // puntero para recorrer la lista de direcciones de servinfo
    int status;      // finalización correcta o no de la llamada getaddrinfo()
    int numdir = 1;  // contador de estructuras de direcciones en la lista de direcciones de servinfo

    // sobreescribimos con ceros la estructura para borrar cualquier dato que pueda malinterpretarse
    memset(&hints, 0, sizeof hints);

    // genera una estructura de dirección con especificaciones de la solicitud
    if (f_verbose)
    {
        printf("1 - Especificando detalles de la estructura de direcciones a solicitar... \n");
        fflush(stdout);
    }

    hints.ai_family = AF_UNSPEC; // opciones: AF_UNSPEC; IPv4: AF_INET; IPv6: AF_INET6; etc.

    if (f_verbose)
    {
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

    hints.ai_socktype = SOCK_STREAM; // especificar tipo de socket 

    if (f_verbose)
    {
        printf("\tTipo de comunicación: ");
        switch (hints.ai_socktype)
        {
            case SOCK_STREAM: printf("flujo (TCP)\n"); break;
            case SOCK_DGRAM:  printf("datagrama (UDP)\n"); break;
            default:          printf("no convencional (%d)\n", hints.ai_socktype); break;
        }
        fflush(stdout);
    }

    // flags específicos dependiendo de si queremos la dirección como cliente o como servidor
    if (dir_servidor != NULL)
    {
        // si hemos especificado dir_servidor, es que somos el cliente y vamos a conectarnos con dir_servidor
        if (f_verbose) printf("\tNombre/dirección del equipo: %s\n", dir_servidor); 
    }
    else
    {
        // si no hemos especificado, es que vamos a ser el servidor
        if (f_verbose) printf("\tNombre/dirección: equipo local\n"); 
        hints.ai_flags = AI_PASSIVE; // especificar flag para que la IP se rellene con lo necesario para hacer bind
    }
    if (f_verbose) printf("\tServicio/puerto: %s\n", servicio);

    // llamada a getaddrinfo() para obtener la estructura de direcciones solicitada
    // getaddrinfo() pide memoria dinámica al SO, la rellena con la estructura de direcciones,
    // y escribe en servinfo la dirección donde se encuentra dicha estructura.
    // La memoria *dinámica* reservada por una función NO se libera al salir de ella.
    // Para liberar esta memoria, usar freeaddrinfo()
    if (f_verbose)
    {
        printf("2 - Solicitando la estructura de direcciones con getaddrinfo()... ");
        fflush(stdout);
    }
    status = getaddrinfo(dir_servidor, servicio, &hints, &servinfo);
    if (status != 0)
    {
        fprintf(stderr,"Error en la llamada getaddrinfo(): %s\n", gai_strerror(status));
        exit(1);
    } 
    if (f_verbose) printf("hecho\n");

    // imprime la estructura de direcciones devuelta por getaddrinfo()
    if (f_verbose)
    {
        printf("3 - Analizando estructura de direcciones devuelta... \n");
        direccion = servinfo;
        while (direccion != NULL)
        {   // bucle que recorre la lista de direcciones
            printf("    Dirección %d:\n", numdir);
            printsockaddr((struct sockaddr_storage*) direccion->ai_addr);
            // "avanzamos" a la siguiente estructura de direccion
            direccion = direccion->ai_next;
            numdir++;
        }
    }

    // devuelve la estructura de direcciones devuelta por getaddrinfo()
    return servinfo;
}
