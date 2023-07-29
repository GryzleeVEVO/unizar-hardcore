/**
 * @file pinball.cc
 * @author Dorian Boleslaw Wozniak  (817570@unizar.es)
 * @author Jesus Mendoza Aranda (777524@unizar.es)
 *
 * @brief Programa que calcula, a partir de un fichero de parámetros,
 *          el lugar donde caerían las primeras i-ésimas bolas dentro
 *          de un árbol de profundidad P. El programa también compueba que
 *          el árbol exista (profundidad mayor o igual que 1), y que 
 *          las bolas tiradas sean entre 1 y el número de hojas terminales
 *          del árbol. Escribe los resultados en un fichero de salida.
 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <chrono>
#include <cmath>

using namespace std;

// Devuelve una marca de tiempo
chrono::high_resolution_clock::time_point iniciarCrono()
{
    return chrono::high_resolution_clock::now();
}

// Devuelve el tiempo transcurrido a partir de una marca de tiempo
double pararCrono(chrono::high_resolution_clock::time_point start)
{
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> time = end - start;
    return time.count();
}

/**
 * @brief Calcula el nodo donde caerá la i-ésima bola dentro
 *          de un árbol binario de profundidad P. 
 * 
 * @param N i-ésima bola
 * @param P Profundidad
 * @param node Nodo actual
 * @return long Nodo resultado
 */
long calcEnd(int N, int P, long node)
{
    if (P <= 0)
        return -1;
    else if (P == 1)
        return node;
    else
        return calcEnd(N / 2 + N % 2, P - 1, node * 2 + (1 - N % 2));
}

int main(int argc, char **argv)
{
    // Mensaje de error
    if (argc < 3)
    {
        cerr << "Argumentos insuficientes. Usar ./pinball param.txt output.txt"
             << endl;
        return 1;
    }

    // Obtiene ficheros de entrada y salida
    ifstream reader(argv[1]);
    ofstream writer(argv[2]);

    if (!reader.is_open())
    {
        cerr << "El fichero " << argv[1] << " no existe o no se ha podido abrir"
             << endl;

        return 1;
    }

    if (!writer.is_open())
    {
        cerr << "El fichero " << argv[2] << " no se ha podido abrir"
             << endl;

        return 1;
    }

    int profundidad, bolas;

    // Mientras queden lineas en el fichero
    while (reader >> profundidad >> bolas)
    {
        writer << "Profundidad: " << profundidad << ", Bolas: " << bolas
               << endl;

        if (profundidad < 1)
        {
            writer << "La profundidad debe ser 1 o mayor" << endl;
        }
        else if (bolas < 1)
        {
            writer << "El número de bolas debe de ser 1 o mayor" << endl;
        }
        else if (bolas > pow(2, profundidad - 1))
        {
            writer << "Sólo caben hasta " << fixed << setprecision(0)
                   << pow(2, profundidad - 1)
                   << " bolas en el árbol" << endl;
        }
        else
        {
            // Almacena los resultados obtenidos
            long resultados[bolas];

            // Calculamos la posicion de las bolas
            auto start = iniciarCrono();

            for (int i = 1; i <= bolas; i++)
            {
                resultados[i - 1] = calcEnd(i, profundidad, 1);
            }

            double tiempo = pararCrono(start);

            // Escribimos los resultados
            for (int i = 0; i < bolas; i++)
            {
                writer << "Bola: " << i + 1 << " , posicion: " << resultados[i]
                       << endl;
            }

            // Escribimos el tiempo de ejecución
            writer << "Tiempo de calculo: " << fixed << setprecision(6)
                   << tiempo << " segundos" << endl;
        }

        writer << endl;
    }

    reader.close();
    writer.close();

    return 0;
}