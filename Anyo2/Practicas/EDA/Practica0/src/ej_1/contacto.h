#ifndef CONTACTO_H
#define CONTACTO_H

#include <iostream>
using namespace std;

// PREDECLARACIONES

struct contacto;

void crear(string nom, string dir, int tel, contacto& c);
string nombre(contacto c);
string direccion(contacto c);
int telefono(contacto c);
bool operator==(const contacto& c1, const contacto& c2);

// DECLARACION DEL STRUCT

struct contacto{
    friend void crear(string nom, string dir, int tel, contacto& c);
    friend string nombre(contacto c);
    friend string direccion(contacto c);
    friend int telefono(contacto c);
    friend bool operator==(const contacto& c1, const contacto& c2);


    private:
        string nombre;
        string direccion;
        int telefono;
};

#endif