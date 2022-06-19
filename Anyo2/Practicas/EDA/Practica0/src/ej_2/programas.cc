#include "programas.h"

void crear(programa& p, const string nom, fecha cre, const string leng, string desc){
    p.nombre = nom;
    p.lenguaje = leng;
    p.descripcion = desc;
    p.creacion = cre;
}

string suNombre(const programa p){
    return p.nombre;
}

void suFecha(const programa p, fecha& f){
    f = p.creacion;
}

string suLenguaje(const programa p){
    return p.lenguaje;
}

string suDescripcion(const programa p){
    return p.descripcion;
}

string identificador(const programa p){
    return suNombre(p);
}

string tema(const programa p){
    return suLenguaje(p);
}

bool operator==(const programa& p1, const programa& p2){
    return(p1.nombre == p2.nombre && p1.lenguaje == p2.lenguaje 
        && p1.descripcion == p2.descripcion && iguales(p1.creacion, p2.creacion));
}

