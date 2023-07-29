/*
	Programación de sistemas concurrentes y distribuidos
	Práctica 4
	Autores: Dorian Boleslaw Wozniak	817570
			 Jaime Velasco Gimeno		816818

	Descripción: Modificación de la práctica 3 en la que se utiliza
	un monitor para la gestión de la concurrencia

	El programa simula un locutorio al que acceden clientes, con una capacidad
	limitada a N_CAB cabinas y en la que se realizan limpiezas periodicamente
*/

#include <iostream>
#include <thread>
#include <chrono>

#include "ControlCabinas.hpp"

#ifdef LOGGING_MODE
    #include <Logger.hpp>
    Logger _logger("_log_.log", 1024);
    const string nada("");
    #define ADD_EVENT(e) {_logger.addMessage(nada+e);} //generar evento
#else
    #define ADD_EVENT(e)   // nada
#endif

using namespace std;

/*
    Pre: true
    Post: Devuelve un número entero aleatorio en el rango [inf, sup]
*/

int aleatorio(int inf, int sup){
    return inf + (rand() % (sup - inf + 1));
}

/*
    Pre: true
    Post: El cliente, un número N_TIMES_USER veces, entrará al locutorio una vez haya una
            cabina vacía y no se está limpiando las cabinas. En el locutorio, ocupará
            una cabina vacía, hablará por telefono un tiempo aleatorio, y saldrá indicando
            que la cabina está vacía
*/

void cliente(ControlCabinas& mCtlCab, int i){
	int cab;
	ADD_EVENT("USER_BEGIN_" + to_string(i));
	for(int j = 0; j < N_TIMES_USER; j++){
		ADD_EVENT("USER_WAIT_" + to_string(i) + "," + to_string(j));
		mCtlCab.entraUsuario(i, j, cab);
		this_thread::sleep_for(chrono::milliseconds(aleatorio(10, 40)));
		mCtlCab.saleUsuario(i, j, cab);

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

void limpieza(ControlCabinas& mCtlCab){
	ADD_EVENT("CLEANING_BEGIN");
	for(int j = 0; j < N_TIMES_CLEANING; j++){
		this_thread::sleep_for(chrono::milliseconds(PER_CLEANING));
		ADD_EVENT("CLEANING_WAIT" + "," + to_string(j));
		mCtlCab.entraLimpieza(j);
		this_thread::sleep_for(chrono::milliseconds(aleatorio(80, 120)));
		mCtlCab.saleLimpieza(j);
	}
	ADD_EVENT("CLEANING_END");
}


int main(){



	ADD_EVENT("MAIN_BEGIN");

	ControlCabinas mCtlCab; // Monitor
		
	thread th[N_USER + 1];

	for(int i = 0; i < N_USER; i++){
		th[i] = thread(&cliente, ref(mCtlCab), i);	
	}

	th[N_USER] = thread(&limpieza, ref(mCtlCab));

	for(int i = 0; i <= N_USER; i++){
		th[i].join();		
	}

	ADD_EVENT("MAIN_END");
	return 0;
}




