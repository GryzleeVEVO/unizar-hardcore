//*****************************************************************
// File:   Symbol.java
// Author: Procesadores de Lenguajes-University of Zaragoza
// Date:   julio 2021
// Coms:   Atributos públicos para evitar el uso de getters y setters
//         Dado que los símbolos se almacenarán en un diccionario, nunca
//         se accederá a ellos sin conocer su nombre, por lo que el atributo
//         nombre es innecesario
//*****************************************************************

package lib.symbolTable;

abstract public class Symbol implements Cloneable {

    /** Clases de parámetros */
    static public enum ParameterClass {
        VAL,
        REF,
        NONE
    }

    /** Tipo de símbolo */
    static public enum Types {
        INT,
        BOOL,
        CHAR,
        ARRAY,
        FUNCTION,
        PROCEDURE,
        STRING,
        UNDEFINED
    }

    /** Nombre del símbolo */
    public String name;

    /** Clase de parámetro (NONE si no lo es) */
    public ParameterClass parClass;

    /** @ Memoria (@ primera instr. para funciones) */
    public long dir;

    /** Tipo del símbolo */
    public Types type;

    /** Indica si es un símbolo constante */
    public boolean constant;

    /** Nivel en la tabla de símbolos */
    public int nivel;

    /**
     * Constructor de un nuevo símbolo
     * 
     * @param _name Nombre del símbolo
     * @param _type Tipo del símbolo
     */
    public Symbol(String _name, Types _type) {
        this(_name, -1, _type, ParameterClass.NONE, false);
    }

    /**
     * Constructor de un nuevo símbolo
     * 
     * @param _name     Nombre del símbolo
     * @param _type     Tipo del símbolo
     * @param _parClass Clase de parámetro del símbolo
     */
    public Symbol(String _name, Types _type, ParameterClass _parClass) {
        this(_name, -1, _type, _parClass, false);
    }

    /**
     * Constructor de un nuevo símbolo
     * 
     * @param _name     Nombre del símbolo
     * @param _dir      Dirección del símbolo
     * @param _type     Tipo del símbolo
     * @param _parClass Clase de parámetro del símbolo
     * @param _constant Símbolo constante
     */
    public Symbol(String _name, long _dir, Types _type, ParameterClass _parClass, boolean _constant) {
        name = _name;
        dir = _dir;
        type = _type;
        parClass = _parClass;
        constant = _constant;
    }

    /**
     * Crea una copia del símbolo.
     */
    public Symbol clone() {
        try {
            return (Symbol) super.clone();
        } catch (CloneNotSupportedException e) {
            return null;
        }
    }

    /**
     * Devuelve el símbolo como una cadena de caracteres.
     */
    abstract public String toString();

}
