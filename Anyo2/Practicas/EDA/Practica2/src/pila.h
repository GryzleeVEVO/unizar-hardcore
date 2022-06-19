#ifndef PILA_H
#define PILA_H

#include <iostream>

using namespace std;

/******************************************************************************/
/* ESPECIFIACIÓN                                                              */
/******************************************************************************/

/*
    Parámetro formal:
        Género T: Un tipo cualquiera a apilar

    Género Pila: El TAD representa una pila de elementos de tipo T siguiendo una
        política LIFO (último en entrar, primero en salir)
*/

/******************************************************************************/
/* PREDECLARACIONES                                                           */
/******************************************************************************/

template<typename T>
struct Pila;

/******************************************************************************/
/* Operaciones del tipo                                                       */
/******************************************************************************/

/*
    nuevaPila: -> Pila

    Crea una nueva pila vacía 
*/

template<typename T>
void nuevaPila(Pila<T>& p);

/*
    apilar: Pila p, T e -> Pila

    Devuelve una pila con el elemento e apilado sobre la cima de la pila
    anterior
*/

template<typename T>
void apilar(Pila<T>& p, const T& e);

/*
    parcial desapilar: Pila p, T e -> Pila

    Devuelve una pila de la que se ha eliminado su cima, siendo la nueva cima
    el elemento siguiente al de la cima

    Parcial: La operación no está definida si p es vacía
*/

/*
    parcial cima: Pila p -> T

    Devuelve el elemento en la cima de la pila

    Parcial: La operación no está definida si p es vacía
*/

template<typename T>
void desapilar(Pila<T>& p, T& e);

/*
    esPilaVacía?: Pila p -> booleano

    Devuelve verdad solo si la pila es vacía
*/

template<typename T>
bool esPilaVacia(const Pila<T>& p);

/******************************************************************************/
/* IMPLEMENTACIÓN                                                             */
/******************************************************************************/


/******************************************************************************/
/* Tipo Pila                                                                  */
/******************************************************************************/

template<typename T>
struct Pila{
    friend void nuevaPila<T>(Pila<T>& p);
    friend void apilar<T>(Pila<T>& p, const T& e);
    friend void desapilar<T>(Pila<T>& p, T& e);
    friend bool esPilaVacia<T>(const Pila<T>& p);

    private:    
        struct Elem{
            T elem;
            Elem* sig;
        };

        Elem* cima;
        int tamanyo;
};

/******************************************************************************/
/* Operaciones del tipo                                                       */
/******************************************************************************/

/*
 * Pre: ---
 * Post: Devuelve una pila vacía, sin elementos.
 */

template<typename T>
void nuevaPila(Pila<T>& p){
    p.cima = nullptr; // La cima no apunta a nada
    p.tamanyo = 0;
}

/*
 * Pre: ---
 * Post: Devuelve la pila resultante de añadir el elemento e a p
 */

template<typename T>
void apilar(Pila<T>& p, const T& e){
    typename Pila<T>::Elem* aux = p.cima; // El puntero aux ahora apunta a la cima de la pila
    p.cima = new typename Pila<T>::Elem; // Crea un nuevo elemento en la cima de la pila
    p.cima -> elem = e; // Asigna e a la cima de la pila
    p.cima -> sig = aux; //El elemento siguiente de la cima apunta a aux, último elemento creado.
    p.tamanyo++;
}

/*
 * Pre: ---
 * Post: Devuelve la pila resultante de eliminar de p el último elemento que fue apilado, si la pila no esta vacía.
 *       Si p es vacía, devuelve una pila igual a p
 */

template<typename T>
void desapilar(Pila<T>& p, T& e){
    typename Pila<T>::Elem* aux = p.cima; // El puntero aux ahora apunta a la cima de la pila
    p.cima = aux -> sig; // La cima apunta ahora a el siguiente elemento a borrar
    e = aux -> elem; // Obtiene el elemento desapilado

    delete aux; // Elimina la antigua cima
    p.tamanyo--;
}

/*
 * Pre:---
 * Post: Devuelve verdad si y solo si p no tiene elementos
 */

template<typename T>
bool esPilaVacia(const Pila<T>& p){
    return p.tamanyo == 0;
}

#endif