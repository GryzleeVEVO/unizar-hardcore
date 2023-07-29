#ifndef COMUN_H
#define COMUN_H

void printsockaddr(struct sockaddr_storage * saddr);

struct addrinfo* obtener_struct_direccion(char *nodo, char *servicio, char f_verbose);


#endif /* COMUN_H */
