#include "contacto.h"

// DECLARACIONES DE FUNCIONES DE INTERFAZ

/*
    Pre: true
    Post: Dada una cadena nom, una cadena dir, y un entero tel, se obtiene un contacto con
            nombre nom, dirección dir, y número de teléfono tel
*/

void crear(string nom, string dir, int tel, contacto& c){
    c.nombre = nom;
    c.direccion = dir;
    c.telefono = tel;
}

/*
    Pre: true
    Post: Dado un contacto c, se obtiene la cadena correspondiente al nombre del 
            contacto c
*/

string nombre(contacto c){
    return c.nombre;
}

/*
    Pre: true
    Post: Dado un contacto c, se obtiene la cadena correspondiente a la
            dirección del contacto c
*/

string direccion(contacto c){
    return c.direccion;
}

/*
    Pre: true
    Post: Dado un contacto c, se obtiene el entero cadena correspondiente
            al telefono del contacto c
*/

int telefono(contacto c){
    return c.telefono;
}

/*
    Pre: true
    Post: Dado un contacto c1 y c2, devuelve verdadero si y solo si el nombre de ambos
        contactos son iguales
*/

bool operator==(const contacto& c1, const contacto& c2){
    return nombre(c1) == nombre(c2); 
}