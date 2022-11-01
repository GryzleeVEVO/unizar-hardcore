/* 
 * Inteligencia Artificial
 * Trabajo TP6 - Primera parte
 * 
 * Fichero: SudokuVariable.java
 * 
 * Autor: Dorian Boleslaw Wozniak (817570@unizar.es)
 * 
 * Descripción: La clase define una variable de sudoku, que es un subtipo de
 * 					Variable que además contiene su valor numérico y su 
 * 					posición mediante coordenadas
 * */

package aima.gui.sudoku.csp;

import aima.core.search.csp.Variable;

public class SudokuVariable extends Variable {

	
	private int value; 	// Valor de la posición
	private int x;	   	// Coordenada x
	private int y;	   	// Coordenada y
	
	// Constructor
	public SudokuVariable(String name, int x, int y) 
		{ super(name); this.x = x; this.y= y; }
	
	// Getters
	public int getValue() { return value; }
	public int getX() { return x; }
	public int getY() { return y; }
	
	// Setters
	public void setValue(int value) { this.value = value; }
}
