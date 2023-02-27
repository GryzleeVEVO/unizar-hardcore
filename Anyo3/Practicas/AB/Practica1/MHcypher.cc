#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

/**
 * @brief Tipo de dato para almacenar la clave privada de un sistema
 * Merckle-Hellman. Contiene los datos necesarios para desencriptar mensajes.
 */
struct MHPrivateKey
{
    /**
     * Bloque de enteros positivos en orden estrictamente creciente.
     * El tamaño determina que alfabetos se pueden encriptar.
     */
    vector<int> e;

    // Número aleatorio mayor que la suma de todos los elementos del bloque
    int N;

    int w;    // Número aleatorio grande, menor que N y coprimo con el
    int wInv; // Inversa del número w
};

/**
 * @brief Tipo de dato para almacenar la clave pública de un sistema
 * Merckle-Hellman. Contiene el bloque de enteros utiles para encroipar
 * mensajes.
 */
typedef vector<int> MHPublicKey;

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

int inverse(int a, int modulo)
{
    for (int i = 1; i < modulo; i++)
    {
        if (((a % modulo) * (i % modulo)) % modulo == 1)
        {
            return i;
        }
    }

    return -1;
}

MHPrivateKey generarClavePrivada(vector<int> e, int N, int w)
{
    int eSum = 0, prev = 0;

    for (auto i : e)
    {
        eSum += i;

        if (i < 1)
        {
            cerr << "El bloque contiene negativos" << endl;
            exit(1);
        }

        if (prev && prev > i)
        {
            cerr << "El bloque no es ascendente: " << prev << " > " << i << endl;
            exit(1);
        }
        prev = i;
    }

    if (eSum > N)
    {
        cerr << "N es demasiado pequeño: sum = " << eSum << " > " << N << endl;
        exit(1);
    }

    int mcd = gcd(N, w);

    if (mcd != 1)
    {
        cerr << "w no es coprimo de N: mcd = " << mcd << endl;
        exit(1);
    }

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
 * @return MHPublicKey Clave pública
 */
MHPublicKey generarClavePublica(MHPrivateKey priv)
{
    MHPublicKey result(priv.e.size());

    // Calcula el nuevo vector de elementos a partir de la clave privada
    for (int i = 0; i < result.size(); i++)
    {
        result[i] = (priv.w * priv.e[i]) % priv.N;
    }

    return result;
}

/**
 * @brief Cifra un mensaje utilizando una clave pública válida
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
    /* Esta parte sobra si se asume que usa codificación ASCII (la "mochila" deberá ser de al menos 8 elementos) */
    vector<int> toCypher(msg.length(), 0);

    for (int i = 0; i < msg.length(); i++)
    {
        toCypher[i] = msg[i] - 'A' + 1;
    }

    vector<int> result(msg.length(), 0);

    // Para cada elemento del mensaje
    for (int i = 0; i < msg.length(); i++)
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
 * @param priv
 * @param cypher
 * @return string
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
            }
        }
    }

    string result;

    for (int i = 0; i < decyphered.size(); i++)
    {
        result += decyphered[i] + 'A' - 1;
    }

    return result;
}

int main(int argc, char **argv)
{
    /* --- Argumentos --- */

    if (argc < 4)
        return 1;

    vector<int> e;

    for (int i = 1; i < argc - 3; i++)
    {
        e.push_back(atoi(argv[i]));
    }

    int N = atoi(argv[argc - 3]);
    int w = atoi(argv[argc - 2]);

    string msg = argv[argc - 1];

    MHPrivateKey priv = generarClavePrivada(e, N, w);
    MHPublicKey pub = generarClavePublica(priv);
    vector<int> cypher = cifrar(pub, msg);

    string s2 = descifrar(priv, cypher);

    cout << s2 << endl;

    return 0;
}