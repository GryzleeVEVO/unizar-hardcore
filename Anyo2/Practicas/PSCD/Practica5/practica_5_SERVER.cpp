#include <iostream>
#include <thread>
#include <chrono>

#include "ControlCabinas.hpp"
#include "Socket.hpp"

#ifdef LOGGING_MODE
    #include <Logger.hpp>
    Logger _logger("_log_SERVER.log", 1024);
    const string nada("");
    #define ADD_EVENT(e) {_logger.addMessage(nada+e);} //generar evento
#else
    #define ADD_EVENT(e)   // nada
#endif

using namespace std;

bool procesarParam(int argc, char* argv[], int& port1, int& port2){
    bool error = false;

    // Caso de error: Número de argumentos incorrecto
    if(!error && argc != 3){
        cerr << "[ERROR] Número de parámetros incorrecto: hay " 
            + to_string(argc) + " cuando deberían ser 2\n";

        error = true;
    }

    else{
        port1 = atoi(argv[1]);
        port2 = atoi(argv[2]);
    }
    
    // Caso de error: Valor de los puertos inválido
    if(!error && (port1 > 65535 || port2 > 65535)){
        cerr << "[ERROR] Puertos inválidos. Puertos obtenidos son " + 
            to_string(port1) + " y " + to_string(port2) 
            + " y uno de ellos supera el valor máximo\n";
        
        error = true;
    }

    if(error){
        cerr << "[INFO] Sintáxis: ./practica_5_SERVER puerto1 puerto2\n";
    }

    return !error;
}

void servirUsuario(Socket& ch, ControlCabinas& mCtlCab, int client_fd){
    bool fin = false;
    int send_bytes, recv_bytes, len = 100;
    int i = -1, c;
    string buffer, s;
    chrono::steady_clock::time_point t1, t2;    
    int t = 0;

    
    while(!fin){
        recv_bytes = ch.Recv(client_fd, buffer, len);

        if(recv_bytes == -1){
            cerr << "[ERROR] [servirUsuario] [" + to_string(i) 
                + "] Al recibir: " + string(strerror(errno)) + "\n";
            ch.Close(client_fd);
            ::exit(1);
        }

        else if(buffer == "FIN"){
            ADD_EVENT("USER_" + to_string(i) + "_MSG_RECV: " + buffer);
            buffer = "TIEMPO_TOTAL," + to_string(t);

            ch.Send(client_fd, buffer);

            ADD_EVENT("USER_" + to_string(i) + "_MSG_SEND: " + buffer);

            fin = true;
        }

        else if(buffer.rfind("RESERVAR_CABINA,") == 0){
            s = buffer.substr(16);
            i = stoi(s);
            ADD_EVENT("USER_" + to_string(i) + "_MSG_RECV: " + buffer);
            mCtlCab.entraUsuario(c);
            t1 = chrono::steady_clock::now();
            buffer = "CONCEDIDA," + to_string(c);

            send_bytes = ch.Send(client_fd, buffer);

            if(send_bytes == -1){
                cerr << "[ERROR] [servirUsuario] [" + to_string(i) 
                    + "] Al enviar: " + string(strerror(errno)) + "\n";
                ch.Close(client_fd);
                ::exit(1);
            }

            else{
                ADD_EVENT("USER_" + to_string(i) + "_MSG_SEND: " + buffer);

                recv_bytes = ch.Recv(client_fd, buffer, len);

                if(recv_bytes == -1){
                    cerr << "[ERROR] [servirUsuario] [" + to_string(i) 
                        + "] Al recibir: " + string(strerror(errno)) + "\n";
                    ch.Close(client_fd);
                    ::exit(1);
                }

                else if(buffer.rfind("LIBERAR_CABINA,", 0) == 0){
                    ADD_EVENT("USER_" + to_string(i) + "_MSG_RECV: " + buffer);
                    s = buffer.substr(15);
                    c = stoi(s);

                    mCtlCab.saleUsuario(c);
                    t2 = chrono::steady_clock::now();
                    
                    t = t + chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
                }
            }
        }
    }
    
    ch.Close(client_fd);
    cout << "[INFO] [servirUsuario] [" + to_string(i) 
        + "] Conexión con cliente finalizado\n";
}

void servirLimpiador(Socket& ch, ControlCabinas& mCtlCab, int client_fd){
    bool fin = false;
    int send_bytes, recv_bytes, len = 100;
    int c;
    string buffer, s;
    chrono::steady_clock::time_point t1, t2;    
    int t = 0;

    
    while(!fin){
        recv_bytes = ch.Recv(client_fd, buffer, len);

        if(recv_bytes == -1){
            cerr << "[ERROR] [servirLimpiador] Al recibir: " 
                + string(strerror(errno)) + "\n";
            ch.Close(client_fd);
            ::exit(1);
        }

        else if(buffer == "FIN"){
            ADD_EVENT("CLEANER_MSG_RECV: " + buffer);
            buffer = "TIEMPO_TOTAL," + to_string(t);

            ch.Send(client_fd, buffer);

            ADD_EVENT("CLEANER_MSG_SENT: " + buffer);
            fin = true;
        }

        else if(buffer == "LIMPIEZA_ENTRAR"){
            ADD_EVENT("CLEANER_MSG_RECV: " + buffer);
            mCtlCab.entraLimpieza();
            t1 = chrono::steady_clock::now();
            buffer = "OK_LIMPIEZA";

            send_bytes = ch.Send(client_fd, buffer);

            if(send_bytes == -1){
                cerr << "[ERROR] [servirLimpiador] Al enviar: " 
                    + string(strerror(errno)) + "\n";
                ch.Close(client_fd);
                ::exit(1);
            }

            else{
                ADD_EVENT("CLEANER_MSG_RECV: " + buffer);

                recv_bytes = ch.Recv(client_fd, buffer, len);

                if(recv_bytes == -1){
                    cerr << "[ERROR] [servirLimpiador] Al recibir: " 
                        + string(strerror(errno)) + "\n";
                    ch.Close(client_fd);
                    ::exit(1);
                }

                else if(buffer == "LIMPIEZA_SALIR"){
                    ADD_EVENT("CLEANER_MSG_RECV: " + buffer);
                    mCtlCab.saleLimpieza();
                    t2 = chrono::steady_clock::now();
                    
                    t = t + chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
                }
            }
        }
    }
    
    ch.Close(client_fd);
    cout << "[INFO] [servirLimpiador] Conexión con cliente finalizado\n";
}

void conectarUsuarios(Socket& ch, ControlCabinas& mCtlCab){
    thread usuarios[N_USER];
    int client_fd[N_USER];

    cout << "[INFO] [conectarUsuarios] Servidor iniciado\n";

    int socket_fd = ch.Bind();

    if(socket_fd == -1){
        cerr << "[ERROR] [conectarUsuarios] Al asociar: " + string(strerror(errno)) + "\n";
        ::exit(1);
    }

    int max_connections = 2 * N_USER;

    if(ch.Listen(max_connections) == -1){
        cerr << "[ERROR] [conectarUsuarios] Durante la escucha: " + string(strerror(errno)) + "\n";
        ch.Close(socket_fd);
        ::exit(1);
    }

    for(int i = 0; i < N_USER; i++){
        client_fd[i] = ch.Accept();

        if(client_fd[i] == -1){
            cerr << "[ERROR] [conectarUsuarios] Al aceptar: " + string(strerror(errno)) + "\n";
            ch.Close(socket_fd);
            ::exit(1);
        }

        usuarios[i] = thread(&servirUsuario, ref(ch), ref(mCtlCab), client_fd[i]);
        cout << "[INFO] [conectarUsuarios] Nuevo cliente: " + to_string(i) + "\n";
    }

    for(int i = 0; i < N_USER; i++){
        usuarios[i].join();
    }

    if(ch.Close(socket_fd) == -1){
        cerr << "[ERROR] [conectarUsuarios] Al cerrar el socket del servidor: " 
            + string(strerror(errno)) + "\n";
    }

    cout << "[INFO] [conectarUsuarios] Servidor cerrado\n";
}

void conectarLimpiador(Socket& ch, ControlCabinas& mCtlCab){
    cout << "[INFO] [conectarLimpiador] Servidor iniciado\n";

    int socket_fd = ch.Bind();

    if(socket_fd == -1){
        cerr << "[ERROR] [conectarLimpiador] Durante la escucha: " + string(strerror(errno)) + "\n";
        ch.Close(socket_fd);
        ::exit(1);
    }
    
    if(ch.Listen(1) == -1){
        cerr << "[ERROR] [conectarLimpiador] Durante la escucha: " + string(strerror(errno)) + "\n";
        ch.Close(socket_fd);
        ::exit(1);
    }

    int client_fd = ch.Accept();

    if(client_fd == -1){
        cerr << "[ERROR] [conectarLimpiador] Al aceptar: " + string(strerror(errno)) + "\n";
        ch.Close(socket_fd);
        ::exit(1);
    }

    servirLimpiador(ch, mCtlCab, client_fd);

    if(ch.Close(socket_fd) == -1){
        cerr << "[ERROR] [conectarLimpiador] Al cerrar el socket del servidor: " 
            + string(strerror(errno)) + "\n";
    }

    cout << "[INFO] [conectarLimpiador] Servidor cerrado\n";
}

int main(int argc, char* argv[]){
    int port1, port2;

    if(procesarParam(argc, argv, port1, port2)){
        cout << "[INFO] Puertos utilizados: " << to_string(port1) 
                + " " + to_string(port2) + "\n";

        Socket ch_usuario(port1), ch_limpiador(port2);
        thread th[2];
        ControlCabinas mCtlCab;

        th[0] = thread(&conectarUsuarios, ref(ch_usuario), ref(mCtlCab));
        th[1] = thread(&conectarLimpiador, ref(ch_limpiador), ref(mCtlCab));

        th[0].join();
        th[1].join();

        return 0;
    }

    else{
        return 1;
    }
}