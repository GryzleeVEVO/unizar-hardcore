#include <iostream>
#include <chrono>
#include <thread>

#include "Socket.hpp"
#include "ControlCabinas.hpp" // Solo constantes

#ifdef LOGGING_MODE
    #include <Logger.hpp>
    Logger _logger("_log_CLIENT.log", 1024);
    const string nada("");
    #define ADD_EVENT(e) {_logger.addMessage(nada+e);} //generar evento
#else
    #define ADD_EVENT(e)   // nada
#endif

using namespace std;

int aleatorio(int inf, int sup){
    return inf + (rand() % (sup - inf + 1));
}

bool procesarParam(int argc, char* argv[], string& ipAddr, int& port1, int& port2){
    bool error = false;

    if(!error && argc != 4){
        cerr << "[ERROR] Número de parámetros incorrecto: hay " 
        + to_string(argc) + " cuando deberían ser 3\n";

        error = true; 
    }

    else{
        ipAddr = argv[1];
        port1 = atoi(argv[2]);
        port2 = atoi(argv[3]);
    }

    if(!error && (port1 > 65535 || port2 > 65535)){
        cerr << "[ERROR] Puertos inválidos. Puertos obtenidos son " + 
            to_string(port1) + " y " + to_string(port2) 
            + " y uno de ellos supera el valor máximo\n";
        
        error = true;
    }

    if(error){
        cerr << "[INFO] Sintáxis: ./practica_5_CLIENT dirección puerto1 puerto2\n";
    }

    return !error;
}

void clienteUsuario(Socket& ch, int client_fd, int i){
    cout << "[INFO] [clienteUsuario] [" + to_string(i) 
        + "] Cliente iniciado\n";
    
    bool fin = false;
    int recv_bytes, send_bytes, len = 100;
    int c, t;
    string buffer, s;

    for(int j = 0; j < N_TIMES_USER; j++){
        buffer = "RESERVAR_CABINA," + to_string(i);

        send_bytes = ch.Send(client_fd, buffer);

        if(send_bytes == -1){
            cerr << "[ERROR] [clienteUsuario] [" + to_string(i) 
                + "] Al enviar: " + string(strerror(errno)) + "\n";
            ch.Close(client_fd);
            ::exit(1);
        }

        else{
            ADD_EVENT("USER_" + to_string(i) + "_MSG_SENT: " + buffer);
            recv_bytes = ch.Recv(client_fd, buffer, len);

            if(recv_bytes == -1){
                cerr << "[ERROR] [clienteUsuario] [" + to_string(i) 
                    + " Al recibir: " + string(strerror(errno)) + "\n";
                ch.Close(client_fd);
                ::exit(1);
            }

            else if(buffer.rfind("CONCEDIDA,", 0) == 0){
                ADD_EVENT("USER_" + to_string(i) + "_MSG_SENT: " + buffer);
                s = buffer.substr(10);
                c = stoi(s);

                this_thread::sleep_for(chrono::milliseconds(aleatorio(10, 40)));

                buffer = "LIBERAR_CABINA," + to_string(c);

                send_bytes = ch.Send(client_fd, buffer);

                if(send_bytes == -1){
                    cerr << "[ERROR] [clienteUsuario] [" + to_string(i) 
                        + "] Al enviar: " + string(strerror(errno)) + "\n";
                    ch.Close(client_fd);
                    ::exit(1);
                }

                else{
                    ADD_EVENT("USER_" + to_string(i) + "_MSG_SENT: " + buffer);

                    this_thread::sleep_for(chrono::milliseconds(aleatorio(20, 50)));
                }
            }
        }
    }

    buffer = "FIN";

    send_bytes = ch.Send(client_fd, buffer);

    if(send_bytes == -1){
        cerr << "[ERROR] [clienteUsuario] [" + to_string(i) 
            + "] Al enviar: " + string(strerror(errno)) + "\n";
        ch.Close(client_fd);
        ::exit(1);
    }

    else{
        ADD_EVENT("USER_" + to_string(i) + "_MSG_SENT: " + buffer);

        recv_bytes = ch.Recv(client_fd, buffer, len);

        if(recv_bytes == -1){
            cerr << "[ERROR] [clienteUsuario] [" + to_string(i) 
                + "] Al recibir: " + string(strerror(errno)) + "\n";
            ch.Close(client_fd);
            ::exit(1);
        }

        else if(buffer.rfind("TIEMPO_TOTAL,", 0) == 0){
            ADD_EVENT("USER_" + to_string(i) + "_MSG_RECV: " + buffer);

            s = buffer.substr(13);
            cout << "[INFO] [clienteUsuario] [" + to_string(i) 
                + "] Tiempo usando la cabina: " + s + " milisegundos \n";
        }
    }
    
    ch.Close(client_fd);
    cout << "[INFO] [clienteUsuario] [" + to_string(i) 
        + "] Cliente cerrado\n";
}

void clienteLimpiador(Socket& ch, int client_fd){
    cout << "[INFO] [clienteLimpiador] Cliente iniciado\n";

    bool fin = false;
    int recv_bytes, send_bytes, len = 100;
    int c, t;
    string buffer, s;

    for(int i = 0; i < N_TIMES_CLEANING; i++){
        this_thread::sleep_for(chrono::milliseconds(PER_CLEANING));
        buffer = "LIMPIEZA_ENTRAR";

        send_bytes = ch.Send(client_fd, buffer);

        if(send_bytes == -1){
            cerr << "[ERROR] [clienteLimpiador] Al enviar: " 
                + string(strerror(errno)) + "\n";
            ch.Close(client_fd);
            ::exit(1);
        }

        else{
            ADD_EVENT("CLEANER_MSG_SENT: " + buffer);

            recv_bytes = ch.Recv(client_fd, buffer, len);

            if(recv_bytes == -1){
                cerr << "[ERROR] [clienteLimpiador] Al recibir: " 
                    + string(strerror(errno)) + "\n";
                ch.Close(client_fd);
                ::exit(1);
            }

            else if(buffer == "OK_LIMPIEZA"){
                ADD_EVENT("CLEANER_MSG_RECV: " + buffer);

                this_thread::sleep_for(chrono::milliseconds(aleatorio(80, 120)));

                buffer = "LIMPIEZA_SALIR";

                send_bytes = ch.Send(client_fd, buffer);

                if(send_bytes == -1){
                    cerr << "[ERROR] [clienteLimpiador] Al enviar: " 
                        + string(strerror(errno)) + "\n";
                    ch.Close(client_fd);
                    ::exit(1);
                }

                else{
                    ADD_EVENT("CLEANER_MSG_SENT: " + buffer);
                }
            }
        }
    }

    buffer = "FIN";

    send_bytes = ch.Send(client_fd, buffer);

    if(send_bytes == -1){
        cerr << "[ERROR] [clienteLimpiador] Al enviar: " 
            + string(strerror(errno)) + "\n";
        ch.Close(client_fd);
        ::exit(1);
    }

    else{
        ADD_EVENT("CLEANER_MSG_SENT: " + buffer);

        recv_bytes = ch.Recv(client_fd, buffer, len);

        if(recv_bytes == -1){
            cerr << "[ERROR] [clienteLimpiador] Al recibir: " 
                + string(strerror(errno)) + "\n";
            ch.Close(client_fd);
            ::exit(1);
        }

        else if(buffer.rfind("TIEMPO_TOTAL,", 0) == 0){
            ADD_EVENT("CLEANER_MSG_RECV: " + buffer);

            s = buffer.substr(13);
            cout << "[INFO] [clienteLimpiador] Tiempo limpiando: " 
                + s + " milisegundos \n";
        }
    }
    
    ch.Close(client_fd);
    cout << "[INFO] [clienteLimpiador] Cliente cerrado\n";
}

void lanzarUsuario(Socket& ch){
    const int MAX_ATTEMPTS = 10;
    int client_fd[N_USER];
    thread th[N_USER];

    for(int i = 0; i < N_USER; i++){
        int count = 0;

        do{
            client_fd[i] = ch.Connect();
            count++;

            if(client_fd[i] == -1){
                this_thread::sleep_for(chrono::seconds(1));
            }

        }while(client_fd[i] == -1 && count < MAX_ATTEMPTS);

        if(client_fd[i] == -1){
            cerr << "[ERROR] [lanzarUsuario] No se ha podido conectar correctamente al servidor\n";
            ::exit(1);
        }

        
        th[i] = thread(&clienteUsuario, ref(ch), client_fd[i], i);
        cout << "[INFO] [lanzarUsuario] Nuevo cliente: " + to_string(i) + "\n";
    }

    for(int i = 0; i < N_USER; i++){
        th[i].join();
    }
}

void lanzarLimpiador(Socket& ch){
    const int MAX_ATTEMPTS = 10;
    
    int count = 0, client_fd;

    do{
        client_fd = ch.Connect();
        count++;

        if(client_fd == -1){
            this_thread::sleep_for(chrono::seconds(1));
        }

    }while(client_fd == -1 && count < MAX_ATTEMPTS);

    if(client_fd == -1){
        cerr << "[ERROR] [lanzarLimpiador] No se ha podido conectar correctamente al servidor\n";
            ::exit(1);
    }

    cout << "[INFO] [lanzarLimpiador] Nuevo cliente\n";
    clienteLimpiador(ch, client_fd);
}

int main(int argc, char* argv[]){
    int port1, port2;
    string ipAddr;

    if(procesarParam(argc, argv, ipAddr, port1, port2)){
        cout << "[INFO] Dirección destino: " + ipAddr + "\n"; 
        cout << "[INFO] Puertos utilizados: " << to_string(port1) 
                + " " + to_string(port2) + "\n";

        Socket ch_usuario(ipAddr, port1), ch_limpiador(ipAddr, port2);
        thread th[2];

        th[0] = thread(&lanzarUsuario, ref(ch_usuario));
        th[1] = thread(&lanzarLimpiador, ref(ch_limpiador));

        th[0].join();
        th[1].join();

        return 0;
    }

    else{
        return 1;
    }
}