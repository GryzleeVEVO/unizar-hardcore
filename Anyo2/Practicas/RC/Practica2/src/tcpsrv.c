#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

#define LISTENQ 5
#define MAXLINE 1000

int main(int argc, char **argv)
{
	int	listenfd, connfd;
	struct sockaddr_in	servaddr =  { 0 };
	char buff[MAXLINE];
	int ncons = 0;

    if (argc != 2)
    {
        printf("usage: %s <puerto>\n", argv[0]);
        exit(0);
    }
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(atoi(argv[1]));	/* daytime server */
	bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	listen(listenfd, LISTENQ);
	for ( ; ; )
	{
		connfd = accept(listenfd, (struct sockaddr *) NULL, NULL);
		ncons++;
        snprintf(buff, sizeof(buff), "Número de conexiones aceptadas por este servidor: %d\n", ncons);
        write(connfd, buff, strlen(buff));
		close(connfd);
	}
}
