/**
 * @file calculaTrenes.cc
 * @author Dorian Boleslaw Wozniak  (817570@unizar.es)
 * @author Jesus Mendoza Aranda (777524@unizar.es)
 *
 * @brief
 */

#include <sstream>
#include <cmath>
#include <fstream>
#include <cstring>

#include <iostream>
#include <iomanip>
#include <vector>
#include <queue>
#include <chrono>

using namespace std;

const int NUM_ESTACIONES = 7;
const int MAX_PEDIDOS = 22;

// Estructura que almacena un pedido individual para un recorrido de tren
struct Pedido
{
    int salida, llegada, pasajeros;
};

// Vector que almacena una serie de pedidos para un recorrido de tren
typedef vector<Pedido> ListaPedidos;

// Estructura que almacena la información de un viaje de un tren
struct Tren
{
    int capacidad, final, numPedidos;
    ListaPedidos *pedidos;
};

// Vector que almacena una serie de trenes, sus recorridos y pedidos asociados
typedef vector<Tren> ListaTrenes;

// Representa un nodo
struct Nodo
{

    vector<int> asientos;

    vector<Nodo *> hijos;
}

// Implementa un cronómetro para medir el tiempo de ejecución
class Reloj
{
private:
    // Marca de tiempo
    chrono::high_resolution_clock::time_point t1, t2;

public:
    Reloj()
    {
        iniciar();
    }

    // Inicia a medir el tiempo.
    void iniciar()
    {
        t1 = chrono::high_resolution_clock::now();
    }

    // Devuelve el tiempo transcurrido en milisegundos.
    double parar()
    {
        t2 = chrono::high_resolution_clock::now();
        chrono::duration<double> time = t2 - t1;
        return time.count();
    }
};

bool comprobarTren(int capacidad, int final, int pedidos)
{
    if (!capacidad)
    {
        cerr << "Error: Tren con capacidad 0" << endl;
        return false;
    }

    if (final < 0 || final > NUM_ESTACIONES)
    {
        cerr << "Error: Línea final del tren fuera de rango (se esperaba [0, "
             << NUM_ESTACIONES << "], se ha introducido " << final << endl;
        return false;
    }

    if (!pedidos || MAX_PEDIDOS)
    {
        cerr << "Error: Número de pedidos fuera de rango (se esperaba [1, "
             << MAX_PEDIDOS << "], se ha introducido " << final << endl;
        return false;
    }

    return true;
}

bool comprobarPedido(int salida, int llegada, int pasajeros, int final)
{
    if (salida < 0 || salida >= final)
    {
        cerr << "Error: Estación de salida fuera de rango (se esperaba [0, "
             << final << "], se ha introducido " << salida << endl;
        return false;
    }

    if (llegada < 0 || llegada >= final)
    {
        cerr << "Error: Estación de llegada fuera de rango (se esperaba [0, "
             << final << "], se ha introducido " << llegada << endl;
        return false;
    }

    if (salida >= llegada)
    {
        cerr << "Error: Trayecto inválido ("
             << salida << " -> " << llegada << ")" << endl;
        return false;
    }

    if (!pasajeros)
    {
        cerr << "Error: el pedido necesita al menos un pasajero" << endl;
        return false;
    }
}

int ganancia() {
    
}

double calcularGanancia()
{
}



int main(int argc, char **argv)
{
    // Mensaje de ayuda
    if (argc < 2)
    {
        cout << "Uso: calculaTrenes fichero_entrada fichero salida;";
        return 1;
    }

    ifstream reader(argv[1]);
    ListaTrenes lt;
    int capacidad, final, pedidos;

    // Lee cada bloque y lo almacena en la lista de trenes
    while (reader >> capacidad >> final >> pedidos)
    {
        // Caso 0 0 0 (fin lista)
        if (!capacidad && !final && !pedidos)
            break;

        if (!comprobarTren(capacidad, final, pedidos))
            return 1;

        int salida, llegada, pasajeros;
        ListaPedidos lp;

        // Obtiene los pedidos de cada bloque
        for (int i = 0; i < pedidos; i++)
        {
            if (reader.eof())
            {
                cerr << "Error: fin de archivo antes de leer todos los pedidos"
                     << endl;
                return 1;
            }

            reader >> salida >> llegada >> pasajeros;

            if (!comprobarPedido(salida, llegada, pasajeros, final))
                return 1;

            Pedido p = {salida, llegada, pasajeros};
            lp.push_back(p);
        }

        Tren t = {capacidad, final, pedidos, &lp};
        lt.push_back(t);
    }

    reader.close();

    // Trata de resolver cada instancia del problema
    for (auto i : lt)
    {
        cout << "Solucionando: (" << i.capacidad << ", " << i.final << ", "
             << i.pedidos << ")" << endl;

        Reloj clk;

        int ganancia = calcularGanancias();

        double tiempo = clk.parar();
    }

    return 0;
}
