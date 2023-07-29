/*
 * 		Inteligencia Artificial
 * 		Trabajo práctico (parte 1)
 *
 * 		Autor: Dorian Boleslaw Wozniak	(817570@unizar.es)
 *
 *		Descripción: Clase que implementa el estado para un problema de los
 *						canibales, junto a métodos para:
 *
 *						- Comprobar si un movimiento es válido canMove()
 *						- Modificar el estado moveX()
 *						- Obtener hashes a partir del estado hashCode()
 *						- Obtener una descripción textual del estado
 *							toString()
 *						- Una función de comparación equals()
 */

package aima.core.environment.canibales;

import java.util.Arrays;

import aima.core.agent.Action;
import aima.core.agent.impl.DynamicAction;

public class CanibalesBoard {

	// ----- CONSTANTES -----
	private final int ORIGEN = 0; // Barca en orilla origen
	private final int DESTINO = 1; // Barca en orilla destino

	private final int MO = 0; // Misioneros en orilla origen
	private final int CO = 1; // Canibales en orilla origen
	private final int MD = 2; // Misioneros en orilla destino
	private final int CD = 3; // Canibales en orilla destino
	private final int LB = 4; // Lado donde está la barca

	// ----- ACCIONES DISPONIBLES PARA EL AGENTE -----
	public static Action M1M = new DynamicAction("M1M");
	public static Action M1C = new DynamicAction("M1C");
	public static Action M2M = new DynamicAction("M2M");
	public static Action M2C = new DynamicAction("M2C");
	public static Action M1M1C = new DynamicAction("M1M1C");

	// TUPLA ESTADO: (MO, CO, MD, MD, LB)
	private int[] state;

	// ----- CONSTRUCTORES -----

	// Por defecto
	public CanibalesBoard() {
		state = new int[] { 3, 3, 0, 0, ORIGEN };
	}

	// Estado inicial personalizado (no comprueba que sea una tupla legal)
	public CanibalesBoard(int[] state) {
		this.state = new int[state.length];
		System.arraycopy(state, 0, this.state, 0, state.length);
	}

	// A partir de otra tupla
	public CanibalesBoard(CanibalesBoard copyBoard) {
		this(copyBoard.getState());
	}

	// Devuelve el estado actual de la clase
	public int[] getState() {
		return state;
	}

	// ----- FUNCIONES moveX() -----
	// Modifican el estado según el movimento a realizar

	// Mover n misionarios
	public void moveM(int n) {
		if (state[LB] == ORIGEN) {
			state[MO] -= n;
			state[MD] += n;
			state[LB] = DESTINO;
		} else {
			state[MD] -= n;
			state[MO] += n;
			state[LB] = ORIGEN;
		}
	}

	// Mover n canibales
	public void moveC(int n) {
		if (state[LB] == ORIGEN) {
			state[CO] -= n;
			state[CD] += n;
			state[LB] = DESTINO;
		} else {
			state[CD] -= n;
			state[CO] += n;
			state[LB] = ORIGEN;
		}
	}

	// Mover un misionario y un canibal
	public void moveMC() {
		if (state[LB] == ORIGEN) {
			state[MO] -= 1;
			state[MD] += 1;
			state[CO] -= 1;
			state[CD] += 1;
			state[LB] = DESTINO;
		} else {
			state[MD] -= 1;
			state[MO] += 1;
			state[CD] -= 1;
			state[CO] += 1;
			state[LB] = ORIGEN;
		}
	}

	// ----- FUNCIÓN CANMOVE() -----

	// Comprueba la validez de un movimiento
	public boolean canMove(Action where) {
		if (where.equals(M1M))
			return canMoveM(1);
		else if (where.equals(M2M))
			return canMoveM(2);
		else if (where.equals(M1C))
			return canMoveC(1);
		else if (where.equals(M2C))
			return canMoveC(2);
		else if (where.equals(M1M1C))
			return canMoveMC();
		else
			return false;
	}

	// Para que un movimiento sea válido:
	// - En todos los casos debe haber suficientes misioneros/canibales
	// para hacer el movimiento

	// Para mover misionarios además
	// - Se mueven todos los misionarios de la orilla inicial o tras el
	// movimiento sigue habiendo mas misionarios que canibales en la
	// orilla inicial
	// - Hay tantos o mas misionarios como canibales en la otra orilla
	private boolean canMoveM(int n) {
		if (state[LB] == ORIGEN)
			return state[MO] >= n &&
					((state[MO] == n || state[MO] - n >= state[CO]) &&
							state[MD] + n >= state[CD]);
		else
			return state[MD] >= n &&
					((state[MD] == n || state[MD] - n >= state[CD]) &&
							state[MO] + n >= state[CO]);
	}

	// Para mover canibales admás
	// - No debe haber misionarios en la otra orilla o tras el movimiento
	// sigue habiendo tantos o mas misionarios que canibales
	private boolean canMoveC(int n) {
		if (state[LB] == ORIGEN)
			return state[CO] >= n &&
					(state[MD] == 0 || state[MD] >= state[CD] + n);
		else
			return state[CD] >= n &&
					(state[MO] == 0 || state[MO] >= state[CO] + n);
	}

	// Para mover canibales ademas
	// - Debe haber tantos misionarios o mas que canibales en la otra
	// orilla
	private boolean canMoveMC() {
		if (state[LB] == ORIGEN)
			return (state[MO] >= 1) &&
					(state[CO] >= 1) &&
					(state[MD] >= state[CD]);
		else
			return (state[MD] >= 1) &&
					(state[CD] >= 1) &&
					(state[MO] >= state[CO]);
	}

	// ----- FUNCIONES ADICIONALES -----

	@Override
	public String toString() {
		String MOstr = "", COstr = "", LBstr = "", MDstr = "", CDstr = "";

		switch (state[MO]) {
			case 0:
				MOstr += "     ";
				break;
			case 1:
				MOstr += "    M";
				break;
			case 2:
				MOstr += "  M M";
				break;
			case 3:
				MOstr += "M M M";
				break;
		}

		switch (state[CO]) {
			case 0:
				COstr += "     ";
				break;
			case 1:
				COstr += "    C";
				break;
			case 2:
				COstr += "  C C";
				break;
			case 3:
				COstr += "C C C";
				break;
		}

		switch (state[LB]) {
			case ORIGEN:
				LBstr += " BOTE --RIO --      ";
				break;
			case DESTINO:
				LBstr += "      --RIO -- BOTE ";
				break;
		}

		switch (state[MD]) {
			case 0:
				MDstr += "     ";
				break;
			case 1:
				MDstr += "    M";
				break;
			case 2:
				MDstr += "  M M";
				break;
			case 3:
				MDstr += "M M M";
				break;
		}

		switch (state[CD]) {
			case 0:
				CDstr += "     ";
				break;
			case 1:
				CDstr += "    C";
				break;
			case 2:
				CDstr += "  C C";
				break;
			case 3:
				CDstr += "C C C";
				break;
		}

		return "RIBERA-IZQ " + MOstr + " " + COstr + LBstr
				+ MDstr + " " + CDstr + " RIBERA-DCH";
	}

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + Arrays.hashCode(state);
		return result;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		CanibalesBoard other = (CanibalesBoard) obj;
		return Arrays.equals(state, other.state);
	}
}
