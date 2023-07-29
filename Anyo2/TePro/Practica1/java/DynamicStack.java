/**
 * DynamicStack.java
 * Dorian Boleslaw Wozniak  817570
 * Practica 1 Teconlogía de Programación
 * 
 * Implementación de una clase DynamicStack, que representa una pila
 * dinámica que implementa operaciones push, pop y un iterador.
 */

import java.util.Iterator;
import java.util.NoSuchElementException;
import java.lang.UnsupportedOperationException;


/**
 * Clase DynamicStack. Implementa una pila genérica mediante una lista enlazada
 * 
 * Donde T es un tipo genérico
 */
public class DynamicStack<T> implements Stack<T>{
    
    /**
     * Clase Node. Implementa un nodo de una lista enlazada para implementar
     * la plia genérica
     */
    private class Node{

        /** Elemento */ 
        public T elem; 
        /** Siguiente nodo de la lista */
        public Node next;          
        
        /**
         * Constructor de un nuevo nodo.
         * 
         * @param t Elemento
         * @param n Siguiente nodo de la lista
         */
        public Node(T t, Node n){
            elem = t; next = n;
        }

    }

    /** Total de elementos */
    private int total;
    /** Primer nodo */
    private Node first;

    /** 
     * Constructor de una nueva clase DynamicStack.
     *
     * Por defecto, total = 0 y first no inicializado  
     */
    public DynamicStack(){
        total = 0;
    }

    /**
     * Operación de apilado. Introduce un nuevo elemento sobre el resto de
     * elementos de la lista enlazada
     * 
     * Donde t es un elemento.
     * 
     * Devuelve true si se ha logrado introducir el elemento
     */
    public boolean push(T t){
        Node aux = first;
        first = new Node(t, aux);
        total++;
        return true;
    }

    /**
     * Operación de desapilado. Retira el primer elemento de la lista enlazada. 
     * 
     * Duvuelve true si la pila no estaba vacía y se ha desapilado un elemento. 
     */
    public boolean pop(){
        if(total != 0) { first = first.next; total--; return true;}
        else { return false; }
    }

    /**
     * Clase StackIterator.
     * Implementa un iterador para su uso sobre DynamicStack
     */
    private class StackIterator implements Iterator<T>{

            /** Nodo actual */
            Node curr;
            /** Posición cardinal en la lista enlazada */
            int i;
        
            /**
             * Constuctor de iterator
             * 
             * @param stk Pila a iterar
             */
            private StackIterator(DynamicStack<T> stk){
                curr = stk.first; i = total;
            }
            
            /**
             * Devuelve true si quedan elementos por iterar en la lista
             */
            public boolean hasNext(){
                return i > 0;
            }

            /**
             * Devuelve el elemento de la lista sobre el que se encuentra el
             * iterador.
             * 
             * Lanza una excepción en caso de no haber siguiente elemento
             */
            public T next() throws NoSuchElementException{
                if(!hasNext()){
                    throw new NoSuchElementException();
                }

                else{
                    T elem = curr.elem;
                    curr = curr.next;
                    i--;
                    return elem;
                }
            }

            /** Operación de eliminación de elemento. Al no ser usada, lanza
             *  una excepción 
             */
            public void remove() throws UnsupportedOperationException{
                throw new UnsupportedOperationException(); 
            }
    }

    /**
     * Inicia el iterador sobre la pila
     */
    public Iterator<T> iterator(){
        return new StackIterator(this);
    }
}
