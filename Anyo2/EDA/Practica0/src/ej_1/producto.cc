#include "producto.h"

// DECLARACIONES DE LAS FUNCIONES DE INTERFAZ

/*  
    crear: cadena nom, entero c -> producto
    Pre: true
    Post: Dada una cadena nom y un entero c, se obtiene un producto de nombre nom
            y con cantidad de producto c
*/ 

void crear(string nombre, int cantidad, producto& p) {
    p.nombre = nombre;
    p.cantidad = cantidad;
}

/*
    nombre: producto p -> cadena
    Pre: true
    Post: Dado un producto p, se obtiene la cadena correspondiente al nombre
            del producto p

*/

string nombre(const producto& p) {
    return p.nombre;
}

/*
    cantidad: producto p -> entero
    Pre: true
    Post: Dado un producto p, se obtiene el entero correspondiente a la cantidad
            del producto p
*/

int cantidad(const producto& p) {
    return p.cantidad;
}

/*
    Pre: true
    Post: Dado un producto p1 y p2, devuelve verdadero si y solo si el nombre de ambos
        productos son iguales
*/

bool operator==(const producto& p1, const producto& p2){
    return nombre(p1) == nombre(p2); 
}

