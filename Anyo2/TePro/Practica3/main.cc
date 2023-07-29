#include "practica3.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
	Contenedor<Carga>	contenedor_estandar(4);
	Contenedor<Toxico>	contenedor_toxico(3);
	Contenedor<SerVivo>	contenedor_servivo(2);

	Producto longaniza_graus("Longaniza de Graus",0.5,0.8);
	contenedor_estandar.guardar(longaniza_graus);
	Producto adoquines_zaragoza("Adoquines de Zaragoza",0.1,5.0);
	contenedor_estandar.guardar(adoquines_zaragoza);

	Toxico discos_melendi("Discos de Melendi",1,10);
	// Esto no deberia compilar
	// contenedor_estandar.guardar(discos_melendi);
	contenedor_toxico.guardar(discos_melendi);

	Camion camion(10);

	SerVivo elvis_presley("Elvis Presley",0.1,100);
	// Esto no deberia compilar
	// camion.guardar(elvis_presley);
	contenedor_servivo.guardar(elvis_presley);

	if (!camion.guardar(contenedor_estandar))
		cout << "Camion lleno con contenedor estandar" << endl;
	if (!camion.guardar(contenedor_toxico))
		cout << "Camion lleno con contenedor toxico" << endl;
	if (!camion.guardar(contenedor_servivo))
		cout << "Camion lleno con contenedor de seres vivos" << endl;

	Producto apuntes_tepro("Apuntes de Tecnologia de Programacion",1,0.1);
	if (!camion.guardar(apuntes_tepro))
		cout << "Camion lleno con apuntes de tepro" << endl;

	Producto trenzas_almudevar("Trenzas de Almudevar",0.5,0.8);
	if (!camion.guardar(trenzas_almudevar))
		cout << "Camion lleno con Trenzas de Almudevar" << endl;

	cout << endl;
	cout << camion << endl;

/*
La salida del programa hasta aquí deberia ser parecida a lo siguiente:

> ./main
Camion lleno con Trenzas de Almudevar

Camion [10 m3] [115.9 kg]
  Contenedor [4 m3] [5.8 kg] de Carga Estandar
    Longaniza de Graus [0.5 m3] [0.8 kg]
    Adoquines de Zaragoza [0.1 m3] [5 kg]
  Contenedor [3 m3] [10 kg] de Productos Toxicos
    Discos de Melendi [1 m3] [10 kg]
  Contenedor [2 m3] [100 kg] de Seres Vivos
    Elvis Presley [0.1 m3] [100 kg]
  Apuntes de Tecnologia de Programacion [1 m3] [0.1 kg]

*/

	Contenedor<Carga>	otro_contenedor_estandar(1);
	Contenedor<Toxico>	otro_contenedor_toxico(1);
	Contenedor<SerVivo>	otro_contenedor_servivo(1);
	contenedor_estandar.guardar(otro_contenedor_estandar);
	contenedor_estandar.guardar(otro_contenedor_toxico);
	contenedor_estandar.guardar(otro_contenedor_servivo);
	// Estas lineas no deberian compilar
	//contenedor_estandar.guardar(camion);
	//contenedor_toxico.guardar(otro_contenedor_toxico);
	//contenedor_servivo.guardar(otro_contenedor_servivo);
}

