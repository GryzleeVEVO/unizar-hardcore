
//*****************************************************************
// File:   Servidor.cpp
// Author: PSCD-Unizar
// Date:   noviembre 2015
// Coms:   Ejemplo de servidor con comunicación síncrona mediante sockets
//         Compilar el fichero "Makefile" asociado, mediante
//         "make".
//*****************************************************************

#include <iostream>
#include "Socket.hpp"

using namespace std;

const int MESSAGE_SIZE = 4001; //mensajes de no más 4000 caracteres

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
int main() {
    string MENS_FIN = "END OF SERVICE";
    // Puerto donde escucha el proceso servidor
    int SERVER_PORT = 2000;

    // Creación del socket con el que se llevará a cabo
    // la comunicación con el servidor.
    Socket chan(SERVER_PORT);

    // Bind
    int socket_fd =chan.Bind();
    if (socket_fd == -1) {
        string mensError(strerror(errno));
        cerr << "Error en el bind: " + mensError + "\n";
        exit(1);
    }

    // Listen
    int max_connections = 1;
    int error_code = chan.Listen(max_connections);
    if (error_code == -1) {
        string mensError(strerror(errno));
        cerr << "Error en el listen: " + mensError + "\n";
        // Cerramos el socket
        chan.Close(socket_fd);
        exit(1);
    }

    // Accept
    int client_fd = chan.Accept();
    if (client_fd == -1) {
        string mensError(strerror(errno));
        cerr << "Error en el accept: " + mensError + "\n";
        // Cerramos el socket
        chan.Close(socket_fd);
        exit(1);
    }

    // Buffer para recibir el mensaje
    int length = 100;
    string buffer;
    int rcv_bytes;   //num de bytes recibidos en un mensaje
    int send_bytes;  //num de bytes enviados en un mensaje

    bool out = false; // Inicialmente no salir del bucle
    while (!out) {
        // Recibimos el mensaje del cliente
        rcv_bytes = chan.Recv(client_fd, buffer, MESSAGE_SIZE);
        if(rcv_bytes == -1) {
            string mensError(strerror(errno));
            cerr << "Error al recibir datos: " + mensError + "\n";
            // Cerramos los sockets
            chan.Close(client_fd);
            chan.Close(socket_fd);
        }

        cout << "Mensaje recibido: '" + buffer + "'\n";

        // Si recibimos "END OF SERVICE" --> Fin de la comunicación
        if (buffer == MENS_FIN) {
            out = true; // Salir del bucle
        } else {
            // Contamos las vocales recibidas en el mensaje anterior
            int num_vocales = cuentaVocales(buffer);

            // Enviamos la respuesta
            string resp = to_string(num_vocales);

            send_bytes = chan.Send(client_fd, resp);
            if(send_bytes == -1) {
                string mensError(strerror(errno));
                cerr << "Error al enviar datos: " + mensError + "\n";
                // Cerramos los sockets
                chan.Close(client_fd);
                chan.Close(socket_fd);
                exit(1);
            }
        }
    }

    // Cerramos el socket del cliente
    error_code = chan.Close(client_fd);
    if (error_code == -1) {
        string mensError(strerror(errno));
        cerr << "Error cerrando el socket del cliente: " + mensError + "\n";
    }

    // Cerramos el socket del servidor
    error_code = chan.Close(socket_fd);
    if (error_code == -1) {
        string mensError(strerror(errno));
        cerr << "Error cerrando el socket del servidor: " + mensError + "\n";
    }
    // Mensaje de despedida
    cout << "Bye bye" << endl;

    return error_code;
}
