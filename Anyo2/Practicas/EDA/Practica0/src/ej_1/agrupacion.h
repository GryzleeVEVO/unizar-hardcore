// El tipo T requiere
//      bool operator==(const T& e1, const t& e2);

#ifndef AGRUPACION_H
#define AGRUPACION_H

const int MAX = 40;

template<typename T> struct agrupacion;

// OPERACIONES AGRUPADOR

template<typename T> void iniciar (agrupacion<T>& c);
template<typename T> bool anyadir (agrupacion<T>& c, const T& e);
template<typename T> bool borrarUltimo (agrupacion<T>& c);
template<typename T> bool esta(agrupacion<T> c, const T e);

// OPERACIONES ITERADOR

template<typename T> void iniciarIterador(agrupacion<T>& c);
template<typename T> bool existeSiguiente(agrupacion<T> c);
template<typename T> bool siguiente(agrupacion<T>& c, T& e);

template<typename T>
struct agrupacion{

    // OPERACIONES AGRUPADOR

    friend void iniciar<T>(agrupacion<T>& c);
    friend bool anyadir<T>(agrupacion<T>& c, const T& e);
    friend bool borrarUltimo<T>(agrupacion<T>& c);
    friend bool esta<T>(agrupacion<T> c, const T e);

    // OPERACIONES ITERADOR

    friend void iniciarIterador<T>(agrupacion<T>& c);
    friend bool existeSiguiente<T>(agrupacion<T> c);
    friend bool siguiente<T>(agrupacion<T>& c, T& p);

    private: 

        //CAMPOS AGRUPADOR

        T datos[MAX];
        int total;

        // CAMPOS ITERADOR

        int pos;
};

/*
    Pre: true
    Post: Devuelve una agrupación vacía, sin elementos
*/

template<typename T>
void iniciar (agrupacion<T>& c) {
    c.total = 0;
}

/*
    Pre: true
    Post: Devuelve la agrupación resultante de añadir un elemento e a una agrupación c
*/

template<typename T>
bool anyadir (agrupacion<T>& c, const T& e) {
    bool sePuede = c.total < MAX;
    if (sePuede) {
        c.datos[c.total] = e;
        c.total++;
     }
    return sePuede;
}

/*
    Pre: c no esta vacío
    Post: Devuelve la agrupación resultante de eliminar de c el último elemento añadido a c
*/

template<typename T>
bool borrarUltimo (agrupacion<T>& c) {
    bool sePuede = c.total > 0;
    if (sePuede) c.total--;
    return sePuede;
}

/*
    Pre: true
    Post: Dada una agrupación c y un elemento e, devuelve verdad si y solo si el elemento e
    está en c, falso en caso contrario

*/

template<typename T>
bool esta(agrupacion<T> c, const T e){
    iniciarIterador(c);
    bool pertenece = false;
    for(int i = 0; i < c.total; i++){
        pertenece = c.datos[i] == e;
    }

    return pertenece;
}


/*
    Pre: true
    Post: Prepara el iterador para que el siguiente elemento a visitar sea el primero
            (situación de no haber visitado ningún elemento)
*/

template<typename T>
void iniciarIterador(agrupacion<T>& c){
    c.pos = 0;
}

/*
    Pre: true
    Post: Devuelve verdad si queda algún elemento por visitar, devuelve falso si ya se ha
            visitado el último elemento
*/

template<typename T>
bool existeSiguiente(agrupacion<T> c){
    return c.pos < c.total;
}

/*
    Pre: Quedan elementos por visitar
    Post: Devuelve el siguiente elemento a visitar y avanza el iterador
            para el proximo elemento
*/

template<typename T>
bool siguiente(agrupacion<T>& c, T& e){
    e = c.datos[c.pos];
    if(existeSiguiente(c)){
        c.pos++;
        return true;
    }

    else{
        return false;
    }
}

#endif //fin de agrupacion.h

