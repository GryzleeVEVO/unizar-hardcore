#include "ControlCabinas.hpp"

/*
	Pre: dentro >= 0
	Post: Inicializamos la variable dentro a 0 (ya qe inicialmente no tenemos a nadie dentro de las cabinas)
			y las variables booleanas limpiando y cabina de N_CAB componentes estan inicializadas a false
*/

ControlCabinas::ControlCabinas(){
	dentro = 0;
	cabina[N_CAB] = {false};
	limpiando = false;
}

/*
	Pre: true
	Post: Inicializamos el destructor de ControlCabinas
*/

ControlCabinas::~ControlCabinas(){}

/*
	Pre: true
	Post: El cliente, un número N_TIMES_USER veces, entrará al locutorio una vez haya una
            cabina vacía y no se está limpiando las cabinas. En el locutorio, ocupará
            una cabina vacía y hablará por telefono un tiempo aleatorio
*/

void ControlCabinas::entraUsuario(int i, int j ,int& cab){
	unique_lock<mutex> lck(mtx);

	// Mientras se está limpiando y dentro haya mas de 4 personas
	while(!(!limpiando && dentro < N_CAB)){
		acceder.wait(lck);										// que usuario acceda se queda esperando
	}

	bool encontrado = false;
	int k = 0;

	// Mientras el usuario no encuentre cabina vacia
	while(!encontrado){

		// Si usuario encuentra cabina vacia
		if(cabina[k] == false){
			cabina[k] = true;
			cab = k;
			dentro++;
			encontrado = true;		
		}	

		// Sino buscamos siguiente cabina
		else{
			k++;		
		}
	}

	ADD_EVENT("USER_IN_" + to_string(i) + "," + to_string(j) + "," + to_string(cab));
}

/*
	Pre: true
	Post: Hablará por telefono un tiempo aleatorio, y saldrá indicando
            que la cabina que ocupaba, está vacía
*/


void ControlCabinas::saleUsuario(int i, int j, int cab){
	unique_lock<mutex> lck(mtx);

	// Indicamos que la cabina de la que sale el usuario la deja vacia
	cabina[cab] = false;
	dentro--;

	// Si no hay nadie dentro, indicamos que esta vacio
	if(dentro == 0){
		vacio.notify_one();
	}	

	// Si no se esta limpiando puede acceder usuario
	if(!limpiando){
		acceder.notify_one();	
	}

	ADD_EVENT("USER_OUT_" + to_string(i) + "," + to_string(j) + "," + to_string(cab));
}

/*
	Pre: true
	Post: El proceso limpieza, un número N_TIMES_CLEANING veces, esperará un tiempo de
            PER_CLEANING milisegundos, pasados los cuales indicará que va a limpiar
            las cabinas del locutorio. Una vez se hayan vaciado todas las cabinas, 
            limpiará un tiempo aleatorio.
*/

void ControlCabinas::entraLimpieza(int j){
	unique_lock<mutex> lck(mtx);

	// Indicamos que las cabinas se estan limpiando
	limpiando = true;

	// Mientras haya alguien dentro limpieza espera
	while(dentro > 0){
		vacio.wait(lck);
	}
	
	ADD_EVENT("CLEANING_IN" + "," + to_string(j));
}

/*
	Pre: true
	Post: El proceso limpieza dejara de limpiar e indicará que los ususrios pueden volver a entrar al locutorio
*/

void ControlCabinas::saleLimpieza(int j){
	unique_lock<mutex> lck(mtx);

	// Una vez ha limpiado las cabinas, se dejan de limpiar
	limpiando = false;

	// Pueden pasar todos los procesos que quieran (N_CAB procesos) de usuarios a cabina
	acceder.notify_all();
	ADD_EVENT("CLEANING_OUT" + "," + to_string(j));
}