/*	
 * 		Inteligencia Artificial
 * 		Trabajo práctico (parte 1)
 * 		
 * 		Autor: Dorian Boleslaw Wozniak	(817570@unizar.es)
 *
 *		Descripción: Clase que contiene el estado objetivo del problema de los
 *						canibales, junto a un método que comprueba si el 
 *						estado final ha sido alcanzado
 */

package aima.core.environment.canibales;

import aima.core.search.framework.GoalTest;

public class CanibalesGoalTest implements GoalTest {
	
	private final int DESTINO = 1;
	
	// La tupla objetivo tiene a misioneros, canibales y la barca en la orilla destino  
	CanibalesBoard goal = new CanibalesBoard(new int[] {0, 0,3, 3, DESTINO});
	
	// Comprueba que las tuplas estado y objetivo coincidan
	public boolean isGoalState(Object state) {
		CanibalesBoard board = (CanibalesBoard) state;
		return board.equals(goal);
	}
}
