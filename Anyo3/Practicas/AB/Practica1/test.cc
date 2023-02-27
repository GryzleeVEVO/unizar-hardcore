#include <math.h>
#include <iostream>
#include <string.h>
using namespace std;

int main(int argc, char *argv[])
{

    /* --- ARGUMENTOS --- */
    if (argc != 5)
    {
        cout << "Numero incorrecto de argumentos" << endl;
        cout << "Formato correcto:" << endl;
        cout << "funcion mochila1.mochila2.mochila3 N w mensaje" << endl;
    }


    char *ptr = strtok(argv[1], ".");
    int size = 0;
    uint32_t bag[100];
    
    for (int i = 0; ptr != NULL; i++)
    {
        bag[i] = atoi(ptr);
        ptr = strtok(NULL, ".");
        size++;
    }

    int N = atoi(argv[2]);

    int w = atoi(argv[3]);

    string mensaje = argv[4];

    int longitud = mensaje.length();

    cout << "mensaje original: " << mensaje << endl;

    /* --- CLAVE PÚBLICA  --- */
    
    uint32_t wInverse = 1;
    while (wInverse < N)
    {
        if (((w % N) * (wInverse % N)) % N == 1)
            break;
        wInverse++;
    }
    cout << "wInverse: " << wInverse << endl;

    /* --- CLAVE PÚBLICA --- */
    // Calculamos la tupla w*e(mod N)
    uint32_t publica[size];
    cout << "Publica: ";
    for (int i = 0; i < size; i++)
    {
        publica[i] = (bag[i] * w) % N;
        cout << publica[i] << " ";
    }

    /* ---- CIFRAR --*/
    // Creamos el mensaje cifrado
    cout << endl
         << "Cifrado intermedio: ";
    uint32_t cifrado[longitud];
    for (int i = 0; i < longitud; i++)
    {
        cifrado[i] = mensaje[i];
    }
    for (int i = 0; i < longitud; i++)
    {
        if (122 <= cifrado[i] <= 65)
        {
            cifrado[i] = cifrado[i] - 64;
            cout << cifrado[i] << " ";
        }
        else
        {
            cout << "Solo aceptamos caracteres entre A-z" << endl;
            exit(1);
        }
    }
    cout << endl;
    uint32_t intermedio;
    for (int i = 0; i < longitud; i++)
    {
        intermedio = cifrado[i];
        cifrado[i] = 0;
        for (int j = longitud; j >= 0; j--)
        {
            if (intermedio % 2 != 0)
            {
                cifrado[i] += publica[j];
            }
            intermedio = intermedio / 2;
        }
        cout << cifrado[i] << " ";
    }
    cout << endl;

    uint32_t descifrado[longitud];

    cout << "Cifrado * wInv: ";
    for (int i = 0; i < longitud; i++)
    {
        descifrado[i] = cifrado[i] * wInverse;
        cout << descifrado[i] << " ";
        descifrado[i] = descifrado[i] % N;
    }
    cout << endl;
    for (int i = 0; i < longitud; i++)
    {
        cout << descifrado[i] << " ";
    }
    cout << endl;
    for (int i = 0; i < longitud; i++)
    {
        intermedio = descifrado[i];
        descifrado[i] = 0;
        for (int j = longitud; j >= 0; j--)
        {
            if (intermedio % 2 != 0)
            {
                descifrado[i] += bag[j];
            }
            intermedio = intermedio / 2;
        }
        cout << descifrado[i] << " ";
    }
    cout << endl;
    for (int i = 0; i < longitud; i++)
    {
        descifrado[i] += 64;
        cout << char(descifrado[i]) << " ";
    }
}