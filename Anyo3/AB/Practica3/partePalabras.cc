/**
 * @file partePalabras.cc
 * @author Dorian Boleslaw Wozniak  (817570@unizar.es)
 * @author Jesus Mendoza Aranda (777524@unizar.es)
 *
 * @brief Programa que, a partir de un diccionario definido por una serie de
 * palabras, recibe una cadena de palabras (alfanuméricas, sin espacios).
 * Si la secuencia se puede dividir en una serie de palabras del diccionario
 * dado, mostrará todas las combinaciones posibles.
 *
 * Invocar como:
 *
 * ./partePalabras diccionario entrada
 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <algorithm>

using namespace std;

// Implementa un cronómetro para medir el tiempo de ejecución
class Reloj
{
private:
    // Marca de tiempo
    chrono::high_resolution_clock::time_point t1, t2;

public:
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

/**
 * @brief Implementa un diccionario que puede almacenar una serie de elementos.
 */
class Diccionario
{
private:
    // Vector de términos del diccionario
    vector<string> dic;

    // Tamaño de la palabra mas corta/larga del diccionario
    int min = 0, max = 0;

    /**
     * @brief Devuelve si el diccionario contiene el término s
     */
    bool contiene(string s)
    {
        for (string termino : dic)
        {
            if (termino == s)
                return true;
        }

        return false;
    }

public:
    /**
     * @brief Crea un nuevo diccionario a partir de un fichero con los términos
     * que contendrá.
     *
     * @param file Fichero con los términos del diccionario
     */
    Diccionario(string file)
    {
        ifstream f(file);

        if (!f.is_open())
        {
            cerr << "No se ha podido abrir el fichero " << file << endl;
            exit(1);
        }

        string termino;

        while (f >> termino)
        {
            dic.push_back(termino);

            min = (!min || min > termino.length()) ? termino.length() : min;
            max = (!max || max < termino.length()) ? termino.length() : max;
        }
    }

    /**
     * @brief Devuelve, si existen, todas las formas en las que se puede partir
     * s con los términos del diccionario.
     *
     * @param s Cadena a partir
     * @return Vector con todas las particiones posibles de s (vacío si no se
     * puede partir de ninguna forma)
     */
    vector<string> partir(string s)
    {
        vector<vector<string>> dp(s.length() + 1);
        dp[0].push_back("");

        // Los bucles revisan solo prefijos del tamaño entre [min, max]
        for (int fin = min; fin <= s.length(); fin++)
        {
            vector<string> temp;

            for (int ini = (fin > max) ? (fin - max) : 0;
                 ini < fin && fin - ini >= min; ini++)
            {
                string prefijo = s.substr(ini, fin - ini);

                // Si el prefijo es una palabra del diccionario, la combina
                // con las soluciones anteriores
                if (dp[ini].size() && contiene(prefijo))
                {
                    for (string parcial : dp[ini])
                    {
                        temp.push_back(
                            parcial + (parcial.length() ? " " : "") + prefijo);
                    }
                }
            }

            // Añade soluciones parciales para el prefijo
            // (vacío si no se puede dividir)
            dp[fin] = temp;
        }

        return dp[s.length()];
    }
};

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        cerr << "Faltan argumentos." << endl
             << "Uso: ./partePalabras diccionario cadena" << endl;

        return 1;
    }

    Reloj r;

    r.iniciar();
    Diccionario d(argv[1]);
    double tiempo = r.parar();

    cout << "Se ha cargado el diccionario en " << fixed << setprecision(6)
         << tiempo << " ms" << endl;

    for (int i = 2; i < argc; i++)
    {
        cout << endl;

        r.iniciar();
        vector<string> particiones = d.partir(argv[i]);
        tiempo = r.parar();

        if (particiones.size())
        {
            for (string s : particiones)
            {
                cout << s << endl;
            }
        }
        else
        {
            cout << "No se puede partir la cadena." << endl;
        }

        cout << "Se ha solucionado el problema en " << fixed << setprecision(6)
             << tiempo << "ms" << endl;
    }
    return 0;
}
