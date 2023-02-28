/**
 * @file MHcypher.cc
 * @author Dorian Boleslaw Wozniak  (817570@unizar.es)
 * @author Jesus Mendoza Aranda (777524@unizar.es)
 *
 * @brief Programa
 *
 */

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

bool verbose = false;
bool ascii = true;

/**
 * @brief Tipo de dato que almacena los datos de una clave privada de un sistema
 * Merkle-Hellman. Se utliiza para desencriptar mensajes.
 */
struct MHPrivateKey
{
    // Bloque de números enteros. El siguiente es positivo y estrictamente
    // anterior al anterior.
    vector<int> e;

    // Número aleatorio mayor que la suma de todos los elementos del bloque
    int N;

    // Número aleatorio grande, menor que N y coprimo con el
    int w;

    // Inversa del número w
    int wInv;
};

/**
 * @brief Tipo de dato para almacenar la clave pública de un sistema
 * Merckle-Hellman. Consistente en un bloque de números positivos enteros
 * que permite desencriptar un mensaje.
 */
typedef vector<int> MHPublicKey;

/**
 * @brief Devuelve el mínimo común denominador de los números a y b.
 *
 * @param a Número entero
 * @param b Número entero
 * @return mcd(a,b)
 */
int gcd(int a, int b)
{
    if (!a || a == b)
        return b;
    else if (!b)
        return a;

    if (a > b)
        return gcd(a - b, b);
    else
        return gcd(a, b - a);
}

/**
 * @brief Devuelve el inverso de un número a para un módulo N si existe.
 *
 * @param a Número entero
 * @param N módulo
 * @return a⁻¹ (mod N), -1 si no existe
 */
int inverse(int a, int N)
{
    for (int i = 1; i < N; i++)
    {
        if (((a % N) * (i % N)) % N == 1)
        {
            return i;
        }
    }

    return -1;
}

/**
 * @brief Genera un nuevo MHPrivateKey a partir de los datos de una clave
 * privada. El programa también comprueba que los parámetros sean adecuados.
 *
 * @pre e[i] < e[i+1], N > sum(e_i), mcd(N,w) == 1
 *
 * @param e Bloque de enteros positivos, el siguiente estrictamente mayor que
 * el anterior
 * @param N Número aleatorio mayor que la suma de los elementos del bloque
 * @param w Número aleatorio grande que sea coprimo de N
 * @return Clave privada del sistema
 */
MHPrivateKey generarClavePrivada(vector<int> e, int N, int w)
{
    int eSum = 0, prev = 0;

    for (auto i : e)
    {
        eSum += i;

        // El sistema no funciona para enteros negativos
        if (i < 1)
        {
            cerr << "El bloque contiene negativos" << endl;
            exit(1);
        }

        // El sistema no funciona para bloques no ascendentes
        if (prev && prev > i)
        {
            cerr << "El bloque no es ascendente: " << prev << " > " << i << endl;
            exit(1);
        }
        prev = i;
    }

    // Avisa si la mochila es pequeña
    if (ascii && e.size() < 8)
        cerr << "AVISO: La mochila debería ser de 8 elementos para la codificación ASCII." << endl;
    else if (!ascii && e.size() < 5)
        cerr << "AVISO: La mochila debería ser de 5 elementos para la codificación en mayúsculas." << endl;

    // N debe ser estrictamente mayor que la suma de elementos
    if (eSum >= N)
    {
        cerr << "N es demasiado pequeño: sum = " << eSum << " > " << N << endl;
        exit(1);
    }

    // Comprueba que w sea un número positivo
    if (w < 0)
    {
        cerr << "w debe ser positivo" << endl;
        exit(1);
    }

    // Comprueba que los números sean coprimos
    int mcd = gcd(N, w);

    if (mcd != 1)
    {
        cerr << "w no es coprimo de N: mcd = " << mcd << endl;
        exit(1);
    }

    if (verbose)
    {
        cout << "Clave privada utilizada:" << endl
             << "e:";

        for (auto i : e)
        {
            cout << " " << i;
        }

        cout << endl
             << "N: " << N << ", w: " << w << endl;
    }

    // Si N y w son coprimos, w es inversible
    return {e, N, w, inverse(w, N)};
}

/**
 * @brief A partir de una clave privada, genera una clave pública que se puede
 *          utliizar para encriptar datos
 *
 * @pre N > sum(K_priv[1..n]), 1 < w < N, mcd(w,N) = 1
 * @post K_pub[i] = (K_priv[i] * e) (mod N)
 *
 * @param priv Clave privada
 * @return Clave pública del sistema
 */
MHPublicKey generarClavePublica(MHPrivateKey priv)
{
    MHPublicKey result(priv.e.size());

    // Calcula el nuevo vector de elementos a partir de la clave privada
    for (int i = 0; i < result.size(); i++)
    {
        result[i] = (priv.w * priv.e[i]) % priv.N;
    }

    if (verbose)
    {
        cout << "Clave pública generada:";

        for (auto i : result)
        {
            cout << " " << i;
        }

        cout << endl;
    }

    return result;
}

/**
 * @brief Cifra un mensaje utilizando la clave pública obtenida.
 *
 * @pre ---
 * @post cypher[i] = sum(msg[i]/2^n (mod 2) + ... + )
 *
 * @param pub Clave pública
 * @param msg Mensaje
 * @return vector<int> Mensaje encriptado
 */
vector<int> cifrar(MHPublicKey pub, string msg)
{
    vector<int> toCypher(msg.length(), 0);

    // Si el mensaje no está codificado en ASCII, lo codifica como en el ejemplo
    for (int i = 0; i < msg.length(); i++)
    {
        if (!ascii)
            toCypher[i] = msg[i] - 'A' + 1;
        else
            toCypher[i] = msg[i];
    }

    vector<int> result(msg.length(), 0);

    // Para cada elemento del mensaje
    for (int i = 0; i < toCypher.size(); i++)
    {
        for (int j = pub.size() - 1; j >= 0; j--)
        {
            if (toCypher[i] % 2)
            {
                result[i] += pub[j];
            }
            
            toCypher[i] /= 2;
        }
    }

    return result;
}

/**
 * @brief Descifra un mensaje usando una clave privada
 *
 * @param priv Clave privada
 * @param cypher Mensaje cifrado
 * @return Mensaje descifrado
 */
string descifrar(MHPrivateKey priv, vector<int> msg)
{
    vector<int> toDecypher(msg.size(), 0);
    vector<int> decyphered(msg.size(), 0);

    for (int i = 0; i < toDecypher.size(); i++)
    {
        // Transforma el bloque multiplicando cada elemento por w⁻¹ mod N
        toDecypher[i] = (msg[i] * priv.wInv) % priv.N;

        // Se resuelve el problema de la mochila de mayor a menor
        for (int j = priv.e.size() - 1; j >= 0; j--)
        {
            if (toDecypher[i] >= priv.e[j])
            {
                decyphered[i] += pow(2.0, priv.e.size() - j - 1);
                toDecypher[i] -= priv.e[j];
                cout << toDecypher[i] << endl;
            }
        }
    }

    string result;

    // Si la codificación no es ASCII, la convierte a esta
    for (int i = 0; i < decyphered.size(); i++)
    {
        if (!ascii)
            result += decyphered[i] + 'A' - 1;
        else
            result += (char) decyphered[i];
    }

    return result;
}

int main(int argc, char **argv)
{
    // Mensaje de ayuda
    if (argc == 1 && argv[1] == "-h")
    {
        cout << "MHcypher: Lista de argumentos" << endl
             << "\t-v: Mostrar claves pública y privada en la salida estandar" << endl
             << "\t-ascii: Encriptar/desencriptar asumiendo codificación ASCII (8 bits) (por defecto)" << endl
             << "\t-mayus: Encriptar/desencriptar asumiendo codificación letras mayúsculas (5 bits)" << endl
             << "\t-f FICHERO: Fichero de donde obtener los datos a encriptar/desencriptar" << endl
             << "\t-o FICHERO: Fichero donde devolver los datos a encriptar/desencriptar" << endl
             << "\t-e: Encriptar los datos" << endl
             << "\t-d: Desencriptar los datos" << endl
             << "\t-mochila_1 ... mochila_n: Elementos de la mochila utilizada para encriptar/desencriptar" << endl
             << "\tN: Número positivo mayor que la suma de los valores de la mochila" << endl
             << "\tw: Número positivo grande menor que N y coprimo con este";

        return 0;
    }

    // Ejemplo de uso
    if (argc < 4)
    {
        cerr << "Argumentos insuficientes." << endl
             << "Ejemplo de uso:" << endl
             << "\t./MHcypher [-v] [-ascii | -mayus] [-e | -d] mochila_1 "
             << "... mochila_n N w " << endl
             << "Para mas ayuda: ./MHcypher -h" << endl;
        return 1;
    }

    // Obtiene argumentos
    vector<int> e;
    bool crypt = true;

    for (int i = 1; i < argc - 2; i++)
    {
        string s = argv[i];

        if (s == "-v")
            verbose = true;
        else if (s == "-ascii")
            ascii = true;
        else if (s == "-mayus")
            ascii = false;
        else if (s == "-f")
        {
            i++;
            freopen(argv[i], "r", stdin);
        }
        else if (s == "-o")
        {
            i++;
            freopen(argv[i], "w", stdout);
        }
        else if (s == "-e")
            crypt = true;
        else if (s == "-d")
            crypt = false;

        else
        {
            while (i < argc - 2)
            {
                e.push_back(atoi(argv[i++]));
            }
        }
    }

    int N = atoi(argv[argc - 2]);
    int w = atoi(argv[argc - 1]);

    // Genera struct clave privada
    MHPrivateKey priv = generarClavePrivada(e, N, w);

    // Comportamiento para encriptar/desencriptar
    if (crypt)
    {
        string msg, input;
        while (cin >> input)
            msg += input;

        MHPublicKey pub = generarClavePublica(priv);
        vector<int> cypher = cifrar(pub, msg);

        for (auto i : cypher)
            cout << i << " ";

        cout << endl;
    }
    else
    {
        vector<int> msg;
        string input;

        while (cin >> input)
            msg.push_back(atoi(input.c_str()));

        string decypher = descifrar(priv, msg);
        cout << decypher << endl;
    }

    return 0;
}