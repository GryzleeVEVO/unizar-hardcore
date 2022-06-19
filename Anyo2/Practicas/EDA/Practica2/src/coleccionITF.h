/*
    Estructura de datos y algoritmos
    Práctica 2

    Autores: Dorian Boleslaw Wozniak    817570
             Jorge Pérez Liesa          821011

    Descripción: Archivo de especificación e implementación de un tipo genérico
    ColeccionITF, que almacenará items temáticos fechados (parámetro formal
    ITF) según la especificación a continuación.

    A diferencia de las prácticas anteriores, que almacenaban los ítems en un
    vector o como un diccionario implementado mediante una lista enlazadas,
    esta versión es implementada en forma de un árbol binario de búsqueda.
*/

#ifndef COLECCIONITF_H
#define COLECCIONITF_H

#include "fechas.h"
#include "pila.h"

#include <iostream>

using namespace std;

/******************************************************************************/
/* ESPECIFIACIÓN                                                              */
/******************************************************************************/

/*
    Parámetro formal:
        Género ITF: Un item temático fechado

        Operaciones: (implementados en el programa principal)
            identificador: ITF i -> cadena
            Operación que devuelve el identificador del ítem

            tema: ITF i -> cadena
            Operación que devuelve el tema del ítem

            fecha: ITF i -> Fecha
            Operación que devuelve la fecha asociada al ítem

    Género ColecciónITF: Los valores del TAD representan colecciones de ITF con
                         identificadores no repetidos
*/

/******************************************************************************/
/* PREDECLARACIONES                                                           */
/******************************************************************************/

template<typename ITF>
struct ColeccionITF;

/******************************************************************************/
/* Operaciones del tipo                                                       */
/******************************************************************************/

/*
    crear: -> ColecciónITF

    Crea una colección de ITF vacía, sin ítems
*/

template<typename ITF>
void crear(ColeccionITF<ITF>& c);

/*
    añadir: ColecciónITF c, ITF i -> ColecciónITF

    Si en c no hay ningún ítem con identificador igual al de i, devuelve una
    colección igual a la resultante de añadir el ítem i a la colección c. En
    caso contrario, devuelve una colección igual a c
*/

template<typename ITF>
bool anyadir(ColeccionITF<ITF>& c, const ITF& i);

/*
    borrar: cadena ident, ColecciónITF c -> ColecciónITF
    
    Si en c hay algún ítem con identificador igual a ident, devuelve una
    colección igual a la resultante de borrar ese ítem de la colección c. En
    caso contrario, devuelve una colección igual a c
*/

template<typename ITF>
bool borrar(ColeccionITF<ITF>& c, const string& ident);

/*
    esta?: cadena ident, ColecciónITF c -> booleano

    Devuelve verdad si y solo si en c hay algún ítem con identificador igual a
    ident
*/

/*
    parcial obtenerITF: cadena ident, ColecciónITF c -> ITF

    Si en c hay algún ítem con identificador igual a ident, devuelve dicho ítem

    Parcial: No definida si en c no hay ningún ítem con identificador ident
*/

template<typename ITF>
bool esta(const ColeccionITF<ITF>& c, ITF& i, const string& ident);


/*
    obsoletos: Fecha f, cadena separador, ColecciónITF c -> cadena

    Devuelve la cadena resultante de concatenar los identificadores de todos los
    ítems que hay en c con fecha estrictamente anterior a f, en cualquier orden
    y separados enre sí por la cadena separador
*/

template<typename ITF>
string obsoletos(const ColeccionITF<ITF>& c, const Fecha& f, const string& separador);

/*
    purgar: Fecha f, ColecciónITF c -> cadena

    Devuelve una colección igual a la resultante de eliminar de c todos los
    ítems con fecha estrictamente anterior a f
*/

template<typename ITF>
bool purgar(ColeccionITF<ITF>& c, const Fecha& f);

/*
    delTema: cadena t, cadena separador, ColecciónITF c -> cadena

    Devuelve la cadena resultante de concatenar los identificadores de todos los
    ítems que hay en c con tema igual a t, en cualquier orden y separados entre
    sí por la cadena separador
*/

template<typename ITF>
string delTema(const ColeccionITF<ITF>& c, const string& t, const string& separador);

/*
    esVacía?: ColecciónITF c -> booleano

    Deuelve verdad solo si la colección c es vacía
*/

template<typename ITF> 
bool esVacia(const ColeccionITF<ITF>& c);

/*
    tamaño: ColecciónITF c -> booleano

    Devuelve el número de ítems que hay en la colección c
*/

template<typename ITF> 
int tamanyo(const ColeccionITF<ITF>& c);

/*
    iniciarIterador: ColecciónITF c -> ColecciónITF c

    Inicializa el iterador para recorrer los ítems de la colección c, de forma
    que el siguiente ítrm a visitar sea el que tiene el (lexicográficamente)
    menor identificador (situación de no haber visitado ningún ítem)
*/

/******************************************************************************/
/* Operaciones del iterador                                                   */
/******************************************************************************/

template<typename ITF>
void iniciarIterador(ColeccionITF<ITF>& c);

/*
    haySiguiente?: ColecciónITF c -> booleano

    Devuelve verdad si queda algún ítem por visitar en el iterador de la
    colección c, devuelve falso si ya se ha visitado el último ítem
*/

template<typename ITF>
bool haySiguiente(const ColeccionITF<ITF>& c);

/*
    parcial siguiente: ColecciónITF c -> ITF

    Devuelve el siguiente ítem a visitar con el iterador de la colección c

    Parcial: La operación no está definida si no quedan ítems por visitar
*/

/*
    parcial avanza: ColecciónITF c -> ColecciónITF

    Prepara el iterador de la colección c para que se pueda visitar el siguiente
    ítem con (lexicográficamente) menor identificador de los todavía no 
    visitados

    Parcial: La operación no está definida si no quedan ítems por visitar
*/

template<typename ITF>
bool siguiente(ColeccionITF<ITF>& c, ITF& i);

/******************************************************************************/
/* Operaciones auxiliares (privadas)                                          */
/******************************************************************************/

template<typename ITF>
bool anyadir(typename ColeccionITF<ITF>::Nodo*& nodo, const ITF& i);

template<typename ITF>
bool borrar(typename ColeccionITF<ITF>::Nodo*& nodo, const string& ident);

template<typename ITF>
void borrarNodo(typename ColeccionITF<ITF>::Nodo*& nodo);

template<typename ITF>
void borrarNodo(typename ColeccionITF<ITF>::Nodo*& nodo, ITF& i);

template<typename ITF>
bool esta(typename ColeccionITF<ITF>::Nodo* nodo, ITF& i, const string& ident);

template<typename ITF>
void obsoletos(typename ColeccionITF<ITF>::Nodo* nodo, const Fecha& f, const string& separador, string& s, bool& p);

template<typename ITF>
void purgar(typename ColeccionITF<ITF>::Nodo*& nodo, const Fecha& f, int& b);

template<typename ITF>
void delTema(typename ColeccionITF<ITF>::Nodo* nodo, const string& t, const string& separador, string& s, bool& p);

/******************************************************************************/
/* IMPLEMENTACIÓN                                                             */
/******************************************************************************/

/******************************************************************************/
/* Tipo ColecciónITF                                                          */
/******************************************************************************/

template<typename ITF>
struct ColeccionITF{
    friend void crear<ITF>(ColeccionITF<ITF>& c);
    friend bool anyadir<ITF>(ColeccionITF<ITF>& c, const ITF& i);
    friend bool borrar<ITF>(ColeccionITF<ITF>& c, const string& ident);
    friend bool esta<ITF>(const ColeccionITF<ITF>& c, ITF& i, const string& ident);
    friend string obsoletos<ITF>(const ColeccionITF<ITF>& c, const Fecha& f, const string& separador);
    friend bool purgar<ITF>(ColeccionITF<ITF>& c, const Fecha& f);
    friend string delTema<ITF>(const ColeccionITF<ITF>& c, const string& t, const string& separador);
    friend bool esVacia<ITF>(const ColeccionITF<ITF>& c);
    friend int tamanyo<ITF>(const ColeccionITF<ITF>& c);

    friend void iniciarIterador<ITF>(ColeccionITF<ITF>& c);
    friend bool haySiguiente<ITF>(const ColeccionITF<ITF>& c);
    friend bool siguiente<ITF>(ColeccionITF<ITF>& c, ITF& i);

    private:
        struct Nodo{
            ITF item;
            Nodo* izda;
            Nodo* dcha;
        };

        Nodo* raiz;
        Pila<Nodo*> iter;
        int tamanyo;

        friend bool anyadir<ITF>(Nodo*& nodo, const ITF& i);
        friend bool borrar<ITF>(Nodo*& nodo, const string& ident);
        friend void borrarNodo<ITF>(Nodo*& nodo);
        friend void borrarNodo<ITF>(Nodo*& nodo, ITF& i);
        friend bool esta<ITF>(Nodo* nodo, ITF& i, const string& ident);
        friend void obsoletos<ITF>(Nodo* nodo, const Fecha& f, const string& separador, string& s, bool& p);
        friend void purgar<ITF>(Nodo*& nodo, const Fecha& f, int& b);
        friend void delTema<ITF>(Nodo* nodo, const string& t, const string& separador, string& s, bool& p);
};

/******************************************************************************/
/* Operaciones del tipo                                                       */
/******************************************************************************/

/*
 * Pre: True
 * Post: Devuelve una colección vacía
 */

template<typename ITF>
void crear(ColeccionITF<ITF>& c){
    c.raiz = nullptr; // La raíz no apuntará a nada
    c.tamanyo = 0;
}
 
/*
 * Pre: True
 * Post: Devuelve verdad solo si el item i no se encuentra en la colección y se ha
 *       logrado añadir el item i a la colección. En caso contrario devuelve falso.
 */

template<typename ITF>
bool anyadir(ColeccionITF<ITF>& c, const ITF& i){
    bool anyadido = anyadir(c.raiz, i);

    if(anyadido){
        c.tamanyo++;
    }

    return anyadido;
}

template<typename ITF>
bool anyadir(typename ColeccionITF<ITF>::Nodo*& nodo, const ITF& i){
    // Encuentra una hoja / raíz del árbol vacía
    if(nodo == nullptr){
        nodo = new typename ColeccionITF<ITF>::Nodo; 
        nodo -> item = i;                            
        nodo -> izda = nullptr;                      
        nodo -> dcha = nullptr;
        return true;
    }

    // Identificador menor que raíz de subárbol
    else if(identificador(i) < identificador(nodo -> item)){
        return anyadir(nodo -> izda, i);
    }

    // Identificador mayor que raíz de subárbol
    else if(identificador(i) > identificador(nodo -> item)){
        return anyadir(nodo -> dcha, i);
    }
    
    // Identificador igual que raíz de subárbol
    else if(identificador(i) == identificador(nodo -> item)){
        return false;
    }

    // Ningún caso se cumple (error grave)
    else{
        return false;
    }
}

/*
 * Pre: True
 * Post: Devuelve verdad y la colección c con el item con identificador ident borrado sólo si se ha hallado
 *       dicho item; devuelve falso y la misma colección c en caso contrario
 */

template<typename ITF>
bool borrar(ColeccionITF<ITF>& c, const string& ident){
    bool borrado = borrar<ITF>(c.raiz, ident);

    if(borrado){
        c.tamanyo--;
    }

    return borrado;
}

template<typename ITF>
bool borrar(typename ColeccionITF<ITF>::Nodo*& nodo, const string& ident){
    // Encuentra una hoja
    if(nodo == nullptr){
        return false;
    }

    // Encuentra el nodo a buscar
    else if(ident == identificador(nodo -> item)){
        borrarNodo<ITF>(nodo);
        return true;
    }

    // Identificador menor que raíz de subárbol
    else if(ident < identificador(nodo -> item)){
        return borrar<ITF>(nodo -> izda, ident);
    }

    // Identificador mayor que raíz de subárbol
    else if(ident > identificador(nodo -> item)){
        return borrar<ITF>(nodo -> dcha, ident);
    }

    // Ningún caso se cumple (error grave)
    return false;
}

/*
 * Pre: True y c es no vacío.
 * Post: Devuelve verdad y la colección con el nodo borrado si dicho nodo esta vacio. Si no esta vacío,
 *       busca el nodo maximo como sustituto de la raíz.
 */

template<typename ITF>
void borrarNodo(typename ColeccionITF<ITF>::Nodo*& nodo){
    // El nodo a la izda está vacío -> Sustituye raíz por raíz del subárbol dcho
    if(nodo -> izda == nullptr){
        typename ColeccionITF<ITF>::Nodo* aux = nodo;
        nodo = nodo -> dcha;
        delete aux;
    }

    // Busca nodo máximo del subárbol izquierdo como sustituto de la raíz
    else{
        borrarNodo(nodo -> izda, nodo -> item);
    }
}

template<typename ITF>
void borrarNodo(typename ColeccionITF<ITF>::Nodo*& nodo, ITF& i){
    // El subárbol a la derecha del nodo es una hoja -> Máximo a su izda
    if(nodo -> dcha == nullptr){
        i = nodo -> item;
        typename ColeccionITF<ITF>::Nodo* aux = nodo;
        nodo = nodo -> izda;
        delete aux;
    }

    // Busca el siguiente candidato en el subárbol dcho
    else{
        borrarNodo(nodo -> dcha, i);
    }
}

/*
 * Pre: True
 * Post: Devuelve verdad y el ítem i con identificador ident solo si se ha encontrado en la colección c dicho item;
 *       devuelve falso y el mismo item i en caso contrario
 */

template<typename ITF>
bool esta(const ColeccionITF<ITF>& c, ITF& i, const string& ident){
    return esta<ITF>(c.raiz, i, ident);
}

template<typename ITF>
bool esta(typename ColeccionITF<ITF>::Nodo* nodo, ITF& i, const string& ident){
    // Encuentra una hoja
    if(nodo == nullptr){
        return false;
    }

    // Encuentra el ítem buscado
    else if(ident == identificador(nodo -> item)){
        i = nodo -> item;
        return true;
    }

    // Identificador menor que raíz de subárbol
    else if(ident < identificador(nodo -> item)){
        return esta(nodo -> izda, i, ident);
    }

    // Identificador mayor que raíz de subárbol
    else if(ident > identificador(nodo -> item)){
        return esta(nodo -> dcha, i, ident);
    }

    // Ningún caso se cumple (error grave)
    return false;
}

/*
 * Pre: True
 * Post: Devuelve la cadena formada por los items de la colección c cuya fecha sea anterior a la fecha f,
 *       separados por la cadena separador. La cadena se recorre en post-orden.
 */

template<typename ITF>
string obsoletos(const ColeccionITF<ITF>& c, const Fecha& f, const string& separador){
    bool p = true;
    string s = "";
    if(!esVacia(c)){
        obsoletos<ITF>(c.raiz, f, separador, s, p);
    }
    return s;
}

template<typename ITF>
void obsoletos(typename ColeccionITF<ITF>::Nodo* nodo, const Fecha& f, const string& separador, string& s, bool& p){
    // Busca en el subárbol izquierdo
    if(nodo -> izda != nullptr){
        obsoletos<ITF>(nodo -> izda, f, separador, s, p);
    }

    // Busca en el subárbol derecho
    if(nodo -> dcha != nullptr){
        obsoletos<ITF>(nodo -> dcha, f, separador, s, p);
    }

    // Encuentra un item a listar en la raíz
    if(anterior(fecha(nodo -> item), f)){
        if(p){
            s = s + identificador(nodo -> item);
            p = false;
        }

        else{
            s = s + separador + identificador(nodo -> item);
        }
    }
}

/*
 * Pre: True
 * Post: Devuelve verdad solo si ha devuelto una colección de la que se han eliminado los items cuya fecha de
 *       creación es anterior a la fecha f. En caso contrario devuelve falso y la misma colección
 */

template<typename ITF>
bool purgar(ColeccionITF<ITF>& c, const Fecha& f){
    int b = 0;
    if(!esVacia(c)){
        purgar<ITF>(c.raiz, f, b);
    }

    c.tamanyo = c.tamanyo - b;
    return b != 0;
}

template<typename ITF>
void purgar(typename ColeccionITF<ITF>::Nodo*& nodo, const Fecha& f, int& b){
    // Busca en el subárbol izquierdo
    if(nodo -> izda != nullptr){
        purgar<ITF>(nodo -> izda, f, b);
    }

    // Busca en el subárbol derecho
    if(nodo -> dcha != nullptr){
        purgar<ITF>(nodo -> dcha, f, b);
    }

    // Encuentra un ítem a borrar en la raíz
    if(anterior(fecha(nodo -> item), f)){
        borrarNodo<ITF>(nodo);
        b++;
    }
}

/*
 * Pre: True
 * Post: Devuelve la cadena formada por los items de la colección cuyo tema sea igual al tema t,
 *       separados por la cadena separador. La cadena se recorre en pre-orden.
 */

template<typename ITF>
string delTema(const ColeccionITF<ITF>& c, const string& t, const string& separador){
    bool p = true;
    string s = "";
    if(!esVacia(c)){
        delTema<ITF>(c.raiz, t, separador, s, p);
    }
    return s;
}

template<typename ITF>
void delTema(typename ColeccionITF<ITF>::Nodo* nodo, const string& t, const string& separador, string& s, bool& p){
        // Encuentra un item a listar en la raíz
    if(t == tema(nodo -> item)){
        if(p){
            s = s + identificador(nodo -> item);
            p = false;
        }

        else{
            s = s + separador + identificador(nodo -> item);
        }
    }

    // Busca en el subárbol izquierdo
    if(nodo -> izda != nullptr){
        delTema<ITF>(nodo -> izda, t, separador, s, p);
    }

    // Busca en el subárbol derecho
    if(nodo -> dcha != nullptr){
        delTema<ITF>(nodo -> dcha, t, separador, s, p);
    }
}

/*
 * Pre: True
 * Post: Devuelve verdad solo si el tamaño de la colección c no es igual a 0
 */

template<typename ITF>
bool esVacia(const ColeccionITF<ITF>& c){
    return c.tamanyo == 0;
}

/*
 * Pre: True
 * Post: Devuelve el número de items que actualmente contiene la colección c
 */

template<typename ITF> 
int tamanyo(const ColeccionITF<ITF>& c){
    return c.tamanyo;
}

/******************************************************************************/
/* Operaciones del iterador                                                   */
/******************************************************************************/

/*
 * Pre: True
 * Post: El puntero iterador apuntará al primer ítem (situación de no haber visitado
 *       ningún elemento)
 */

template<typename ITF>
void iniciarIterador(ColeccionITF<ITF>& c){
    // Crea una nueva pila
    nuevaPila(c.iter); 

    // Puntero auxiliar a la raíz
    typename ColeccionITF<ITF>::Nodo* aux = c.raiz;

    // Apila todos los nodos inmediatamente a la izda de la raíz
    while(aux != nullptr){
        apilar(c.iter, aux);
        aux = aux -> izda;
    }
}

/*
 * Pre: True
 * Post: Devuelve verdad solo si el puntero iterador no tiene un valor nulo;
 */

template<typename ITF>
bool haySiguiente(const ColeccionITF<ITF>& c){
    return !(esPilaVacia(c.iter));
}

/*
 * Pre: True
 * Post: Devuelve verdad solo si existe el siguiente item; si es el caso, obtiene el siguiente
 *       ítem y el puntero iterador apunta al subarbol derecho del nodo desapilado y apila todo
 *       a la izquierda del subarbol derecho
 */

template<typename ITF>
bool siguiente(ColeccionITF<ITF>& c, ITF& i){
    if(haySiguiente(c)){
        // Crea un puntero auxiliar
        typename ColeccionITF<ITF>::Nodo* aux;
        // Desapila el último elemento
        desapilar(c.iter, aux);
        i = aux -> item;
        // aux ahora apuntara al subárbol dcho del nodo desapilado
        aux = aux -> dcha;

        // Apila todo inmediatamente a la izquierda del subárbol derecho
        while(aux != nullptr){
            apilar(c.iter, aux);
            aux = aux -> izda;
        }

        return true;
    }

    else{
        return false;
    }
}

#endif