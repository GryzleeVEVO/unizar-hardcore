//******************************************************************
// File:   ServidorMulticliente.cpp
// Author: PSCD-Unizar
// Date:   Noviembre 2015
// Coms:   Ejemplo de servidor multicliente con comunicación síncrona mediante sockets
//*****************************************************************

#include "Socket.hpp"
#include <iostream>
#include <thread>

using namespace std;

//-------------------------------------------------------------
// Cuenta el número de vocales existentes en un mensaje
// PRE:
// POST: Devuelve el número de vocales existentes en el mensaje 'message'
int cuentaVocales(string message) {
    int count = 0;

    for (int i=0; i < message.length(); i++) {

        switch(message[i]) {
        case 'a':
        case 'A':
        case 'e':
        case 'E':
        case 'i':
        case 'I':
        case 'o':
        case 'O':
        case 'u':
        case 'U':
            count++;
            break;
        }
    }

    return count;
}
//-------------------------------------------------------------
void servCliente(Socket& soc, int client_fd) {
    string MENS_FIN = "END OF SERVICE";
    // Buffer para recibir el mensaje
    int length = 100;
    string buffer;


    bool out = false; // Inicialmente no salir del bucle
    while(!out) {
        // Recibimos el mensaje del cliente
        int rcv_bytes = soc.Recv(client_fd,buffer,length);
        if (rcv_bytes == -1) {
            cerr << "Error al recibir datos: " + string(strerror(errno)) + "\n";
            // Cerramos los sockets
            soc.Close(client_fd);
        }

        cout << "Mensaje recibido: " + buffer + "\n";

        // Si recibimos "END OF SERVICE" --> Fin de la comunicación
        if (buffer == MENS_FIN) {
            out = true; // Salir del bucle
        } else {
            // Contamos las vocales recibidas en el mensaje anterior
            int num_vocales = cuentaVocales(buffer);

            // Enviamos la respuesta
            string s = to_string(num_vocales);

            int send_bytes = soc.Send(client_fd, s);
            if(send_bytes == -1) {
                cerr << "Error al recibir datos: " + string(strerror(errno)) + "\n";
                // Cerramos los sockets
                soc.Close(client_fd);
                exit(1);
            }
        }
    }
    soc.Close(client_fd);
}
//-------------------------------------------------------------
int main() {
    const int N = 4;
    // Puerto donde escucha el proceso servidor
    int SERVER_PORT = 2000; //normalmente será un parámetro de invocación
    thread cliente[N];
    int client_fd[N];

    // Creación del socket con el que se llevará a cabo
    // la comunicación con el servidor.
    Socket chan(SERVER_PORT);

    // Bind
    int socket_fd = chan.Bind();
    if (socket_fd == -1) {
        cerr << "Error en el bind: " + string(strerror(errno)) + "\n";
        exit(1);
    }

    // Listen
    int max_connections = 2*N;
    int error_code = chan.Listen(max_connections);
    if (error_code == -1) {
        cerr << "Error en el listen: " + string(strerror(errno)) + "\n";
        // Cerramos el socket
        chan.Close(socket_fd);
        exit(1);
    }

    for (int i=0; i<N; i++) {
        // Accept
        client_fd[i] = chan.Accept();

        if(client_fd[i] == -1) {
            cerr << "Error en el accept: " + string(strerror(errno)) + "\n";
            // Cerramos el socket
            chan.Close(socket_fd);
            exit(1);
        }

        cliente[i] = thread(&servCliente, ref(chan), client_fd[i]);
        cout << "Nuevo cliente " + to_string(i) + " aceptado" + "\n";
    }

    //¿Qué pasa si algún thread acaba inesperadamente?
    for (int i=0; i<N; i++) {
        cliente[i].join();
    }

    // Cerramos el socket del servidor
    error_code = chan.Close(socket_fd);
    if (error_code == -1) {
        cerr << "Error cerrando el socket del servidor: " + string(strerror(errno)) + " aceptado" + "\n";
    }

    // Despedida
    cout << "Bye bye" << endl;

    return error_code;
}
//-------------------------------------------------------------
