//*****************************************************************
// File:   SymbolInt.java
// Author: Procesadores de Lenguajes-University of Zaragoza
// Date:   julio 2021
// Coms:   Atributos públicos para evitar el uso de getters y setters
//*****************************************************************

package lib.symbolTable;

/**
 * Subclase de simbolo para datos simples de tipo entero (int).
 */
public class SymbolInt extends Symbol implements Cloneable {

    /** Valor del símbolo */
    public int value;

    /**
     * Constructor de un nuevo símbolo int
     * 
     * @param _name Nombre del símbolo
     */
    public SymbolInt(String _name) {
        super(_name, Types.INT, ParameterClass.NONE);
        value = 0;
    }

    /**
     * Constructor de un nuevo símbolo int
     * 
     * @param _name  Nombre del símbolo
     * @param _class Clase de parámetro del símbolo
     */
    public SymbolInt(String _name, ParameterClass _class) {
        super(_name, Types.INT, _class);
        value = 0;
    }

    /**
     * Constructor de un nuevo símbolo int
     * 
     * @param _name  Nombre del símbolo
     * @param _value Valor del símbolo
     * @param _class Clase de parámetro del símbolo
     */
    public SymbolInt(String _name, int _value, ParameterClass _class) {
        super(_name, Types.INT, _class);
        value = _value;
    }

    public String toString() {
        return "(" + name + "," + type + "," + value + "," + parClass + "," + nivel + ")";
    }

    public SymbolInt clone() {
        return (SymbolInt) super.clone();
    }
}
