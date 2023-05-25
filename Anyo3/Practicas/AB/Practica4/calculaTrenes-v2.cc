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

// Vector para los trenes a evaluar
int allTrains[100][3];
// Vector para los posibles trayectos
static int allTrips[100][100][3];

// Variables para poda
struct Nodo
{
    int tren;
    int cota;
    int coste;
    int asientosLibres[7];
    int x;
    int xMaxima;
};

int gananciaViaje(Nodo nodo)
{
    return (allTrips[nodo.tren][nodo.x - 1][2] * (allTrips[nodo.tren][nodo.x - 1][1] - allTrips[nodo.tren][nodo.x - 1][0]));
}

bool sePuedeAceptar(Nodo &nodo)
{
    // Cmprobamos si se puede aceptar el viaje y ocupamos los asientos
    int inicio = allTrips[nodo.tren][nodo.x - 1][0];
    int final = allTrips[nodo.tren][nodo.x - 1][1];
    int pasajeros = allTrips[nodo.tren][nodo.x - 1][2];
    bool hayEspacio = true;
    for (int i = inicio; i < final; i++)
    {
        if (nodo.asientosLibres[i] < pasajeros)
        {
            hayEspacio = false;
            break;
        }
        nodo.asientosLibres[i] -= pasajeros;
    }
    return hayEspacio;
}

int calcularGanancia(vector<Nodo> nodos)
{
    int desperdicioMinimo = 1000000000;
    while (true)
    {
        // Se elige el nodo mas prometedor
        int expandido;
        int expandidoCoste = 1000000000;
        for (int i = 0; i < nodos.size(); i++)
        {
            if (nodos[i].coste < expandidoCoste)
            {
                expandido = i;
            }
        }
        // Se generan los hijos del nodo mas prometedor
        for (int i = nodos[expandido].x + 1; i < nodos[expandido].xMaxima; i++)
        {
            Nodo hijo = nodos[expandido];
            hijo.x = i;
            // Comprobamos si se puede aceptar el viaje
            if (sePuedeAceptar(hijo))
            {
                hijo.cota -= gananciaViaje(hijo);
                // Actualizamos la cota
                if (hijo.cota < desperdicioMinimo)
                {
                    desperdicioMinimo = hijo.cota;
                }

                // Calculamos el coste
                for (int j = nodos[expandido].x + 1; j < hijo.x; j++)
                {
                    Nodo aux = hijo;
                    aux.x = j;
                    hijo.coste += gananciaViaje(aux);
                }
                nodos.push_back(hijo);
            }
        }
        // Quitamos el nodo expandido de la lista de vivos
        nodos.erase(nodos.begin() + expandido);
        // Podamos los nodos cuyo coste sea mayor que el desperdicio minimo
        int vectorOffset = 0;
        for (int i = 0; i < nodos.size(); i++)
        {
            if (nodos[i - vectorOffset].coste > desperdicioMinimo)
            {
                nodos.erase(nodos.begin() + expandido);
                vectorOffset++;
            }
        }
        // Si no quedan mas nodos vivos que expandir salimos
        if (nodos.size() == 0)
        {
            break;
        }
    }
    return desperdicioMinimo;
}

int main(int argc, char **argv)
{
    // Mensaje de ayuda
    if (argc == 1)
    {
        cout << "calculaTrenes fichero_entrada fichero salida;";
        exit(1);
    }

    // Ejemplo de uso
    if (argc < 2)
    {
        cerr << "Argumentos insuficientes." << endl
             << "Ejemplo de uso:" << endl
             << "\t./calculaTrenes datos.txt salida.txt"
             << "Para mas ayuda: ./calculaTrenes" << endl;
        exit(1);
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
    int remainingTrips = 0;
    int trainNum = 0;
    int tripsNum;
    // Vector para la ganancia maxima de cada tren
    float trainPay[100];
    // Vector para el tiempo de calculo de cada tren
    double trainTime[100];
    // Vector para las particiones intermedias
    reader.open(argv[1]);
    // Leemos el fichero
    while (true)
    {
        reader.getline(line, 256);
        it = 0;
        while (it < 3)
        {
            if (it == 0)
            {
                temp[it] = atoi(strtok(line, " "));
            }
            else
            {
                temp[it] = atoi(strtok(NULL, " "));
            }
            it++;
        }
        if ((temp[0] == 0) && (temp[1] == 0) && (temp[2] == 0))
        {
            break;
        }
        if (remainingTrips == 0)
        {
            if (temp[0] < 1)
            {
                cout << "No tiene sentido que un tren tenga una capacidad de menos de un pasajero" << endl;
                exit(0);
            }
            if (temp[1] < 1)
            {
                cout << "El tren debe ir por lo menos de una estacion 0 a una estacion 1" << endl;
                exit(0);
            }
            else if (temp[1] > 7)
            {
                cout << "El numero de estacion final debe ser como maximo 7" << endl;
                exit(0);
            }
            if (temp[2] < 1)
            {
                cout << "Se debe presentar al menos un viaje posible" << endl;
                exit(0);
            }
            allTrains[trainNum][0] = temp[0];
            allTrains[trainNum][1] = temp[1];
            allTrains[trainNum][2] = temp[2];
            remainingTrips = temp[2];
            trainNum++;
        }
        else
        {
            if (temp[0] >= temp[1])
            {
                cout << "La estacion de llegada debe ser posterior a la de salida" << endl;
                cout << "Salida: " << temp[0] << "    Llegada: " << temp[1] << endl;
                exit(0);
            }
            if (temp[2] > allTrains[trainNum - 1][0])
            {
                cout << "El viaje propuesto no puede tener mas pasajeros de la capacidad maxima del tren" << endl;
                cout << "Pasajeros viaje: " << temp[2] << "    Capacidad tren: " << allTrains[trainNum - 1][0] << endl;
                exit(0);
            }
            remainingTrips--;
            allTrips[trainNum - 1][allTrains[trainNum - 1][2] - remainingTrips - 1][0] = temp[0];
            allTrips[trainNum - 1][allTrains[trainNum - 1][2] - remainingTrips - 1][1] = temp[1];
            allTrips[trainNum - 1][allTrains[trainNum - 1][2] - remainingTrips - 1][2] = temp[2];
        }
    }
    reader.close();

    for (int i = 0; i < trainNum; i++)
    {
        vector<Nodo> vectorNodos;
        Nodo raiz = {i, 0, 0, {0, 0, 0, 0, 0, 0, 0}, 0, allTrains[i][2]};
        // Inicializamos los asientos libres
        for (int j = 0; j < 7; j++)
        {
            raiz.asientosLibres[j] = allTrains[i][0];
        }
        // Calculamos la ganancia si pudieramos aceptar todos los viajes
        for (int j = 0; j < raiz.xMaxima; j++)
        {
            int ganancia = (allTrips[i][j][1] - allTrips[i][j][0]) * allTrips[i][j][2];
            raiz.cota += ganancia;
        }
        vectorNodos.push_back(raiz);
        auto calc_begin = std::chrono::high_resolution_clock::now();
        trainPay[i] = raiz.cota - calcularGanancia(vectorNodos);
        // trainPay[i] = (float)6;
        trainTime[i] = (double)(std::chrono::high_resolution_clock::now() - calc_begin).count() / std::chrono::high_resolution_clock::period::den;
    }
    ofstream writer(argv[2]);
    writer << endl
           << "<Ganancias>" << setw(25) << "<Tiempo de calculo>" << endl;
    for (int i = 0; i < trainNum; i++)
    {
        writer << setw(3) << trainPay[i] << setw(20) << trainTime[i] << endl;
    }
    writer.close();
}
