#ifndef COLECCIONITF_H
#define COLECCIONITF_H

#include "fechas.h"
#include <iostream>
using namespace std;

const int MAX_ITEMS = 100;

template<typename T> struct coleccionITF; 
template<typename T> void crear(coleccionITF<T>& c);
template<typename T> bool anyadir(coleccionITF<T>& c, const T i);
template<typename T> bool borrar(coleccionITF<T>& c, const string ident);
template<typename T> bool esta(coleccionITF<T> c, const string ident);
template<typename T> bool obtenerITF(coleccionITF<T> c, T& i, const string ident);
template<typename T> void obsoletos(coleccionITF<T> c, const fecha f, const string separador, string& s);
template<typename T> bool purgar(coleccionITF<T>& c, const fecha f);
template<typename T> void delTema(coleccionITF<T> c, const string t, const string separador, string& s);
template<typename T> bool esVacia(coleccionITF<T> c);
template<typename T> int tamanyo(coleccionITF<T> c);

template<typename T> void iniciarIterador(coleccionITF<T>& c);
template<typename T> bool haySiguiente(coleccionITF<T> c);
template<typename T> bool siguiente(coleccionITF<T>& c, T& i);

template<typename T> struct coleccionITF{ 
    friend void crear<T>(coleccionITF<T>& c);
    friend bool anyadir<T>(coleccionITF<T>& c, const T i);
    friend bool borrar<T>(coleccionITF<T>& c, const string ident);
    friend bool esta<T>(coleccionITF<T> c, const string ident);
    friend bool obtenerITF<T>(coleccionITF<T> c, T& i, const string ident);
    friend void obsoletos<T>(coleccionITF<T> c, const fecha f, const string separador, string& s);
    friend bool purgar<T>(coleccionITF<T>& c, const fecha f);
    friend void delTema<T>(coleccionITF<T> c, const string t, const string separador, string& s);
    friend bool esVacia<T>(coleccionITF<T> c);
    friend int tamanyo<T>(coleccionITF<T> c);

    friend void iniciarIterador<T>(coleccionITF<T>& c);
    friend bool haySiguiente<T>(coleccionITF<T> c);
    friend bool siguiente<T>(coleccionITF<T>& c, T& i);

    private:
        T items[MAX_ITEMS];
        int tamanyo;
        int iter;
};

template<typename T> void crear(coleccionITF<T>& c){
    c.tamanyo = 0;
}

template<typename T> bool anyadir(coleccionITF<T>& c, const T i){
    bool anyadido = false;
    if(esVacia(c)){
        c.items[c.tamanyo] = i;
        c.tamanyo++;
        anyadido = true;
    }

    else{
        if(!esta(c, identificador(i))){
            coleccionITF<T> c_aux;
            T i_aux;
            crear(c_aux);
            iniciarIterador(c);

            while(siguiente(c, i_aux)){
                if(identificador(i_aux) > identificador(i) && !anyadido){
                    c_aux.items[c_aux.tamanyo] = i;
                    c_aux.tamanyo++;
                    anyadido = true;
                }

                c_aux.items[c_aux.tamanyo] = i_aux;
                c_aux.tamanyo++;
            }

            if(!anyadido){
                c_aux.items[c_aux.tamanyo] = i;
                c_aux.tamanyo++;
                anyadido = true;
            }
            c = c_aux;
        }
    }

    return anyadido;
}

template<typename T> bool borrar(coleccionITF<T>& c, const string ident){
    bool eliminado = false;
    if(!esVacia(c) && esta(c, ident)){
        coleccionITF<T> c_aux;
        T i_aux;
        crear(c_aux);
        iniciarIterador(c);

        while(siguiente(c, i_aux)){
            if(identificador(i_aux) != ident){
                c_aux.items[c_aux.tamanyo] = i_aux;
                c_aux.tamanyo++;
            }
        }

        c = c_aux;
        eliminado = true;
    }

    return eliminado;
}

template<typename T> bool esta(coleccionITF<T> c, const string ident){
    bool existe = false;

    if(!esVacia(c)){
        T i_aux;

        iniciarIterador(c);
        while(!existe && siguiente(c, i_aux)){
            if(identificador(i_aux) == ident){
                existe = true;
            }
        }
    }

    return existe;
}

template<typename T> bool obtenerITF(coleccionITF<T> c, T& i, const string ident){
    bool obtenido = false;

    T i_aux;
    iniciarIterador(c);

    while(siguiente(c, i_aux)){
        if(identificador(i_aux) == ident){
            i = i_aux;
            obtenido = true;
        }
    }

    return obtenido;
}

template<typename T> void obsoletos(coleccionITF<T> c, const fecha f, const string separador, string& s){
    T i_aux;
    fecha f_aux;
    iniciarIterador(c);
    while(siguiente(c, i_aux)){
        suFecha(i_aux, f_aux);
        if(anterior(f_aux, f)){
            s = s + separador + identificador(i_aux) ;
        }
    }
}

template<typename T> bool purgar(coleccionITF<T>& c, const fecha f){
    bool purgados = false;

    coleccionITF<T> c_aux;
    T i_aux;
    fecha f_aux;
    if(!esVacia(c)){
        crear(c_aux);
        iniciarIterador(c);
        while(siguiente(c, i_aux)){
            suFecha(i_aux, f_aux);
            if(!anterior(f_aux, f)){
                c_aux.items[c_aux.tamanyo] = i_aux;
                c_aux.tamanyo++;
            }

            else{
                purgados = true;
            }
        }

        c = c_aux;
    }

    return purgados;
}


template<typename T> void delTema(coleccionITF<T> c, const string t, const string separador, string& s){
    T i_aux;


    iniciarIterador(c);
    while(siguiente(c, i_aux)){
        if(tema(i_aux) == t){
            s = s + separador + identificador(i_aux) ;
        }
    }
}

template<typename T> bool esVacia(coleccionITF<T> c){
    return (tamanyo(c) == 0);
}

template<typename T> int tamanyo(coleccionITF<T> c){
    return c.tamanyo;
}


template<typename T> void iniciarIterador(coleccionITF<T>& c){
    c.iter = 0;
}

template<typename T> bool haySiguiente(coleccionITF<T> c){
    return c.iter < c.tamanyo;
}

template<typename T> bool siguiente(coleccionITF<T>& c, T& i){
    i = c.items[c.iter];

    if(haySiguiente(c)){
        c.iter++;
        return true;
    }

    else{
        return false;
    }
}

#endif