//*****************************************************************
// File:   SymbolTable.java
// Author: Procesadores de Lenguajes-University of Zaragoza
// Date:   julio 2021
// Coms:   Atributos públicos para ahorrarnos el uso de getters y setters
//*****************************************************************

//la tabla de símbolos será un ArrayList de diccionarios (HashMap<String, Symbol>), manejada como
//una pila: se inserta y accede por la derecha
//Cada nuevo bloque se apilará, guardando los símbolos en el diccionario correspondiente
//El constructor ya genera el primer bloque, vacío inicialmente.

//https://quick-adviser.com/can-a-hashmap-have-multiple-values-for-same-key/
//HashMap doesn’t allow duplicate keys but allows duplicate values. That means A 
//single key can’t contain more than 1 value but more than 1 key can contain a single value.
//HashMap allows null key also but only once and multiple null values.
//https://docs.oracle.com/javase/8/docs/api/java/util/HashMap.html

package lib.symbolTable;

import java.util.*;
import lib.symbolTable.exceptions.SymbolNotFoundException;
import lib.symbolTable.exceptions.AlreadyDefinedSymbolException;

/**
 * TAD Tabla de símbolos. Es una pila de diccionarios de parejas clave-símbolo.
 * La tabla realiza sus operaciones de derecha a izquierda, apilando y
 * desapilando bloques cada vez que empiece o acabe la sección.
 * 
 * La tabla no permite símbolos con la misma clave, pero si símbolos con los
 * mismos valores.
 */
public class SymbolTable {

    /** Nivel máximo de bloques en la tabla */
    private final int ST_SIZE = 16;

    /** Tamaño total del hashmap */
    private final int HASH_SIZE = 1024;

    /**
     * Tabla de símbolos. Es un vector de diccionarios clave-símbolo que
     * actúa como una pila a medida que se añaden y eliminan bloques.
     */
    private ArrayList<HashMap<String, Symbol>> st;

    /** Conjunto de palabras reservadas */
    private Set<String> reservedWords;

    /** Nivel actual */
    public int level;

    /**
     * Crea una nueva tabla de símbolos
     */
    public SymbolTable() {
        st = new ArrayList<HashMap<String, Symbol>>(ST_SIZE);
        level = -1;
        insertBlock();
        reservedWords = new HashSet<String>();
    }

    /**
     * Crea una nueva tabla de símbolos
     * 
     * @param pals Conjunto de palabras reservadas de la tabla
     */
    public SymbolTable(String[] pals) {
        st = new ArrayList<HashMap<String, Symbol>>(ST_SIZE);
        level = -1; // aún no hay ningún bloque introducido
        insertBlock();
        reservedWords = new HashSet<String>(Arrays.asList(pals));
    }

    /**
     * Apila un nuevo bloque a la tabla
     */
    public void insertBlock() {
        st.add(new HashMap<String, Symbol>(HASH_SIZE));
        level++;
    }

    /**
     * Desapila el último bloque de la tabla
     */
    public void removeBlock() {
        st.remove(st.size() - 1);
        level--;
    }

    /**
     * Añade un conjunto de palabras reservadas a la tabla, sobrescribiendo
     * la anterior si existiera.
     * 
     * @param pals Conjunto de palabras reservadas de la tabla
     */
    public void insertReservedWords(String[] pals) {
        reservedWords = new HashSet<String>(Arrays.asList(pals));
    }

    /**
     * Comprueba si una cadena es una palabra reservada
     * 
     * @param word Una cadena cualquiera
     * @return Verdad sólo si no pertenece al conjunuto de palabras reservadas
     */
    public boolean isReservedWord(String word) {
        return reservedWords.contains(word);
    }

    /**
     * Inserta un nuevo símbolo en el bloque actual de la tabla.
     * 
     * @param s Símbolo a añadir
     * @throws AlreadyDefinedSymbolException Hay otro símbolo con el mismo
     *                                       nombre o el nombre es una palabra
     *                                       reservada
     */
    public void insertSymbol(Symbol s) throws AlreadyDefinedSymbolException {
        if (reservedWords.contains(s.name)) {
            throw new AlreadyDefinedSymbolException();
        }
        HashMap<String, Symbol> currentBlock = st.get(st.size() - 1);
        if (currentBlock.containsKey(s.name)) { // ya está
            throw new AlreadyDefinedSymbolException();
        } else {
            s.nivel = level;
            currentBlock.put(s.name, s);
        }
    }

    /**
     * Devuelve un símbolo de la tabla dado su nombre.
     * 
     * 
     * @param name Nombre del símbolo
     * @return Símbolo de la tabla encontrado
     * @throws SymbolNotFoundException No se ha encontrado un símbolo con el
     *                                 mismo nombre
     */
    public Symbol getSymbol(String name) throws SymbolNotFoundException {
        Symbol result = findSymbol(name);
        if (result == null) {
            throw new SymbolNotFoundException();
        }
        return result;
    }

    /**
     * Comprueba si un símbolo se encuentra en la tabla.
     * 
     * @param name Nombre del símbolo
     * @return Verdad sólo si se encuentra en la tabla.
     */
    public boolean containsSymbol(String name) {
        return findSymbol(name) != null;
    }

    /**
     * Busca un símbolo en la tabla dado un nombre
     * 
     * @param name Nombre del símbolo
     * @return El símbolo de la tabla, o null si no lo ha encontrado
     */
    private Symbol findSymbol(String name) {
        for (int i = st.size() - 1; i >= 0; i--) {
            if (st.get(i).containsKey(name)) {
                return st.get(i).get(name);
            }
        }
        return null;
    }

    /**
     * Devuelva la tabla como una cadena
     */
    public String toString() {
        final String linea = "------------------------------------------------\n";
        StringBuilder builder = new StringBuilder();
        builder.append(linea);
        String tabs = "";
        for (int i = 0; i < st.size(); i++) {
            for (Map.Entry entry : st.get(i).entrySet()) {
                // crear secuencia de tabuladores
                tabs = new String(new char[i]).replace("\0", "\t");
                builder.append(tabs);
                builder.append(entry.toString());
                builder.append("\n");
            }
        }
        builder.append(linea);
        return builder.toString();
    }
}