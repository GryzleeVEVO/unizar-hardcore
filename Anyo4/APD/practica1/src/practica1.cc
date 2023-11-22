/**
 * Algoritmia para problemas difíciles
 *
 * Práctica 1
 *
 * Autor: Dorian Boleslaw Wozniak (817570@unizar.es)
 * Autor: Alvaro Seral Gracia (819425@unizar.es)
 *
 * Descripción: Funciones relativas a los algoritmos de ordenación 
 * QuickSort, RadixSort y HeapSort.
 *
 */

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <climits>
#include <cmath>
#include <stdio.h>
#include <sys/time.h>

#define TIMES 100

using namespace std;

enum SORT_TYPE {
    ENTERO = 0,
    DECIMAL = 1,
    CADENA = 2,
};

/******************************************************************************/
/* HEAP SORT */

/**
 * @brief Aplica transformaciones mantener la propiedad de montículo en un
 * vector de entradas. La propiedad de montículo se cumple si para todos los
 * nodos de un árbol sus hijos son menores.
 * 
 * Esta función compara el elemento en la posición "root" con sus hijos y, 
 * si es necesario, realiza un intercambio para que el elemento más grande 
 * esté en la posición "root".
 *
 * Para representar el árbol binario, se utiliza el propio vector, en el cual
 * el elemento i es un nodo, y sus hijos los nodos en la posición (2i+1)
 * y (2i+2).
 *
 * @tparam T Tipo de dato debe tener definidos los operadores de comparación.
 * @param v Vector de entradas
 * @param size Tamaño del vector
 * @param root Nodo raíz del subárbol
 */
template <typename T>
void heapify(T v[], int size, int root = 0)
{
    // La raíz no existe
    if (root >= size)
        return;

    int left = 2 * root + 1,
        right = 2 * root + 2;

    // Busca el elemento más grande entre la raíz y sus hijos
    int largest = root;
    if (left < size && v[largest] < v[left])
        largest = left;
    if (right < size && v[largest] < v[right])
        largest = right;

    // Si el elemento más grande no es la raíz, 
    // intercambia los valores y vuelve a llamar recursivamente
    if (root != largest)
    {
        T aux = v[root];
        v[root] = v[largest];
        v[largest] = aux;
        heapify(v, size, largest);
    }
}

/**
 * @brief Ordena un vector utilizando heapsort.
 * 
 * El algoritmo primero construye un montículo a partir del vector y luego
 * repite un proceso de extracción y restauración del montículo para 
 * ordenar el vector.
 *
 * @tparam T Tipo de dato debe tener definidos los operadores de comparación.
 * @param v Vector de entradas
 * @param size Tamaño del vector
 */
template <typename T>
void heap_sort(T v[], int size)
{
    // Se asegura que el vector cumpla la propiedad de montículo
    for (int s = size / 2 - 1; s >= 0; s--)
        heapify(v, size, s);

    // Para cada iteración, se intercambia la raíz con el último elemento y se
    // vuelve a aplicar la propiedad de montículo sobre el vector
    T aux;
    for (int s = size; s >= 1; s--)
    {
        aux = v[0];
        v[0] = v[s - 1];
        v[s - 1] = aux;
        heapify(v, s - 1);
    }
}

/******************************************************************************/
/* QUICK SORT */

/**
 * @brief Función recursiva que ordena un subvector utilizando quicksort.
 *
 * @tparam T Tipo de dato debe tener definidos los operadores de comparación.
 * @param v Vector de entradas
 * @param head Primer elemento del vector
 * @param tail Último elemento del vector
 */
template <typename T>
void quick_sort(T v[], int head, int tail)
{
    // La lista ya está ordenada si solo contiene un elemento
    if (head >= tail)
        return;

    int lo = head,     // Puntero al primer elemento
        hi = tail - 1, // Puntero al último elemento
        pivot = tail;  // Pivote (asume último en lista)
    T aux;

    // Recorre el vector
    while (lo != hi)
    {

        // Encuentra dos elementos a intercambiar
        if (v[lo] > v[pivot] && v[hi] < v[pivot])
        {
            aux = v[lo];
            v[lo] = v[hi];
            v[hi] = aux;
        }
        // Avanza los punteros
        else
        {
            if (lo == pivot || v[lo] <= v[pivot])
            {
                lo++;
            }

            else if (hi == pivot || v[hi] >= v[pivot])
            {
                hi--;
            }
        }
    }

    // Intercambia el pivote con el punto de intersección
    if (v[pivot] < v[lo])
    {
        aux = v[pivot];
        v[pivot] = v[hi];
        v[hi] = aux;
        pivot = lo;
    }

    // Llamadas recursivas para cada subvector
    quick_sort(v, head, pivot - 1);
    quick_sort(v, pivot + 1, tail);
}

/**
 * @brief Ordena un vector utilizando quicksort. Quicksort utiliza un pivote
 * sobre el que se intercambian elementos que sean mayores o menores, realizando
 * una llamada recursiva para cada subvector a ambos lados del pivote.
 *
 * @tparam T Tipo de dato debe tener definidos los operadores de comparación.
 * @param v Vector de entradas
 * @param size Tamaño del vector
 */
template <typename T>
void quick_sort(T v[], int size)
{
    quick_sort(v, 0, size - 1);
}

/******************************************************************************/
/* RADIX SORT */

/**
 * @brief Devuelve el dígito del número dado en la posición dada.
 *
 * @param num Número
 * @param digit Dígito dado. El dígito debe estar en el rango [1, num. dígitos]
 * @return Dígito solicitado del número dado
 */
int get_digit(int num, int digit)
{
    return num / (int)pow(10, (digit - 1)) % 10;
}

/**
 * @brief Devuelve el número de dígitos que contiene un número.
 *
 * @param num Número
 * @return Número de dígitos del número
 */
int get_number_of_digits(int num)
{
    if (num / 10 == 0)
        return 1;
    return 1 + get_number_of_digits(num / 10);
}

/**
 * @brief Función recursiva que ordena el vector v usando radix sort. La función
 * ordena el vector dígito a dígito usando ordenamiento por cuentas.
 *
 * @param v Vector de entradas
 * @param size Tamaño del vector
 * @param current_digit Dígito actual a ordenar
 * @param max_digits Número de dígitos máximo
 */
void radix_sort(int v[], int size, int current_digit, int max_digits)
{
    // Se utiliza count sort para ordenar la lista del tamaño de la base
    int count[10] = {};

    // Cada elemento de count cuenta el número de elementos  
    // con la misma raíz o menor
    for (int i = 0; i < size; i++)
    {
        for (int j = get_digit(v[i], current_digit); j < 10; j++)
        {
            count[j]++;
        }
    }

    // Reordena la lista
    int *result = new int[size];

    for (int i = size - 1; i >= 0; i--)
    {
        result[--count[get_digit(v[i], current_digit)]] = v[i];
    }

    for (int i = 0; i < size; i++)
    {
        v[i] = result[i];
    }

    delete[] result;

    // Ordena con el siguiente dígito (salvo que ya se hayan ordenado todos los dígitos)
    if (current_digit < max_digits)
        radix_sort(v, size, current_digit + 1, max_digits);
}

/**
 * @brief Ordena un vector utilizando radix sort. El algoritmo obtiene el número
 * con el mayor número de dígitos. Para ordenarlo, recorre varias veces el
 * vector y los ordena dígito a dígito.
 *
 * @param v Vector de entradas
 * @param size Tamaño del vector
 */
void radix_sort(int v[], int size)
{ 
    int digits = INT_MIN,
        i_digits;

    bool hayNeg = false;
    int maxNeg = 0;

    // Busca, si lo hay, el mayor número negativo
    for (int i = 0; i < size; i++) 
    {
        if (v[i] < maxNeg) 
        {
            if (!hayNeg) hayNeg = true;
            maxNeg = v[i];
        }
    }
    maxNeg = -maxNeg;
    
    // Si hay negativos, suma el valor del negativo más grande
    // Busca el número con el mayor número de números significativos
    for (int i = 0; i < size; i++) 
    {
        if (hayNeg) 
            v[i] +=maxNeg;

        i_digits = get_number_of_digits(v[i]);
        if (digits < i_digits)
            digits = i_digits;
    }   

    // Llamada a función recursiva
    radix_sort(v, size, 1, digits);

    // Si hay negativos, restar el valor del negativo más grande
    if (hayNeg) {
        for (int i = 0; i < size; i++)
            v[i] -=maxNeg;
    }
}

/******************************************************************************/

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

/**
 * @brief Imprime el modo de escribir correctamente los argumentos 
 * a la hora de ejecutar el programa.
 * 
 * @return EXIT_FAILURE
*/
bool argumentos_incorrectos()
{
    cerr << "Argumentos incorrectos.\nModos de ejecución:" << endl;
    cerr << "  -  ./practica1 --single [tipo (0-entero, 1-decimal, 2-string)] [ruta fichero origen] [ruta fichero resultados]" << endl;
    cerr << "  -  ./practica1 --multiple [tipo (0-entero, 1-decimal, 2-string)] [ruta directorio origen] [num ficheros a ordenar] [ruta fichero resultados]" << endl;
    return EXIT_FAILURE;
}

/**
 * @brief Imprime que un algoritmo de ordenación ha falldo
 * 
 * @return False
*/
bool algoritmo_fallado(string algoritmo) 
{
    cerr << "El algoritmo de ordenación " << algoritmo << " ha fallado." << endl;
    return false;
}

/**
 * @brief Muestra por pantalla un vector de entradas.
 *
 * El formato es:
 *
 * `[entrada_0 entrada_1 ... entrada_(size-1)]\\n`
 *
 * @tparam T Tipo de dato debe tener el operador << definido
 * @param v Vector de entradas
 * @param size Tamaño del vector
 * @param f Instancia en la que escribir
 */
template <typename T>
void print_array(T v[], int size, ofstream& f)
{
    f << "[";
    for (int i = 0; i < size; i++)
    {
        f << v[i] << " ";
    }
    f << "]\n";
}

/**
 * @brief Comprueba que un vector este ordenado.
 *
 * @tparam T Tipo de dato debe tener definidos los operadores de comparación.
 * @param v Vector de entradas
 * @param size Tamaño del vector
 * @return Verdadero sólo si el vector está ordenado
 */
template <typename T>
bool in_order(T v[], int size)
{
    if (size == 1)
        return true;

    for (int i = 0; i < size - 1; i++)
        if (v[i] > v[i + 1])
            return false;

    return true;
}

/**
 * @brief Cuenta el número de línas de un fichero
 * 
 * @param data Dirección del fichero donde están los datos
 * @return Número de líneas
*/
int countLines(string data) {
    ifstream f;
    if (!abrir_fichero_in(data, f))
        return 0;

    string line;
    int count = 0;

    while(getline(f, line)) {
        count++;
    }

    f.close();
    if (count == 0)
        cerr << "El fichero no tiene elementos" << endl;
    return count;
}

/**
 * @brief Rellena un vector de strings con los datos de cada fila de un fichero
 * 
 * @param v Vector de entrada
 * @param size Tamaño del vector
 * @param data Dirección del fichero
 * @return True si se ha llenado correctamente
*/
bool fill(string v[], int size, string data) {
    ifstream f;
    if (!abrir_fichero_in(data, f))
        return false;

    string line;
    int index = 0;
    while(getline(f, line)) 
    {
        if (index < size)
        {
            v[index] = line;
            index++;
        }
        else
            return false;
    }

    f.close();
    return true;
}

/**
 * @brief Rellena un vector de enteros con los datos de cada fila de un fichero
 * 
 * @param v Vector de entrada
 * @param size Tamaño del vector
 * @param data Dirección del fichero
 * @return True si se ha llenado correctamente
*/
bool fill(int v[], int size, string data) {
    string *aux = new string[size];
    if (!fill(aux, size, data))
        return false;

    try {
        for (int i = 0; i < size; i++) {
            v[i] = stoi(aux[i]);
        }
    } 
    catch (exception &err)
    {
        cerr << "El fichero contiene elementos no enteros" << endl;
        return false;
    }

    delete[] aux;  
    return true;
}

/**
 * @brief Rellena un vector de reales con los datos de cada fila de un fichero
 * 
 * @param v Vector de entrada
 * @param size Tamaño del vector
 * @param data Dirección del fichero
 * @return True si se ha llenado correctamente
*/
bool fill(double v[], int size, string data) {
    string *aux = new string[size];
    if (!fill(aux, size, data))
        return false;

    try {
        for (int i = 0; i < size; i++) {
            v[i] = stod(aux[i]);
        }
    } 
    catch (exception &err)
    {
        cerr << "El fichero contiene elementos no decimales" << endl;
        return false;
    }

    delete[] aux;  
    return true;
}

/**
 * @brief Ejecuta un algoritmo de ordenación, mide su tiempo de ejecución 
 * y comprueba si está correctamente ordenado.
 *
 * @tparam T Tipo de dato debe tener el operador << definido
 * @param func Función de ordenamiento a utilizar
 * @param v Vector de entradas
 * @param size Tamaño del vector
 * @param time Variable donde guardar el tiempo
 * @return True si está ordenado
 */
template <typename T>
bool sort_test(void (*func) (T[], int), T v[], int size, double& time) 
{
    struct timeval ti, tf;

    gettimeofday(&ti, NULL);
    func(v, size);
    gettimeofday(&tf, NULL);

    time = (tf.tv_sec - ti.tv_sec)*1000 + (tf.tv_usec - ti.tv_usec)/1000.0;
    if (!in_order(v, size))
        return false;
    return true;
}

/**
 * @brief Ejecuta cada uno de los tres algoritmos de ordenación para un mismo vector
 * 
 * @tparam T Tipo de dato debe tener el operador << definido
 * @param v Vector de entradas
 * @param size Tamaño del vector
 * @param f Instancia del fichero de escritura
 * @return True si está ordenado con todos los algoritmos
*/
template <typename T>
bool testSingle(T v[], int size, string data, ofstream& f)
{
    double time;

    if (!fill(v, size, data) || !sort_test(quick_sort, v, size, time))
        return algoritmo_fallado("QuickSort");
    f << time << ",";

    if (std::is_same<T, int>::value) 
    {
        if (!fill(v, size, data) || !sort_test(radix_sort, (int*)v, size, time))
            return algoritmo_fallado("RadixSort");
        f << time << ",";
    }

    if (!fill(v, size, data) || !sort_test(heap_sort, v, size, time))
            return algoritmo_fallado("HeapSort");
    f << time << endl;

    return true;
}

/**
 * @brief Dependiendo del tipo de datos que contiene un fichero, 
 * ejecuta algoritmos de ordenación para un vector del mismo tipo 
 * que los datos del fichero.
 * 
 * @param type Tipo de dato
 * @param fOrigen Ubicación del fichero de lectura
 * @param fDestino Ubicación del fichero de escritura
 * @return True si los ficheros se han abierto y los vectores han quedado ordenados
*/
bool execSingle(int type, string fOrigen, string fDestino) 
{
    if (type < 0 || type > 2)
        return false;

    ofstream f;
    if(!abrir_fichero_out(fDestino, f))
        return false;

    bool ok = true;

    int size = countLines(fOrigen);
    if (size == 0) 
        ok = false;
    
    if (ok && type == SORT_TYPE::ENTERO)
    {
        f << "QuickSort,RadixSort,HeapSort" << endl;
        int *v = new int[size];
        ok = testSingle(v, size, fOrigen, f);
        delete[] v;
    }
    else if (ok && type == SORT_TYPE::DECIMAL) 
    {
        f << "QuickSort,HeapSort" << endl;
        double *v = new double[size];
        ok = testSingle(v, size, fOrigen, f);
        delete[] v;
    }
    else if (ok && type == SORT_TYPE::CADENA) 
    {
        f << "QuickSort,HeapSort" << endl;
        string *v = new string[size];
        ok = testSingle(v, size, fOrigen, f);
        delete[] v;
    }

    f.close();
    return ok;
}

/**
 * @brief Dependiendo del tipo de datos que contienen los ficheros, 
 * ejecuta algoritmos de ordenación para vectores del mismo tipo 
 * que los datos de los ficheros.
 * Los nombres de los ficheros del directorio deben ir de 1 a n.
 * 
 * @param type Tipo de dato
 * @param dirOrigen Ubicación del directorio con los ficheros de lectura
 * @param n Número de ficheros a leer.
 * @param fDestino Ubicación del fichero de escritura
 * @return True si los ficheros se han abierto y los vectores han quedado ordenados
*/
bool execMultiple(int type, string dirOrigen, int n, string fDestino) 
{
    if (type < 0 || type > 2)
        return false;

    ofstream f;
    if(!abrir_fichero_out(fDestino, f))
        return false;

    bool ok = true;
    string file;

    for (int i = 1; i <= n; i++) {
        file = dirOrigen+"/"+to_string(i)+".txt";
        int size = countLines(file);
        if (size == 0) 
            ok = false;

        if (!ok)
            break;

        if (type == SORT_TYPE::ENTERO)
        {
            if (i == 1)
                f << "QuickSort,RadixSort,HeapSort" << endl;
            int *v = new int[size];
            ok = testSingle(v, size, file, f);
            delete[] v;
        }
        else if (type == SORT_TYPE::DECIMAL) 
        {
            if (i == 1)
                f << "QuickSort,HeapSort" << endl;
            double *v = new double[size];
            ok = testSingle(v, size, file, f);
            delete[] v;
        }
        else if (type == SORT_TYPE::CADENA) 
        {
            if (i == 1)
                f << "QuickSort,HeapSort" << endl;
            string *v = new string[size];
            ok = testSingle(v, size, file, f);
            delete[] v;
        }
    }

    f.close();
    return ok;
}

int main(int argc, char *argv[])
{
    if (argc == 5 && (string)argv[1] == "--single")
    {
        if (!execSingle(stoi(argv[2]), (string)argv[3], (string)argv[4]))
            return EXIT_FAILURE;
    }
    else if (argc == 6 && (string)argv[1] == "--multiple")
    {
        execMultiple(stoi(argv[2]), (string)argv[3], stoi(argv[4]), (string)argv[5]);
    }
    else 
        return argumentos_incorrectos();

    cout << "Fichero de resultados creado." << endl;
    return EXIT_SUCCESS;
}
