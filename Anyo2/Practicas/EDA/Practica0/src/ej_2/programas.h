#ifndef PROGRAMAS_C
#define PROGRAMAS_C

#include "fechas.h"
#include <iostream>

using namespace std;

struct programa;

void crear(programa& p, const string nom, fecha cre, const string leng, string desc);
string suNombre(const programa p);
void suFecha(programa p, fecha& f);
string suLenguaje(const programa p);  
string suDescripcion(const programa p);
string identificador(const programa p);
string tema(const programa p);
string laFecha(const programa p, fecha f);
bool operator==(const programa& p1, const programa& p2);

struct programa{
    friend void crear(programa& p, const string nom, fecha cre, const string leng, string desc);
    friend string suNombre(const programa p);
    friend void suFecha(programa p, fecha& f);
    friend string suLenguaje(const programa p);
    friend string suDescripcion(const programa p);
    friend string identificador(programa p);
    friend string tema(programa p);
    friend string laFecha(const programa p, fecha f);
    friend bool operator==(const programa& p1, const programa& p2);

    private:
        string nombre;
        string lenguaje;
        string descripcion;
        fecha creacion;
};

#endif