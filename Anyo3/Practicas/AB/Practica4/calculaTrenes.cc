/**
 * @file MHcypher.cc
 * @author Dorian Boleslaw Wozniak  (817570@unizar.es)
 * @author Jesus Mendoza Aranda (777524@unizar.es)
 *
 * @brief Programa que permite encriptar o desencriptar texto utilizando el
 * sistema criptográfico Merkle-Hellman. Para enriptar se utiliza la opción
 * -e; para desencriptar la opción -d. Los últimos parámetros deben ser
 * los elementos de la mochila, y los números N y w.
 *
 * Vease el LEEME o usar -h para una explicación mas detallada del uso
 */

#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <cmath>
#include <chrono>
#include <fstream>
#include <string.h>

using namespace std;

//Vector para los trenes a evaluar
int allTrains[100][3];
//Vector para los posibles trayectos
int allTrips[100][100][3];
//Variables para poda
int gananciaMaxima;
struct Nodo{
    int tren;
    int ganancias;
    int asientosLibres[7];
    int x;
    int xMaxima;
};

int gananciaViaje(Nodo nodo){
    return (allTrips[nodo.tren][nodo.x][2] * (allTrips[nodo.tren][nodo.x][1] - allTrips[nodo.tren][nodo.x][1]));
}

bool sePuedeAceptar(Nodo &nodo){
    //Cmprobamos si se puede aceptar el viaje y ocupamos los asientos
    int inicio = allTrips[nodo.tren][nodo.x][0];
    int final = allTrips[nodo.tren][nodo.x][1];
    int pasajeros = allTrips[nodo.tren][nodo.x][2];
    bool hayEspacio = true;
    for(int i = inicio; i < final; i++){
        if(nodo.asientosLibres[i] < pasajeros){
            hayEspacio = false;
            break;
        }
        nodo.asientosLibres[i] -= pasajeros;
    }
}

int calcularCoste(Nodo nodo){
    for(int i = nodo.x + 1 ; i < nodo.xMaxima; i++){
        //Comprobamos si se puede aceptar el viaje
        Nodo hijo = nodo;
        hijo.x = i;
        //Comprobamos si se puede aceptar el viaje
        if(sePuedeAceptar(hijo)){
            hijo.ganancias += gananciaViaje(hijo);
            if(gananciaMaxima < hijo.ganancias){
                //Añadir a cola
            }
        }
    }  
}

int main(int argc, char **argv)
{
    // Mensaje de ayuda
    if (argc == 1)
    {
        cout << "calculaTrenes fichero_entrada fichero salida;"; 
        exit (1);
    }

    // Ejemplo de uso
    if (argc < 2)
    {
        cerr << "Argumentos insuficientes." << endl
             << "Ejemplo de uso:" << endl
             << "\t./calculaTrenes datos.txt salida.txt"
             << "Para mas ayuda: ./calculaTrenes" << endl;
        exit (1);
    }
    int stringSize;
    char line[256];
    int temp[3];
    stringstream ss;
    ifstream reader;
    int index;
    int it = 0;
    int seatNum;
    int stops;
    int remainingTrips;
    int trainNum;
    int tripsNum;
    //Vector para los trenes a evaluar
    int allTrains[100][3];
    //Vector para la ganancia maxima de cada tren
    float trainPay[100];
    //Vector para el tiempo de calculo de cada tren
    double trainTime[100];
    //Vector para las particiones resultado
    int allTrips[100][28][3];
    //Vector para las particiones intermedias
    reader.open(argv[1]);
    //Leemos el fichero
    while(true){
        reader.getline(line,256);
        it = 0;
        while (it < 3) {
            if(it == 0){
                temp[it] = atoi(strtok(line," "));
            }else{
                temp[it] = atoi(strtok(NULL," "));
            }
            it++;
        }
        if((temp[0] == 0) && (temp[1] == 0) && (temp[2] == 0)){
            break;
        }
        if(remainingTrips == 0){
            if(temp[0] < 1){
                cout << "No tiene sentido que un tren tenga una capacidad de menos de un pasajero" << endl;
                exit(0);
            }
            if(temp[1] < 1){
                cout << "El tren debe ir por lo menos de una estacion 0 a una estacion 1" << endl;
                exit(0);
            }else if(temp[1] > 7){
                cout << "El numero de estacion final debe ser como maximo 7" << endl;
                exit(0);
            }
            if(temp[2] < 1){
                cout << "Se debe presentar al menos un viaje posible" << endl;
                exit(0);
            }
            allTrains[trainNum][0] = temp[0];
            allTrains[trainNum][1] = temp[1];
            allTrains[trainNum][2] = temp[2];
            remainingTrips = temp[2];
            trainNum++;
        }else{
            if(temp[0] >= temp[1]){
                cout << "La estacion de llegada debe ser posterior a la de salida" << endl;
                cout << "Salida: " << temp[0] << "    Llegada: " << temp[1] << endl;
                exit(0);
            }
            if(temp[2] > allTrains[trainNum-1][0]){
                cout << "El viaje propuesto no puede tener mas pasajeros de la capacidad maxima del tren" << endl;
                cout << "Pasajeros viaje: " << temp[2] << "    Capacidad tren: " << allTrains[trainNum-1][0] << endl;
                exit(0);
            }
            remainingTrips--;
            allTrips[trainNum-1][allTrains[trainNum-1][2]-remainingTrips-1][0] = temp[0];
            allTrips[trainNum-1][allTrains[trainNum-1][2]-remainingTrips-1][1] = temp[1];
            allTrips[trainNum-1][allTrains[trainNum-1][2]-remainingTrips-1][2] = temp[2];
        }
    }
    reader.close();
    for(int i = 0; i < trainNum; i++){
        cout << endl <<"TREN " << i+1 << endl;
        cout << allTrains[i][0] << " ";
        cout << allTrains[i][1] << " ";
        cout << allTrains[i][2] << endl;
        cout <<"VIAJES A CUBRIR " << endl;
        for(int j = 0; j < allTrains[i][2]; j++){

            cout << allTrips[i][j][0] << " ";
            cout << allTrips[i][j][1] << " ";
            cout << allTrips[i][j][2] << endl;
        }
    }
    
    for(int i = 0; i < trainNum; i++){
        Nodo raiz = {i,0,{0,0,0,0,0,0,0},0,allTrains[i][2]};
        for(int j = 0; j < 7; j++){
            raiz.asientosLibres[j] = allTrains[i][0];
        }
        //Reseteamos la cota
        gananciaMaxima = 0;
        auto calc_begin = std::chrono::high_resolution_clock::now();
        trainPay[i] = calcularCoste(raiz);
        trainTime[i] = (double)(std::chrono::high_resolution_clock::now()-calc_begin).count()/std::chrono::high_resolution_clock::period::den;
    }
    cout << endl << "<Ganancias>" << setw(25) << "<Tiempo de calculo>" << endl;
    for(int i = 0; i < trainNum; i++){
        cout << setw(3) <<trainPay[i] << setw(20) << trainTime[i] << endl;
    }
}
