/*
 * 		Inteligencia Artificial
 * 		Trabajo práctico (parte 1)
 *
 * 		Autor: Dorian Boleslaw Wozniak	(817570@unizar.es)
 *
 *		Descripción: Clase que contiene las subclases ActionsFunction y
 *						ResultsFunction del problema de los canibales, que
 *						contienen respectivamente métodos para obtener acciones
 *						disponibles a partir de un estado y estados nuevos a
 *						partir de una acción
 */

package aima.core.environment.canibales;

import java.util.Set;
import java.util.LinkedHashSet;

import aima.core.agent.Action;
import aima.core.search.framework.ActionsFunction;
import aima.core.search.framework.ResultFunction;

public class CanibalesFunctionFactory {

	private static ActionsFunction _actionsFunction = null;
	private static ResultFunction _resultFunction = null;

	public static ActionsFunction getActionsFunction() {
		if (_actionsFunction == null)
			_actionsFunction = new CanibalesActionsFunction();
		return _actionsFunction;
	}

	public static ResultFunction getResultFunction() {
		if (_resultFunction == null)
			_resultFunction = new CanibalesResultFunction();
		return _resultFunction;
	}

	private static class CanibalesActionsFunction implements ActionsFunction {

		// Añade las acciones que se pueden realizar a una lista
		public Set<Action> actions(Object state) {
			CanibalesBoard board = (CanibalesBoard) state;
			Set<Action> actions = new LinkedHashSet<Action>();

			if (board.canMove(CanibalesBoard.M1M))
				actions.add(CanibalesBoard.M1M);
			if (board.canMove(CanibalesBoard.M1C))
				actions.add(CanibalesBoard.M1C);
			if (board.canMove(CanibalesBoard.M2M))
				actions.add(CanibalesBoard.M2M);
			if (board.canMove(CanibalesBoard.M2C))
				actions.add(CanibalesBoard.M2C);
			if (board.canMove(CanibalesBoard.M1M1C))
				actions.add(CanibalesBoard.M1M1C);

			return actions;
		}
	}

	private static class CanibalesResultFunction implements ResultFunction {

		// Devuelve un nuevo estado dado una acción válida
		public Object result(Object s, Action a) {
			CanibalesBoard board = (CanibalesBoard) s;
			CanibalesBoard newBoard = new CanibalesBoard(board);

			if (CanibalesBoard.M1M.equals(a) && board.canMove(CanibalesBoard.M1M))
				newBoard.moveM(1);
			else if (CanibalesBoard.M1C.equals(a) && board.canMove(CanibalesBoard.M1C))
				newBoard.moveC(1);
			else if (CanibalesBoard.M2M.equals(a) && board.canMove(CanibalesBoard.M2M))
				newBoard.moveM(2);
			else if (CanibalesBoard.M2C.equals(a) && board.canMove(CanibalesBoard.M2C))
				newBoard.moveC(2);
			else if (CanibalesBoard.M1M1C.equals(a) && board.canMove(CanibalesBoard.M1M1C))
				newBoard.moveMC();
			// NoOp
			else
				return s;

			return newBoard;
		}
	}

}
