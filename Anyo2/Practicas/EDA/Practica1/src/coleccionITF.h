/*
    Estructura de datos y algoritmos
    Práctica 1

    Autores: Dorian Boleslaw Wozniak        817570
             Jorge Pérez Liesa              821011

    Descripción: Archivo de especificación e implementación de un tipo genérico colecciónITF, que 
    almacenará ítems temáticos fechados, que será un parámetro formal T, según la especificación
    a continuación
*/

#ifndef COLECCIONITF_H
#define COLECCIONITF_H

#include <iostream>
#include "fechas.h"

using namespace std;

// ESPECIFICACIÓN

/*
    Parámetro formal:
        Género T: Un item temático fechado

        Operaciones: 
            identificador: T i -> cadena
            Operación que devuelve el identificador del ítem

            tema: T i -> cadena
            Operación que devuelve el tema del ítem

            fecha: T i -> fecha
            Operación que devuelve la fecha asociada al item

    Género colecciónITF: Los valores del TAD representan colecciones de ITF con
                            identificadores no repetidos
*/

// PREDECLARACIONES

template<typename T> struct coleccionITF;

/*
    crear: -> colecciónITF

    Crea una colección de ITF vacía, sin ítems
*/

template<typename T> void crear(coleccionITF<T>& c);

/*
    añadir: colecciónITF c, ITF i -> colecciónITF

    Si en c no hay ningún ítem con identificador igual al de i, devuelve una colección
    igual a la resultante de añadir el item i a la colección c. En caso contrario,
    devuelve una colección igual a c
*/

template<typename T> bool anyadir(coleccionITF<T>&, T i);
/*
    borrar: cadena ident, colecciónITF c --> colecciónITF
    
    Si en c hay algún ítem con identificador igual a ident, devuelve una colección
    igual a la resultante de borrar ese item de la colección c. En caso contrario,
    devuelve una colección igual a c
*/

template<typename T> bool borrar(coleccionITF<T>& c, string ident);

/*
    esta?: cadena ident, colecciónITF c -> booleano

    Devuelve verdad si y solo si en c hay algún ítem con identificador igual a ident
*/


/*
    parcial obtenerITF: cadena ident, colecciónITF c -> ITF

    Si en c hay algún ítem con identificador igual a ident, devuelve dicho item

    Parcial: no definida si en c no hay ningún ítem con identificador igual a ident
*/

template<typename T> bool esta(const coleccionITF<T>& c, T& i, string ident);


/*
    obsoletos: fecha f, cadena separador, colecciónITF c -> cadena

    Devuelve la cadena resultante de concatenar los identificadores de todos los ítems
    que hay en c con Fecha estrictamente anterior a f, en cualquier orden y separados
    entre sí por la cadena separador 
*/

template<typename T> string obsoletos(const coleccionITF<T>& c, Fecha f, string separador);


/*
    purgar: fecha f, cadena separador, colecciónITF c -> cadena

    Devuelve una colección igual a la resultante de eliminar de c todos los ítems con fecha
    estrictamente anterior a f
*/

template<typename T> bool purgar(coleccionITF<T>& c, Fecha f);


/*
    delTema: cadena t, cadena separador, colecciónITF c -> cadena

    Devuelve la cadena resultante de concatenar los identificadores de todos los ítems que
    hay en c con tema igual a t, en cualquier orden y separados entre sí por la cadena separador
*/

template<typename T> string delTema(const coleccionITF<T>& c, string t, string separador);


/*
    esVacía?: colecciónITF c -> booleano

    Devuelve el número de ítems que hay en la colección c
*/

template<typename T> bool esVacia(const coleccionITF<T>& c);

/*
    tamaño: colecciónITF c -> booleano

    Devuelve el número de ítems que hay en la colección c
*/

template<typename T> int tamanyo(const coleccionITF<T>& c);

// OPERACIONES DEL ITERADOR

/*
    iniciarIterador: colecciónITF c -> colecciónITF

    Inicializa el iterador para recorrer los ítems de la colección c, de forma que el siguiente ítem
    a visitar sea el que tiene el (lexicográficamente) menor identificador (situación de no haber
    visitado ningún ítem)
*/

template<typename T> void iniciarIterador(coleccionITF<T>& c);

/*
    haySiguiente?: colecciónITF c -> booleano

    Devuelve verdad si queda algún ítem por visitar con el iterador de la colección c, devuelve
    falso si ya se ha visitado el último ítem
*/

template<typename T> bool haySiguiente(const coleccionITF<T>& c);

/*
    parcial siguiente: colecciónITF c -> ITF

    Devuelve el siguiente ítem a visitar con el iterador de la colección c 

    Parcial: la operación no está definida si no quedan ítems por visitar
*/

/*
    parcial avanza: colecciónITF c -> colecciónITF

    Prepara el iterador de la colección c para que se pueda visitar el siguiente ítem, entendiendo
    por siguiente aquel ítem con (lexicográficamente) menor iterador de los todavía no visitados

    Parcial: la operación no está definida si no quedan ítems por visitar
*/

template<typename T> bool siguiente(coleccionITF<T>& c, T& i);

// IMPLEMENTACIÓN

template<typename T> struct coleccionITF{
    friend void crear<T>(coleccionITF<T>& c);
    friend bool anyadir<T>(coleccionITF<T>&, T i);
    friend bool borrar<T>(coleccionITF<T>& c, string ident);
    friend bool esta<T>(const coleccionITF<T>& c, T& i, string ident);
    friend string obsoletos<T>(const coleccionITF<T>& c, Fecha f, string separador);
    friend bool purgar<T>(coleccionITF<T>& c, Fecha f);
    friend string delTema<T>(const coleccionITF<T>& c, string t, string separador);
    friend bool esVacia<T>(const coleccionITF<T>& c);
    friend int tamanyo<T>(const coleccionITF<T>& c);
    
    friend void iniciarIterador<T>(coleccionITF<T>& c);
    friend bool haySiguiente<T>(const coleccionITF<T>& c);
    friend bool siguiente<T>(coleccionITF<T>& c, T& i);

    private:
        struct unItem{
            T item;
            unItem* siguiente;
        };

        int tamanyo;
        unItem* primero;
        unItem* iter;

};

/*
    Pre: true
    Post: Devuelve una colección vacía
*/

template<typename T> void crear(coleccionITF<T>& c){
    c.primero = nullptr;
    c.iter = nullptr;
    c.tamanyo = 0;
}

/*
    Pre: true
    Post: Devuelve verdad solo si el ítem i no se encuentra en la colección y se ha
            logrado añadir el item i a la colección. En caso contrario, devuelve falso.
*/


template<typename T> bool anyadir(coleccionITF<T>& c, T i){
    bool anyadido = false; 
    
    // Caso 1: Está vacío
    if(c.tamanyo == 0){
        c.primero = new typename coleccionITF<T>::unItem; // Reserva una nueva celda
        c.primero -> item = i;                           // Introduce el ítem
        c.primero -> siguiente = nullptr;                // No hay siguiente elemento, =nil
        c.tamanyo++;
        anyadido = true;
    }

    else{
        typename coleccionITF<T>::unItem* pAux = c.primero; // pAux ahora apuntará al primer elemento

        // Caso 2: Hay que introducirlo al principio
        if(identificador(i) < identificador(c.primero -> item)){
            c.primero = new typename coleccionITF<T>::unItem;   // c.primero ahora apunta a una nueva celda
            c.primero -> item = i;
            c.primero -> siguiente = pAux;                      // siguiente ahora apuntará a la celda a la que
                                                                // antes apuntaba c.primero
            c.tamanyo++;
            anyadido = true;
        }

        else{

            // Itera hasta que llega al final o hasta que encuentre el primer ítem con identificador mayor que el de i
            while(pAux -> siguiente != nullptr && identificador(pAux -> item) < identificador(i)){
                pAux = pAux -> siguiente;   // pAux ahora apuntará al siguiente elemento de la lista
            }

            // Caso de error: Ya existe un elemento con el mismo identificador (no hacer nada)
            if(identificador(i) != identificador(pAux ->item)){

                // Caso 3: El iterador se ha detenido y el identificador de i es menor que el de la celda pAux actual
                if(identificador(i) < identificador(pAux -> item)){ 
                    typename coleccionITF<T>::unItem celdaAux; // Se crea una celda auxiliar 
                    celdaAux.item = i;                         
                    typename coleccionITF<T>::unItem* nuevo = new typename coleccionITF<T>::unItem; // Se crea un puntero a una nueva celda 
                    
                    *nuevo = *pAux;    // El contenido de lo apuntado por nuevo es el mismo de lo apuntado por pAux
                    *pAux = celdaAux;   // Ahora lo apuntado por pAux es igual a celdaAux 
                    pAux -> siguiente = nuevo; // pAux ahora apunta a nuevo
                    c.tamanyo++;                // Antes: Anterior -> pAux -> Siguiente
                    anyadido = true;            // Despues: Anterior -> celdaAux (sustituye el pAux anterior) -> nuevo (contiene el pAux anterior) -> Siguiente  
                }                             

                // Caso 4: El iterador se ha detenido en la última celda e i sigue siendo más grande que cualquier elemento
                else{
                    pAux -> siguiente = new typename coleccionITF<T>::unItem; // Se reserva una nueva celda desde el último siguiente 
                    pAux = pAux -> siguiente;                                 // Ahora pAux también apunta al último
                    pAux -> item = i;
                    pAux -> siguiente = nullptr;
                    c.tamanyo++;
                    anyadido = true;
                }
            }
        }
    }


    return anyadido;
}

/*
    Pre: true
    Post: Devuelve verdad y la colección c con el item con identificador ident borrado sólo si se ha hallado
            dicho item; devuelve falso y la misma colección c en caso contrario
*/

template<typename T> bool borrar(coleccionITF<T>& c, string ident){
    bool borrado = false;
    
    // Caso de error: La colección está vacía o el primer elemenento ya tiene un identificador mayor que el buscado 
    if(c.tamanyo != 0 && identificador(c.primero -> item) <= ident){
        typename coleccionITF<T>::unItem* pAux1; // Crea un puntero auxiliar

        // Caso 1: El primer elemento es el buscado
        if(identificador(c.primero -> item) == ident){
            pAux1 = c.primero;                                  // pAux1 ahora apunta al primer elemento
            c.primero = c.primero -> siguiente;                 // El puntero al primer elemento apunta al segundo
            delete pAux1;                                       // Libera la memoria ocupada por la primera celda
            c.tamanyo--;
            borrado = true;
        }
        
        else{
            bool parar = false;
            pAux1 = c.primero -> siguiente;                         // Ahora pAux1 apunta al segundo elemento
            typename coleccionITF<T>::unItem* pAux2 = c.primero;    // Y pAux2 al primero

            
            while(pAux1 != nullptr && !parar){

                // Caso de error: Ningún identificador menor que el buscado coincidía
                if(ident < identificador(pAux1 -> item)){           
                    parar = true;
                }

                // Caso 2: El elemento se encuentra en la cola de la lista
                else if(ident == identificador(pAux1 -> item)){
                    pAux2 -> siguiente = pAux1 -> siguiente;        // El elemento anterior al buscado ahora apunta al siguiente
                    delete pAux1;                                   // del buscado, y el buscado queda eliminado
                    c.tamanyo--;
                    parar = true;
                    borrado = true;
                }

                else{                                               // Seguir:
                    pAux2 = pAux1;                                  // pAux2 apunta al elemento visitado más reciente
                    pAux1 = pAux1 -> siguiente;                     // y pAux1 al siguiente no visitado aún
                }
            }
        }
    }

    return borrado;
}


/*
    Pre: true
    Post: Devuelve verdad y el ítem i con identificador ident solo si se ha encontrado en la colección c dicho item;
            devuelve falso y el mismo item i en caso contrario
*/

template<typename T> bool esta(const coleccionITF<T>& c, T& i, string ident){
    bool encontrado = false;
    typename coleccionITF<T>::unItem* pAux = c.primero; // Puntero auxiliar apunta a la primera celda

    // Itera hasta que llega al final o encuentra un identificador menor que el buscado 
    while(pAux != nullptr && identificador(pAux -> item) < ident){
        pAux = pAux -> siguiente;
    }

    // Si no es el final y son iguales, devuelve el item buscado
    if(pAux != nullptr && identificador(pAux -> item) == ident){
        i = pAux -> item;
        encontrado = true;
    }
    
    return encontrado;
}

/*
    Pre: true
    Post: Devuelve la cadena formada por los items de la colección c cuya fecha sea anterior a la fecha f,
            separados por la cadena separador 
*/

template<typename T> string obsoletos(const coleccionITF<T>& c, Fecha f, string separador){
    string s = ""; 

    if(c.tamanyo != 0){
        bool primero = false;
        typename coleccionITF<T>::unItem* pAux = c.primero;
        while(pAux != nullptr){ // Recorrerá toda la lista, empezando con el primero
            if(anterior(fecha(pAux -> item), f)){ // Comprueba si el item contenido es anterior a la fecha
                if(!primero){   // No añade el separador si es el primer elemento concatenado
                    s = s + identificador(pAux -> item);
                    primero = true;
                }

                else{
                    s = s + separador + identificador(pAux -> item);
                }
            }
             pAux = pAux -> siguiente;  // Avanza a la siguiente celda; si es la última,
                                        // la siguiente será un puntero nulo
        }
    }

    return s;
}

/*
    Pre: true
    Post: Devuelve verdad solo si ha devuelto una colección de la que se han eliminado los items cuya fecha de
            creación es anterior a la fecha f. En caso contrario devuelve falso y la misma colección
*/

template<typename T> bool purgar(coleccionITF<T>& c, Fecha f){
    // Similar a la función borrar()
   
    bool purgadoAlguno = false;
    if(c.tamanyo != 0){
        typename coleccionITF<T>::unItem* nuevo = new typename coleccionITF<T>::unItem; // Item vacío auxiliar
        nuevo -> siguiente = c.primero; // El nuevo item apuntará al primer item
        c.primero = nuevo;              // Y nuevo susitituirá como primer ítem
        typename coleccionITF<T>::unItem* pAux1 = c.primero -> siguiente; // Crea un puntero pAux1 que apunta al segundo elemento
        typename coleccionITF<T>::unItem* pAux2 = c.primero;                // Y pAux2 al primero

        while(pAux1 != nullptr){    // Itera hasta que pAux1 ha recorrido toda la lista
        
            // Caso 2: El elemento se encuentra en la cola de la lista
            if(anterior(fecha(pAux1 -> item), f)){
                pAux2 -> siguiente = pAux1 -> siguiente;        // El elemento anterior al buscado ahora apunta al siguiente
                delete pAux1;                                   // del buscado, y el buscado queda eliminado
                c.tamanyo--;
                pAux1 = pAux2 -> siguiente;                     // pAux1 también apunta al elemento siguiente del eliminado
                purgadoAlguno = true;
            } 

            else{
                pAux2 = pAux1;                                  // pAux2 apunta al elemento visitado más reciente
                pAux1 = pAux1 -> siguiente;                     // y pAux1 al siguiente no visitado aún
            }
        }

        c.primero = nuevo -> siguiente; // El puntero primero volvera a apuntar al primer elemento real
        delete nuevo;                   // Elimina el espacio reservado por nuevo
    }

    return purgadoAlguno;
}

/*
    Pre: true
    Post: Devuelve la cadena formada por los items de la colección cuyo tema sea igual al tema t,
            separados por la cadena separador
*/

template<typename T> string delTema(const coleccionITF<T>& c, string t, string separador){
    string s = ""; 
    if(c.tamanyo != 0){
        bool primero = false;
        typename coleccionITF<T>::unItem* pAux = c.primero;
        // Similar a la función obsoletos()
        while(pAux != nullptr){
            if(tema(pAux -> item) == t){    // Comprueba si el tema del item contenido es igual a t
                if(!primero){
                    s = s + identificador(pAux -> item);
                    primero = true;
                }

                else{
                    s = s + separador + identificador(pAux -> item);
                }
  
            }

            pAux = pAux -> siguiente;
        }
    }

    return s;
}

/*
    Pre: true
    Post: Devuelve verdad solo si el tamaño de la colección c no es igual a 0
*/

template<typename T> bool esVacia(const coleccionITF<T>& c){
    return c.tamanyo == 0;
}

/*
    Pre: true
    Post: Devuelve el número de items que actualmente contiene la colección c
*/

template<typename T> int tamanyo(const coleccionITF<T>& c){
    return c.tamanyo;
}

/*
    Pre: true
    Post: El puntero iterador apuntará al primer ítem (situación de no haber visitado
            ningún elemento)
*/

template<typename T> void iniciarIterador(coleccionITF<T>& c){
    c.iter = c.primero;
}

/* 
    Pre: true
    Post: Devuelve verdad solo si el puntero iterador no tiene un valor nulo;
*/

template<typename T> bool haySiguiente(const coleccionITF<T>& c){
    return c.iter != nullptr;
}

/*
    Pre: true
    Post: Devuelve verdad solo si existe el siguiente item; si es el caso, obtiene el siguiente
            ítem y el puntero iterador apunta al próximo elemento de la lista
*/


template<typename T> bool siguiente(coleccionITF<T>& c, T& i){
    if(haySiguiente(c)){
        i = c.iter -> item;
        c.iter = c.iter -> siguiente;
        return true;
    }

    else{
        return false;
    }
}



#endif