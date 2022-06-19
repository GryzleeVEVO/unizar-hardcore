/*

Programación de sistemas concurrentes y distribuidos
Practica 1
Ejercicio 2

Autor: Wozniak, Dorian Boleslaw
NIP: 817570

*/

#include <iostream>
#include <thread>
#include <string>
#include <chrono>
#include <ctime>
#include <cstdlib>

using namespace std;

/*

Pre: retardo tiene un valor entre 100 y 300
    veces tiene un valor entre 5 y 15
    id tiene un valor entre 0 y 9

Post: Espera <retardo> milisegundos y escribe por salida estandar "Soy <id>".
    Este proceso lo realiza <veces> veces.

Nota: Se ejecutan 10 instancias concurrentes del mismo proceso. El programa
    al ejecutarse sleep_for() saltará a otra instancia preparada para escribir.
    Se puede notar al final como la ejecución se ralentiza al cerrarse procesos
    pues no tiene procesos preparados que atender.

*/

void saludo(int id, int retardo, int veces){
    string s = to_string(id);

    for(int i = 0; i < veces; i++){
        this_thread::sleep_for(chrono::milliseconds(retardo));
        cout << "Soy " + s + '\n';
    }
}

/*

Pre: Existen inf y sup
Post: Genera un valor entero aleatorio en el intervalo [inf, sup]

*/

int aleatorio(int inf, int sup){
    return inf + (rand() % (sup - inf + 1));
}

int main(){
    srand(time(NULL));
    const int N = 10;
    thread th[N];

// Inicia los procesos con un retardo y numero de repeticiones aleatorio

    for(int i = 0; i < N; i++){
        int retardo = aleatorio(100, 300);
        int veces = aleatorio(5, 15);

        th[i] = thread(&saludo, i + 1, retardo, veces);
    }

// Espera hasta que todos los procesos se unan

    for(int i = 0; i < N; i++){
        th[i].join();
    }

    return 0;
}