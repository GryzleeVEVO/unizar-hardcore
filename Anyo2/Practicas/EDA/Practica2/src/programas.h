/*
    Estructura de datos y algoritmos
    Práctica 2

    Autores: Dorian Boleslaw Wozniak        817570
             Jorge Pérez Liesa              821011

    Descripción: Archivo de especificación de un tipo programa, que almacena el
    nombre, lenguaje, fecha de creación y descripción de un programa, con las
    operaciones de acceso y comparación especificadas a continuación
*/

#ifndef PROGRAMAS_C
#define PROGRAMAS_C

#include "fechas.h"
#include <iostream>

using namespace std;

// ESPECIFICACION

/*
    Género programas: Los valores del TAD programas representan la información 
                        de que disponemos sobre un programa informático
*/

// PREDECLARACIONES

struct Programa;

/*
    crear: cadena nom, fecha cre, cadena leng, cadena desc -> programa
    
    Crea un programa de nombre nom, con fecha de creación cre, codificado en 
    lenguaje leng, y con descripción desc
*/

void crear(Programa& p, const string& nom, const Fecha& cre, const string& leng, const string& desc);

/*
    suNombre: programa p -> cadena
    
    Devuelve el nombre del programa p
*/

string suNombre(const Programa& p);

/*
    suFecha: programa p -> fecha

    Devuelve la fecha del programa p
*/

Fecha suFecha(const Programa& p);

/*
    suLenguaje: programa p -> fecha

    Devuelve la fecha del programa p
*/

string suLenguaje(const Programa& p); 

/*
    suDescripción: programa p -> cadena

    Devuelve la descripción del programa p
*/

string suDescripcion(const Programa& p);


/*
    operador ==: programa p1, programa p2 -> booleano

    Devuelve verdad si y solo si los programas p1 y p2 son iguales
*/

bool operator==(const Programa& p1, const Programa& p2);

struct Programa{
    friend void crear(Programa& p, const string& nom, const Fecha& cre, const string& leng, string& desc);
    friend string suNombre(const Programa& p);
    friend Fecha suFecha(const Programa& p);
    friend string suLenguaje(const Programa& p);
    friend string suDescripcion(const Programa& p);
    friend bool operator==(const Programa& p1, const Programa& p2);

    private:
        string nombre;
        string lenguaje;
        string descripcion;
        Fecha creacion;
};

#endif