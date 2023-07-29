/* 
 * Inteligencia Artificial
 * Trabajo TP6 - Primera parte
 * 
 * Fichero: SudokuConstraint.java
 * 
 * Autor: Dorian Boleslaw Wozniak (817570@unizar.es)
 * 
 * Descripción: La clase define una restricción sobre dos celdas de un sudoku
 * 					tal que no puedan ser iguales
 * */

package aima.gui.sudoku.csp;

import java.util.ArrayList;
import java.util.List;

import aima.core.search.csp.Assignment;
import aima.core.search.csp.Constraint;
import aima.core.search.csp.Variable;

public class SudokuConstraint implements Constraint{

	private Variable var1, var2;	// Variables que definen restricción
	private List<Variable> scope;	// Ámbito de la restricción
	
	// Constructor
	public SudokuConstraint(Variable var1, Variable var2) {
		this.var1 = var1; this.var2 = var2;
		this.scope = new ArrayList<Variable>(2);
		scope.add(var1); scope.add(var2);
	}
	
	// Devuelve el ámbito de la resticción
	@Override
	public List<Variable> getScope() { return (List<Variable>) scope; }

	// Devuelve si el valor de dos celdas es diferente
	@Override
	public boolean isSatisfiedWith(Assignment assigment) {		
		Object value1 = assigment.getAssignment(var1);
		Object value2 = assigment.getAssignment(var2);
		return value1 == null || !value1.equals(value2);
	}
}
