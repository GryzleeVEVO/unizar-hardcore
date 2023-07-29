#ifndef CONTROLCABINAS_HPP
#define CONTROLCABINAS_HPP

#include <mutex>
#include <condition_variable>

using namespace std;

/* CONSTANTES GLOBALES */
const int N_USER = 20;
const int N_TIMES_USER = 30;
const int N_TIMES_CLEANING = 5;
const int PER_CLEANING = 100;
const int N_CAB = 4;

/* DECLARACIÓN DE MONITOR CONTROLCABINAS*/
class ControlCabinas{

	private:
		int dentro;
		bool cabina[N_CAB], limpiando;
		
		mutex mtx;
		condition_variable_any vacio, acceder;

	public:

		ControlCabinas();
		~ControlCabinas();
		
		void entraUsuario(int& cab);
		void saleUsuario(int cab);

		void entraLimpieza();
		void saleLimpieza();
};

#endif