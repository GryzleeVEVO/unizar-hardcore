/*
    Programación de sistemas concurrentes y distribuidos
    Práctica 3

    Autores:    Dorian Boleslaw Wozniak     817570
                Jaime Velasco Gimeno        816818

    Descripción: El programa simula un locutorio al que acceden 

*/

#include <iostream>
#include <thread>
#include <ctime>
#include <chrono>
#include <cstdlib>
#include "Semaphore_V4.hpp"

using namespace std;

// Compilación condicional para la generación de archivos de logging
#ifdef LOGGING_MODE
    #include <Logger.hpp>
    Logger _logger("_log_.log", 1024);
    //Logger _logger("_log_2.log", cout, 1); // si queremos echo de eventos en stdout
    string nada("");
    #define ADD_EVENT(e) {_logger.addMessage(nada+e);} //generar evento
#else
    #define ADD_EVENT(e)   // nada
#endif

// CONSTANTES GLOBALES

const int N_USER = 20;
const int N_TIMES_USER = 30;
const int N_TIMES_CLEANING = 5;
const int PER_CLEANING = 100;
const int N_CAB = 4;

/*
    Pre: true
    Post: Devuelve un número entero aleatorio en el rango [inf, sup]
*/


int aleatorio(int inf, int sup){
    return inf + (rand() % (sup - inf + 1));
}

/*
    Pre: true
    Post: Realizará las siguientes acciones:

    - Si hay procesos cliente bloqueados a la espera de que haya una cabina libre o
        a que no es esté limpiando, reducirá el contador de procesos bloqueados
        y permitirá que uno de ellos se desbloquee aumentando en 1 el semáforo entrar

    - Si no, si no hay clientes en las cabinas y el proceso limpieza está bloqueado, 
        reducirá el contador de procesos bloqueados y aumentará en 1 el semáforo
        vacio para que el proceso limpieza marque que deja de limpiar

    - En caso contrario, pasa testigo a la siguiente función esperando a entrar en zona
        de exclusión mútua
*/

void pasarTestigo(int dentro, int& cEnt, int& cVac, bool limpiar, 
                    Semaphore& sEnt, Semaphore& sVac, Semaphore& sTes){
    if(dentro < N_CAB && !limpiar && cEnt > 0){
        cEnt--;
        sEnt.signal();
    }

    else if(dentro == 0 && cVac > 0){
        cVac--;
        sVac.signal();
    }

    else{
        sTes.signal();
    }
}

/*
    Pre: true
    Post: El cliente, un número N_TIMES_USER veces, entrará al locutorio una vez haya una
            cabina vacía y no se está limpiando las cabinas. En el locutorio, ocupará
            una cabina vacía, hablará por telefono un tiempo aleatorio, y saldrá indicando
            que la cabina está vacía
*/

void cliente(int i, int& dentro, int& cEnt, int& cVac, bool& limpiar, bool cab[],
             Semaphore& sEnt, Semaphore& sVac, Semaphore& sTes){
    ADD_EVENT("USER_BEGIN_" + to_string(i));

    for(int j = 0; j < N_TIMES_USER; j++){
        // SC: Comprueba que no se está limpiando y que hay cabinas disponibles
        ADD_EVENT("USER_WAIT_" + to_string(i) + "," + to_string(j));
        sTes.wait();

        if(!(!limpiar && dentro < N_CAB)){ // Si no puede entrar, se queda bloqueado
            cEnt++;
            sTes.signal();
            sEnt.wait();
        }

        int indice = -1;

        // Busca una cabina libre y la ocupa
        for(int k = 0; indice == -1 && k < N_CAB; k++){
            if(!cab[k]){
                cab[k] = true;
                indice = k;
                dentro++;
            }
        }

        ADD_EVENT("USER_IN_" + to_string(i) + "," + to_string(j) + "," + to_string(indice));
        pasarTestigo(dentro, cEnt, cVac, limpiar, sEnt, sVac, sTes);
        // Fin SC

        // SNC: Usa la cabina
        this_thread::sleep_for(chrono::milliseconds(aleatorio(10, 40)));
        // Fin SNC

        // SC: Sale de la cabina
        sTes.wait();
        cab[indice] = false;
        dentro--;
        pasarTestigo(dentro, cEnt, cVac, limpiar, sEnt, sVac, sTes);
        // Fin SC

        // SNC: Espera un tiempo antes de volver a usar el locutorio
        ADD_EVENT("USER_OUT_" + to_string(i) + "," + to_string(j) + "," + to_string(indice));
        this_thread::sleep_for(chrono::milliseconds(aleatorio(20, 50)));
    }

    ADD_EVENT("USER_END_" + to_string(i));
}

/*
    Pre: true
    Post: El proceso limpieza, un número N_TIMES_CLEANING veces, esperará un tiempo de
            PER_CLEANING milisegundos, pasados los cuales indicará que va a limpiar
            las cabinas del locutorio. Una vez se hayan vaciado todas las cabinas, 
            limpiará un tiempo aleatorio, tras el cual indicará que se puede entrar
            de nuevo al locutorio
*/

void limpieza(int& dentro, int& cEnt, int& cVac, bool& limpiar, Semaphore& sEnt, 
                Semaphore& sVac, Semaphore& sTes){
    ADD_EVENT("CLEANING_BEGIN");

    for(int i = 0; i < N_TIMES_CLEANING; i++){
        // SNC: Espera un tiempo PER_CLEANING
        this_thread::sleep_for(chrono::milliseconds(PER_CLEANING));
        ADD_EVENT("CLEANING_WAIT" + "," + to_string(i));
        // Fin SNC

        // SC: Avisa que va a limpiar
        sTes.wait();
        limpiar = true;
        pasarTestigo(dentro, cEnt, cVac, limpiar, sEnt, sVac, sTes);
        // Fin SC

        // SC: Espera a que las cabinas esten libres
        sTes.wait();
        if(dentro != 0){ // Se queda bloqueado mientras haya cabinas ocupadas
            cVac++;
            sTes.signal();
            sVac.wait();
        }

        ADD_EVENT("CLEANING_IN" + "," + to_string(i));
        this_thread::sleep_for(chrono::milliseconds(aleatorio(80, 120))); // Limpia
        ADD_EVENT("CLEANING_OUT" + "," + to_string(i));
        limpiar = false;
        pasarTestigo(dentro, cEnt, cVac, limpiar, sEnt, sVac, sTes);
        // Fin SC
    }

    ADD_EVENT("CLEANING_END");
}



int main(){
    srand(time(NULL));

    // SEMÁFOROS
    /*
        sEnt: Gestiona los bloqueos de los procesos cliente al intentar entrar
        sVac: Gestiona el bloqueo del proceso limpieza si el locutorio no está vacío
        sTes: Semáforo testigo, gestiona las entradas a secciones críticas
    */
    Semaphore sEnt(0), sVac(0), sTes(1);

    // VARIABLES
    /*
        boolean cab[N_CAB]: Cabinas físicamente vacías/ocupadas
        boolean limpiar: Verdad si limpieza va a limpiar el locutorio

        integer dentro: Cuenta el número de cabinas ocupadas
        integer cEnt: Cuenta el número de procesos cliente bloqueados
        integer cVac: 1 si el proceso limpieza está bloqueado
    */
    bool cab[N_CAB] = {false}, limpiar = false;
    int dentro = 0, cEnt = 0, cVac = 0;

    thread th[N_USER + 1];

    // Inicia los procesos cliente

    for(int i = 0; i < N_USER; i++){
        th[i] = thread(&cliente, i, ref(dentro), ref(cEnt), ref(cVac),
                         ref(limpiar), cab, ref(sEnt), ref(sVac), ref(sTes));
    }

    // Inicia el proceso limpieza

    th[N_USER] = thread(&limpieza, ref(dentro), ref(cEnt), ref(cVac),
                         ref(limpiar), ref(sEnt), ref(sVac), ref(sTes));
    
    // Espera a que los procesos acaben

    for(int i = 0; i <= N_USER; i++){
        th[i].join();
    }
    return 0;
}