/**
 * @file dynamic-stack.h
 * @author Dorian Boleslaw Wozniak  817570
 * @brief   Practica 1 Tecnología de Programación
 * 
 *          Implementación de una clase dynamic_stack, que representa una pila
 *          dinámica que implementa operaciones push, pop y un iterador.
 * @version 1
 * @date 2022-02-25
 */

#pragma once

/**
 * @brief Clase dynamic_stack. Implementa una pila generica mediante una lista
 *          enlazada
 * @tparam T Tipo genérico 
 */
template<typename T>
class dynamic_stack{

    private:

        /**
         * @brief Clase Node. Implementa un nodo de una lista enlazada para
         *          implementar la pila genérica
         */
        class Node{

            public:
                T elem;         // Elemento
                Node* next;     // Puntero a siguiente nodo

                /**
                 * @brief Constructor de un nuevo nodo
                 * 
                 * @param e Elemento a introducir
                 * @param nPtr Siguiente nodo en la lista
                 */
                Node(const T& e, Node* nPtr) : elem(e), next(nPtr) {}
                
        };

        int total;              // Total de elementos
        Node* first;            // Puntero al primer nodo

    public:

        /**
         * @brief Constructor de una clase dynamic_stack. Por defecto, 
         *          total = 0 y no apunta a ningún nodo
         */
        dynamic_stack() : total(0), first(nullptr) {}
        
        /**
         * @brief Destructor de una clase dynamic_stack. Vacía la pila
         */
        ~dynamic_stack(){
            while(first != nullptr){
                pop();
            }

        }

        /**
         * @brief Operación de apilado. Introduce un nuevo elemento sobre el
         *          resto de elementos de la lista enlazada
         * @param e Elemento
         * @return true si ha logrado introducir el elemento 
         */
        bool push(const T& e){
            Node* aux = first;
            first = new Node(e, aux);
            total++;
            return true;
        }

        /**
         * @brief Operación de desapilado. Retira el primer elemento de la lista
         *          enlazada
         * @return true si la pila no estaba vacía y se ha desapilado un 
         *          elemento 
         */
        bool pop(){
            if(first != nullptr){
                Node* aux = first;
                first = first -> next;
                delete aux;
                total--;
                return true;
            }

            else{ return false; }
        }


        friend class iterator;

        /**
         * @brief Clase iterator. Implementa un iterador para su uso sobre
         *          dynamic_stack. 
         */
        class iterator{

            private:

                Node* i;            // Puntero a nodo actual

            public:

                /**
                 * @brief Constructor de iterador
                 * @param ptr Puntero a siguiente elemento 
                 */
                iterator(Node* ptr) : i(ptr) {}

                /**
                 * @brief Operador de avance
                 * @return iterador tal que el elemento actual es el siguiente
                 *          al iterator que lo llama 
                 */
                iterator& operator++(){ i = i -> next; return (*this); }

                /**
                 * @brief Operador de dereferenciado
                 * @return Elemento apuntado por el iterador 
                 */
                const T& operator*() const{ return (i -> elem); }

                /**
                 * @brief Operador de desigualdad
                 * @param that iterator a comparar con el que llama
                 * @return true si ambos iteradores apuntan al mismo nodo
                 *          (o a nada) 
                 */
                bool operator!=(const iterator& that) const{
                    return i != that.i;
                }

        };

        /**
         * @brief Inicialización de iterador al inicio de la pila
         * @return iterator tal que apunte al primer elemento de la pila 
         */
        iterator begin() const{ return iterator(first); }

        /**
         * @brief Inicialización de iterador al final de la pila
         * @return iterator tal que apunte al fondo de la pila (nullptr)
         */
        iterator end() const{ return iterator(nullptr); }

};