/*
    Programación de sistemas concurrentes y distribuidos
    Practica 2
    Ejercicio 2
    Autor: Wozniak, Dorian Boleslaw
    NIP: 817570

    Programa: Obtiene del usuario por entrada estandar un número del 1 al 25.
    A continuación, devuelve al usuario por salida estandar el número de apariciones
    de la sección del vector evaluada por los procesos buscadores en la que aparece más
    veces, ademas de los indices inferior y superior de dicha sección 
*/

#include <iostream>
#include <fstream>
#include <thread>
#include <atomic>

using namespace std;

#include <iostream>
#include <fstream>
#include <thread>

using namespace std;

// CONSTANTES GLOBALES

const int N = 512;
const int N_BUSC = 8;
const int SECCION = N / N_BUSC;
using VectInt = int[N];

// VARIABLES GLOBALES

bool iniciarBusqueda = false;
bool falloLectura = false;
bool busquedaTerminada[N_BUSC];

/*
    Pre: 0 ≤ i ≤ d ≤ N - 1
    Post: (Num α ∈ [i,d].value = v[α] > maxVeces) -> (maxVeces = Num α ∈ [i,d].value = v[α]
    ^ indMin = i ^ indMax = i)

    Obtiene el número de apariciones de un vector de enteros en un rango [i,d] válido y
    si es mayor que cualquier sección anterior evaluada anteriormente sustituye
    maxVeces por este valor e indMin e indMax por los indices inferior y superior

*/

void search(const VectInt v, const int i, const int d,
            const int value, int& maxVeces, int& indMin,
            int& indMax, atomic_flag& tas){
                
    int cuenta = 0; 

    for(int a = i; a <= d; a++){
        if(v[a] == value){
            cuenta++;
        }
    }

    while(tas.test_and_set()){}
    if(cuenta > maxVeces){
        maxVeces = cuenta;
        indMin = i;
        indMax = d;
    }
    tas.clear();
}

/*
    Pre: 0 ≤ i ≤ d ≤ N - 1, 0 ≤ ident ≤ N_BUSC ^ falloLectura = false
    Post: Post: ((Num α ∈ [i,d].value = v[α] > maxVeces) -> (maxVeces = Num α ∈ [i,d].value = v[α]
    ^ indMin = i ^ indMax = i)) ^ busquedaTerminada[ident] = true

    Proceso concurrente que gestiona la interacción con otros procesos e invoca el proceso
    de busqueda en el vector si se ha leido correctamente el fichero "datos.txt"
*/

void buscar(const VectInt v, const int i, const int d,
            const int value, int& maxVeces, int& indMin,
            int& indMax, atomic_flag& tas, int ident){
    while(!iniciarBusqueda){}   // Espera activa
    
    if(!falloLectura){  // Si no ha habido ningún problema
        search(v, i, d, value, maxVeces, indMin, indMax, tas); // Busca el n° de ocurrencias
        busquedaTerminada[ident] = true;    // Informa que ha acabado
    }
}

/*
    Pre: Existe "datos.txt"
    Post: Si existe "datos.txt", llena sus posiciones con enteros encontrados en el fichero
    hasta encontrar el final del fichero o exceder las N posiciones del vector.

    Si no se ha abierto el fichero correctamente o se ha agotado las entradas del fichero sin
    inicializar el vector en todos sus campos, devuelve falso. En caso contrario devuelve verdadero
*/

bool leer(VectInt v){
    ifstream f;
    f.open("datos.txt");

    if(f.is_open()){
        bool lleno = false;
        int i = 0;

        // Obtiene los datos del vector
        while(!f.eof() && i < N){
            f >> v[i];
            i++;
            if(i >= N){
                lleno = true;
            }
        }

        f.close();
        return lleno;
    }

    else{
        return false;
    }
}

/*
    Pre: true
    Post: maxVeces es el numéro de apariciones de la sección analizada del vector en la que
    mas ha aparecido, e indMin e indMax sus indices inferior y superior

    Proceso coordinador, quien una vez leido el fichero y cargado sus datos en el vector
    da la orden de comenzar a buscar a los procesos buscadores y espera hasta que acaben.
    Una vez han acabado, muestra por salida estandar el número de apariciones de la sección analizada 
    del vector en la que mas ha aparecido, y sus indices inferior y superior

    Si ha habido un errror al leer el fichero, alternativamente muestra por salida estandar
    un mensaje de errror y avisa a los procesos buscadores de no buscar 
*/


void coord(VectInt v, const int value, int& maxVeces, int& indMin, int& indMax){
    if(leer(v)){    // Si consigue abrir el fichero
        int total = 0;
            
        for(int i = 0; i < N_BUSC; i++){
            busquedaTerminada[i] = false;   // Rellena vector comprobacion de search con falsos
        }

        iniciarBusqueda = true;             // Da la orden de comenzar a buscar

        for(int i = 0; i < N_BUSC; i++){
            while(!busquedaTerminada[i]){}  // Espera activa hasta que todos terminen
        }

        cout << "Mayor numero de ocurrencias de " + to_string(value) + 
                " en un sector del vector: " + to_string(maxVeces) + "\n";
        cout << "Indice inferior: " + to_string(indMin) + "\n";
        cout << "Indice superior: " + to_string(indMax) + "\n";
    }

    else{
        cerr << "Error de lectura";
        falloLectura = false;
        iniciarBusqueda = true;
    }
}

int main(){
    VectInt v;
    thread th[N_BUSC + 1];
    atomic_flag tas = ATOMIC_FLAG_INIT;
    int maxVeces = 0, indMin = 0, indMax = 0, value = 0, i_aux = 0, d_aux = 0;

    while(value < 1 || value > 25){ 
        cout << "Introduzca un valor entero a buscar en el rango [1, 25]: ";
        cin >> value;
    }

    // Inicia los procesos de busqueda

    for(int i = 0; i < N_BUSC; i++){
        d_aux = d_aux + SECCION;

        if(d_aux >= N){
            d_aux = N - 1;
        }

        th[i] = thread(buscar, v, i_aux, d_aux, value, ref(maxVeces), ref(indMin), ref(indMax), ref(tas), i);
        i_aux = d_aux + 1;
    }

    th[N_BUSC] = thread(coord, v, value, ref(maxVeces), ref(indMin), ref(indMax)); // Inicia proceso coordinador
 
    for(int i = 0; i <= N_BUSC ; i++){            // Espera hasta que terminen procesos
        th[i].join();
    }

    return 0;
}