/*	
 * 		Inteligencia Artificial
 * 		Trabajo práctico (parte 1)
 * 		
 * 		Autor: Dorian Boleslaw Wozniak	(817570@unizar.es)
 *
 *		Descripción: Clase que implementa el estado para un problema de las
 *						fichas, junto a métodos para:
 *						
 *						- Comprobar si un movimiento es válido canMove()
 *						- Modificar el estado move()
 *						- Obtener hashes a partir del estado hashCode()
 *						- Obtener una descripción textual del estado 
 *							toString()
 *						- Una función de comparación equals()
 */

package aima.core.environment.fichas;

import java.util.Arrays;

import aima.core.agent.Action;
import aima.core.agent.impl.DynamicAction;

public class FichasBoard {
	
	// ----- CONSTANTES -----
	private final int X = 0;	// Posición vacía
	private final int B = 1;	// Ficha B
	private final int V = 2;	// Ficha V
	
	private final int I = 0;	// Izquierda
	private final int D = 1;	// Derecha
	
	private final int BOARD_SIZE = 7; // Tamaño del tablero
	
	// ----- ACCIONES DISPONIBLES PARA EL AGENTE -----
	public static Action MB1I = new DynamicAction("MB1I");
	public static Action MB2I = new DynamicAction("MB2I");
	public static Action MB3I = new DynamicAction("MB3I");
	public static Action MB1D = new DynamicAction("MB1D");
	public static Action MB2D = new DynamicAction("MB2D");
	public static Action MB3D = new DynamicAction("MB3D");
	public static Action MV1I = new DynamicAction("MV1I");
	public static Action MV2I = new DynamicAction("MV2I");
	public static Action MV3I = new DynamicAction("MV3I");
	public static Action MV1D = new DynamicAction("MV1D");
	public static Action MV2D = new DynamicAction("MV2D");
	public static Action MV3D = new DynamicAction("MV3D");	

	// TUPLA ESTADO: (0, 1, 2, 3, 4, 5, 6)
	private int[] state;
	
	// ----- CONSTRUCTORES -----
	
	// Por defecto
	public FichasBoard() { state = new int[] {B, B, B, X, V, V, V}; }
	
	// Estado inicial personalizado (no comprueba que sea una tupla legal)
	public FichasBoard(int[] state) {
		this.state = new int[state.length];
		System.arraycopy(state, 0, this.state, 0, state.length);
	}
	
	// A partir de otra tupla
	public FichasBoard(FichasBoard copyBoard) { this(copyBoard.getState()); }
	
	// Devuelve el estado actual de la clase
	public int[] getState() { return state; }
	
	// ----- FUNCIÓN move() -----
	
	// Intercambia posición del hueco con una ficha a n pasos en dirección dir
	public void move(int dir, int n) {
		int gap = getGapPos();
		if (dir == I) 
			{ state[gap] = state[gap + n]; state[gap + n] = X; }
		else 
			{ state[gap] = state[gap - n]; state[gap - n] = X; }
	}
	
	// ----- FUNCIÓN CANMOVE() -----
	
	// Comprueba la validez de un movimiento	
	public boolean canMove(Action where) {
		if 		(where.equals(MB1I)) return canMove(I, 1, B);
		else if (where.equals(MB2I)) return canMove(I, 2, B);
		else if (where.equals(MB3I)) return canMove(I, 3, B);
		else if (where.equals(MB1D)) return canMove(D, 1, B);
		else if (where.equals(MB2D)) return canMove(D, 2, B);
		else if (where.equals(MB3D)) return canMove(D, 3, B);
		else if (where.equals(MV1I)) return canMove(I, 1, V);
		else if (where.equals(MV2I)) return canMove(I, 2, V);
		else if (where.equals(MV3I)) return canMove(I, 3, V);
		else if (where.equals(MV1D)) return canMove(D, 1, V);
		else if (where.equals(MV2D)) return canMove(D, 2, V);
		else if (where.equals(MV3D)) return canMove(D, 3, V);		
		else return false;
	}

	// Para que un movimiento sea válido:
	//		- La ficha a cambiar debe estar en el tablero, es decir, su indice
	//			se encuentra entre 0 y BOARD_SIZE, según se mueva la ficha a
	//			la izquierda o a la derecha
	//		- La posición a +- n pasos de la posición del hueco debe ser del
	//			mismo tipo que la ficha que se quiere mover
	public boolean canMove(int dir, int n, int type) {
		int gap = getGapPos();
		
		if (dir == I)
			return (gap + n < BOARD_SIZE) && (state[gap + n] == type);
		else
			return (gap - n >= 0) && (state[gap - n] == type);
	}

	// Obtiene la posición del hueco, que se usa como base para mover
	private int getGapPos() {
		for (int i = 0; i < BOARD_SIZE; i++)
			if (state[i] == X) return i;
		return -1;
	}

	// ----- FUNCIONES ADICIONALES -----

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + Arrays.hashCode(state);
		return result;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj) return true;
		if (obj == null) return false;
		if (getClass() != obj.getClass()) return false;
		FichasBoard other = (FichasBoard) obj;
		return Arrays.equals(state, other.state);
	}

	@Override
	public String toString() {
		String str = "+---+---+---+---+---+---+---+\n";
		for (int i = 0; i < BOARD_SIZE; i++) {
			str += "|";
			switch (state[i]) {
				case X: str += "   "; break;
				case B: str += " B "; break;
				case V: str += " V "; break;
			}
		}
		str += "|\n+---+---+---+---+---+---+---+\n";

		
		return str;
	}
	
	
	
	
}
