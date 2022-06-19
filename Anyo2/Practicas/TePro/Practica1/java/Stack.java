/**
 *  Stack.java
 * 
 *  Dorian Boleslaw Wozniak 817570
 *  Practica 1 Tecnología de Programación
 * 
 *  Implementación de una interfaz Stack, que habilita mediante herencia el
 *  poliformismo para las clases StaticStack y DynamicStack 
 */

import java.lang.Iterable;

public interface Stack<T> extends Iterable<T>{
    public boolean push(T t);
    public boolean pop();
}