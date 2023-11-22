/**
 * Algoritmia para problemas difíciles
 *
 * Práctica 1
 *
 * Autor: Dorian Boleslaw Wozniak (817570@unizar.es)
 * Autor: Alvaro Seral Gracia (819425@unizar.es)
 *
 * Descripción: Funciones destinadas a la creación de pruebas 
 * para algoritmos de ordenación.
 *
 */

#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

/**
 * @brief Imprime el modo de escribir correctamente los argumentos 
 * a la hora de ejecutar el programa.
 * 
 * @return EXIT_FAILURE
*/
bool argumentos_incorrectos()
{
    cerr << "Argumentos incorrectos.\nModos de ejecución:" << endl;
    cerr << "  -  ./generarPruebas --(int, double) [fichero] [tamaño] [valor min] [valor max]" << endl;
    cerr << "  -  ./generarPruebas --string [fichero] [tamaño]" << endl;
    cerr << "  -  ./generarPruebas --(ordered, reversed) [fichero] [valor min] [valor max]" << endl;
    cerr << "  -  ./generarPruebas --Nrandom [directorio (en carpeta dataset/synthetic)] [tamaño] [valor min] [valor max] [num veces]" << endl;
    cerr << "  -  ./generarPruebas --Nscalar [directorio (en carpeta dataset/synthetic)] [tamaño max] [valor min] [valor max] [frecuencia de crecimiento]" << endl;
    cerr << "  -  ./generarPruebas --fromCsv [fichero origen (en carpeta dataset/real/raw)] [fichero destino (en carpeta dataset/real/processed)] [columna] [separador]" << endl;
    return EXIT_FAILURE;
}

/**
 * @brief Abre un fichero en modo escritura
 * 
 * @param file Fichero
 * @param f Instancia del fichero
 * @return True si se ha abierto correctamente
*/
bool abrir_fichero_out(string file, ofstream &f) 
{
    f.open(file);
    if (!f.is_open()) 
    {
        cerr << "Fallo al crear el fichero " << file << endl;
        return false;
    }
    return true;
}

/**
 * @brief Abre un fichero en modo lectura
 * 
 * @param file Fichero
 * @param f Instancia del fichero
 * @return True si se ha abierto correctamente
*/
bool abrir_fichero_in(string file, ifstream &f) 
{
    f.open(file);
    if (!f.is_open()) 
    {
        cerr << "Fallo al abrir el fichero " << file << endl;
        return false;
    }
    return true;
}

/******************************************************************************/

/**
 * @brief Devuelve un entero aleatorio en un rango [min, max].
 *
 * @param min Valor mínimo del rango
 * @param max Valor máximo del rango
 * @return Número entero aleatorio
 */
int random(int min, int max)
{
    return (rand() % (max - min)) + min;
}

/**
 * @brief Devuelve un real aleatorio en un rango [min, max].
 *
 * @param min Valor mínimo del rango
 * @param max Valor máximo del rango
 * @return Número real aleatorio
 */
double random(double min, double max)
{
    return (rand() / (RAND_MAX / (max - min))) + min;
}

/**
 * @brief Devuelve una cadena de carácteres aleatoria de longitud "length".
 *
 * @param length Longitud de la cadena
 * @return Cadena de carácteres aleatoria
 */
string random(int length)
{
    string charset = "0123456789"
                     "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                     "abcdefghijklmnopqrstuvwxyz",
           res = "";

    for (int i = 0; i < length; i++)
    {
        res += charset[random(0, charset.length() - 1)];
    }

    return res;
}

/******************************************************************************/

/**
 * @brief Rellena un fichero con "size" filas, 
 * donde cada fila tiene un número entero.
 * 
 * @param file Ubicación del fichero de escritura
 * @param size Número de filas
 * @param min Valor mínimo de los números
 * @param max Valor máximo de los números
*/ 
void generar(string file, int size, int min, int max) 
{
    ofstream f;
    if(!abrir_fichero_out(file, f))
        return;

    for (int i = 0; i < size; i++)
        f << random(min, max) << endl;

    f.close();
}

/**
 * @brief Rellena un fichero con "size" filas, 
 * donde cada fila tiene un número real.
 * 
 * @param file Ubicación del fichero de escritura
 * @param size Número de filas
 * @param min Valor mínimo de los números
 * @param max Valor máximo de los números
*/ 
void generar(string file, int size, double min, double max) 
{
    ofstream f;
    if(!abrir_fichero_out(file, f))
        return;

    for (int i = 0; i < size; i++)
        f << random(min, max) << endl;
    
    f.close();
}

/**
 * @brief Rellena un fichero con "size" filas, 
 * donde cada fila tiene una cadena de carácteres.
 * 
 * @param file Ubicación del fichero de escritura
 * @param size Número de filas
*/ 
void generar(string file, int size)
{
    ofstream f;
    if(!abrir_fichero_out(file, f))
        return;
        
    for (int i = 0; i < size; i++)
        f << random(random(1, 25)) << endl;
    
    f.close();
}

/**
 * @brief Rellena un fichero con "size" filas, 
 * donde cada fila tiene un número entero.
 * Si order=True, la primera fila vale min, cada fila aumenta el valor en 1 
 * hasta llegar al valor max.
 * Si order=False, la primera fila vale max, cada fila disminuye el valor en 1 
 * hasta llegar al valor min.
 * 
 * @param file Ubicación del fichero de escritura
 * @param min Valor mínimo de los números
 * @param max Valor máximo de los números
 * @param order True para fichero ordenado normal, 
 *              False para fichero ordenado inversamente
*/ 
void generar(string file, int min, int max, bool order)
{
    ofstream f;
    if(!abrir_fichero_out(file, f))
        return;

    if (order)
    {
        for (int i = min; i <= max; i++)
            f << i << endl;
    }
    else
    {
        for (int i = max; i >= min; i--)
            f << i << endl;
    }

    f.close();
}

/**
 * @brief Rellena "times" ficheros con "size" filas, 
 * donde cada fila tiene un número entero.
 * 
 * @param dir Ubicación del directorio para fichero de escritura
 * @param size Número de filas
 * @param min Valor mínimo de los números
 * @param max Valor máximo de los números
 * @param times Número de ficheros
*/ 
void generarNrandom(string dir, int size, int min, int max, int times)
{
    string file;
    for (int i = 1; i <= times; i++)
    {
        file = "/"+to_string(i)+".txt";
        generar(dir+file, size, min, max);
    }
}

/**
 * @brief Rellena ficheros con un número de filas incremental, 
 * donde cada fila tiene un número entero.
 * Con cada fichero nuevo, se rellenan el número de línes del anteior 
 * más "frecuencia" hasta llegar a "size" filas.
 * 
 * @param dir Ubicación del directorio para fichero de escritura
 * @param size Número de filas
 * @param min Valor mínimo de los números
 * @param max Valor máximo de los números
 * @param frecuencia Frecuencia de crecimiento de filas
*/ 
void generarNscalar(string dir, int size, int min, int max, int freq)
{
    string file;
    int num = 1;
    for (int i = 1; i <= size; i+=freq)
    {
        file = "/"+to_string(num)+".txt";
        generar(dir+file, i, min, max);
        num++;
    }
}

/**
 * @brief Rellena un fichero con los elementos de una columna concreta 
 * de otro fichero csv.
 * 
 * @param fOrigen Ubicación del fichero de lectura
 * @param fDestino Ubicación del fichero de escritura
 * @param col Número de la columna
 * @param sep Carácter separador de columnas
*/ 
void generarDeCSV(string fOrigen, string fDestino, int col, char sep)
{
    ifstream i;
    if(!abrir_fichero_in("dataset/real/raw/"+fOrigen, i))
        return;
    ofstream o;
    if(!abrir_fichero_out("dataset/real/processed/"+fDestino, o))
        return;

    string line, token;
    int count;

    getline(i, line);
    while(getline(i, line)) 
    {
        istringstream ss(line);
        count = 1;
        while(getline(ss, token, sep)) 
        {
            if (count == col)
            {
                o << token << endl;
                break;
            }
            count++;
        }
    }

    o.close();
    i.close();
}

/******************************************************************************/

int main(int argc, char *argv[])
{
    srand(time(NULL));
    const string synthetic = "dataset/synthetic/";

    if (argc == 6 && (string)argv[1] == "--int")
        generar(synthetic+(string)argv[2], stoi(argv[3]), stoi(argv[4]), stoi(argv[5]));
    else if (argc == 6 && (string)argv[1] == "--double")
        generar(synthetic+(string)argv[2], stoi(argv[3]), stod(argv[4]), stod(argv[5]));
    else if (argc == 4 && (string)argv[1] == "--string")
        generar(synthetic+(string)argv[2], stoi(argv[3]));
    else if (argc == 5 && (string)argv[1] == "--ordered")
        generar(synthetic+(string)argv[2], stoi(argv[3]), stoi(argv[4]), true);
    else if (argc == 5 && (string)argv[1] == "--reversed")
        generar(synthetic+(string)argv[2], stoi(argv[3]), stoi(argv[4]), false);
    else if (argc == 7 && (string)argv[1] == "--Nrandom")
        generarNrandom(synthetic+(string)argv[2], stoi(argv[3]), stoi(argv[4]), stoi(argv[5]), stoi(argv[6]));
    else if (argc == 7 && (string)argv[1] == "--Nscalar")
        generarNscalar(synthetic+(string)argv[2], stoi(argv[3]), stoi(argv[4]), stoi(argv[5]), stoi(argv[6]));
    else if (argc == 6 && (string)argv[1] == "--fromCsv")
        generarDeCSV((string)argv[2], (string)argv[3], stoi(argv[4]), argv[5][0]);
    else
        return argumentos_incorrectos();

    cout << "Finalizado." << endl;
    return EXIT_SUCCESS;
}