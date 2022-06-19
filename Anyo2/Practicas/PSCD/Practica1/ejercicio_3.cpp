/*

Programación de sistemas concurrentes y distribuidos
Practica 1
Ejercicio 3

Autor: Wozniak, Dorian Boleslaw
NIP: 817570

*/

#include <iostream>
#include <thread>
#include <string>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <cmath>

using namespace std;


/*

Pre: Existen inf y sup
Post: Devuelve un valor real aleatorio en el intervalo [inf, sup]

Nota: rand() solo genera valores enteros entre 0 y RAND_MAX.
    rand() tiene cierto sesgo por la forma en la que obtiene sus valores,
    por lo que los resultados no estan distribuidos uniformente.
    Ejecutar varias veces el programa en un segundo dará los mismos
    resultados por la forma en la que se genera la semilla del generador de
    numeros aleatorios.

*/


double aleatorioReal(double inf, double sup){
    double real = rand();
    return inf + real / (RAND_MAX / (sup - inf));
}

/*

Pre: Existe datos[], #datos = NUM_DATOS
Post: Los NUM_DATOS elementos de <datos> son numeros aleatorios reales entre -1000 y 1000

Nota: Los límites inferiores y superiores para la generación de numeros aleatorios son
    arbitrarios. 

*/

void initVectores(double datos[], const int NUM_DATOS){
    for(int i = 0; i < NUM_DATOS; i++){
        datos[i] = aleatorioReal(-1000, 1000);
    }
}

/*

Pre: Los NUM_DATOS elementos del vector datos[] son numeros reales
Post: Obtiene la media del conjunto de elementos de <datos>, donde 

    media = (Σα ∈ [0, NUM_DATOS].datos[α]) / NUM_DATOS

*/

void calcMedia(double datos[], const int NUM_DATOS, double& media, bool& mediaObtenida){
    media = 0;

    for(int i = 0; i < NUM_DATOS; i++){
        media = media + datos[i];
    }

    media = media / NUM_DATOS;
    mediaObtenida = true; // Necesaria para poder iniciar la ejecución de calcDesv()
}

/*

Pre: Los NUM_DATOS elementos del vector datos[] son numeros reales
Post: Obtiene los valores máximo y mínimo del conjunto de elementos de <datos>, donde

    max = MAX α ∈ [0, NUM_DATOS].datos[α]
    min = MIN α ∈ [0, NUM_DATOS].datos[α]

*/

void calcMinMax(double datos[], const int NUM_DATOS, double& min, double& max){
    max = datos[0], min = datos[0];
    
    for(int i = 1; i < NUM_DATOS; i++){
        if(datos[i] > max){
            max = datos[i];
        }

        else if(datos[i] < min){
            min = datos[i];
        }
    }
}

/*

Pre: Los NUM_DATOS elementos del vector datos[] son numeros reales
Post: Obtiene la desviación típica del conjunto de elementos de <datos>, donde

    dt = √(Σα ∈ [0, NUM_DATOS].(datos[α] - media)² / NUM_DATOS)

Nota: La función debe esperar hasta que calcMedia() acabe de calcular la media para
    poder iniciar su ejecución.

*/

void calcDesv(double datos[], const int NUM_DATOS, double& dt, double& media, bool& mediaObtenida){
    while(!mediaObtenida){} // Espera activa

    dt = 0;

    for(int i = 0; i < NUM_DATOS; i++){
        dt = dt + pow(datos[i] - media, 2);
    }

    dt = sqrt(dt / NUM_DATOS);
}

int main(){
    srand(time(NULL));
    const int PROCS = 3;
    const int NUM_DATOS = 100;

    double datos[NUM_DATOS];
    double media, min, max, dt;
    bool mediaObtenida = false;

    initVectores(datos, NUM_DATOS);

    thread th[PROCS];
    th[0] = thread(&calcMedia, datos, NUM_DATOS, ref(media), ref(mediaObtenida));
    th[1] = thread(&calcMinMax, datos, NUM_DATOS, ref(min), ref(max));
    th[2] = thread(&calcDesv, datos, NUM_DATOS, ref(dt), ref(media), ref(mediaObtenida));

    for(int i = 0; i < PROCS; i++){
        th[i].join();
    }

    cout << "media: " + to_string(media) + '\n';
    cout << "max: " + to_string(max) + '\n';
    cout << "min: " + to_string(min) + '\n';
    cout << "sigma: " + to_string(dt) + '\n'; 


    return 0;

}

