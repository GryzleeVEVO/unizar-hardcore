/*	
 * 		Inteligencia Artificial
 * 		Trabajo práctico (parte 1)
 * 		
 * 		Autor: Dorian Boleslaw Wozniak	(817570@unizar.es)
 *
 *		Descripción: Clase que contiene el estado objetivo del problema de las
 *						fichas, junto a un método que comprueba si el 
 *						estado final ha sido alcanzado
 */


package aima.core.environment.fichas;

import aima.core.search.framework.GoalTest;

public class FichasGoalTest implements GoalTest {
	
	// CONSTANTES
	private final int X = 0; // Hueco
	private final int B = 1; // Ficha B
	private final int V = 2; // Ficha V
	
	// La tupla tiene las fichas en lados opuestos al estado inicial
	FichasBoard goal = new FichasBoard(new int[] {V, V, V, X, B, B, B});
	
	// Comprueba que las tuplas estado y objetivo coincidan
	public boolean isGoalState(Object state) {
		FichasBoard board = (FichasBoard) state;
		return board.equals(goal);
	}
}
