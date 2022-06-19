/*
    Estructura de datos y algoritmos
    Práctica 2

    Autores: Dorian Boleslaw Wozniak        817570
             Jorge Pérez Liesa              821011

    Descripción: Archivo de implementación de un tipo programa que almacena el
    nombre, lenguaje, fecha de creación y descripción de un programa
*/

#include "programas.h"

/******************************************************************************/
/* IMPLEMENTACIÓN                                                             */
/******************************************************************************/

/******************************************************************************/
/* Operaciones del tipo                                                       */
/******************************************************************************/

/*
    Pre: true
    Post: Crea un programa p con un nombre nom, una fecha de creación cre,
            un lenguaje de codificación leng, y una descripción desc
*/

void crear(Programa& p, const string& nom, const Fecha& cre, const string& leng, string& desc){
    p.nombre = nom;
    p.lenguaje = leng;
    p.descripcion = desc;
    p.creacion = cre;
}

/*
    Pre: true
    Post: Devuelve el nombre del programa p
*/

string suNombre(const Programa& p){
    return p.nombre;
}

/*
    Pre: true
    Post: Devuelve la fecha de creación del programa p
*/

Fecha suFecha(const Programa& p){
    return p.creacion;
}

/*
    Pre: true
    Post: Devuelve el lenguaje de codificación del programa p
*/

string suLenguaje(const Programa& p){
    return p.lenguaje;
}

/*
    Pre: true
    Post: Devuelve la descripción del programa p
*/

string suDescripcion(const Programa& p){
    return p.descripcion;
}

/******************************************************************************/
/* Operadores                                                                 */
/******************************************************************************/

/*
    Pre: true
    Post: Devuelve verdad si y solo si los programas p1 y p2 son iguales, es
        decir, si sus nombres, fechas de creación, lenguajes y descripción son
        iguales
*/

bool operator==(const Programa& p1, const Programa& p2){
    return(p1.nombre == p2.nombre && p1.lenguaje == p2.lenguaje 
        && p1.descripcion == p2.descripcion && iguales(p1.creacion, p2.creacion));
}