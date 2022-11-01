/*	
 * 		Inteligencia Artificial
 * 		Trabajo práctico (parte 1)
 * 		
 * 		Autor: Dorian Boleslaw Wozniak	(817570@unizar.es)
 *
 *		Descripción: Clase que contiene las subclases ActionsFunction y
 *						ResultsFunction del problema de las fichas, que 
 *						contienen respectivamente métodos para obtener acciones
 *						disponibles a partir de un estado y estados nuevos a
 *						partir de una acción
 */

package aima.core.environment.fichas;

import java.util.LinkedHashSet;
import java.util.Set;

import aima.core.agent.Action;
import aima.core.search.framework.ActionsFunction;
import aima.core.search.framework.ResultFunction;

public class FichasFunctionFactory {
	
	private final static int I = 0;
	private final static int D = 1;
	
	private static ActionsFunction _actionsFunction = null;
	private static ResultFunction _resultFunction = null;
	
	public static ActionsFunction getActionsFunction() {
		if (_actionsFunction == null) _actionsFunction = new FichasActionsFunction();
		return _actionsFunction;
	}
	
	public static ResultFunction getResultFunction() {
		if (_resultFunction == null) _resultFunction = new FichasResultFunction();
		return _resultFunction;
	}
	
	private static class FichasActionsFunction implements ActionsFunction {
			
			// Añade las acciones que se pueden realizar a una lista
			public Set<Action> actions(Object state) {
				FichasBoard board = (FichasBoard) state;
				Set<Action> actions = new LinkedHashSet<Action>();
								
				if (board.canMove(FichasBoard.MB1I)) actions.add(FichasBoard.MB1I);
				if (board.canMove(FichasBoard.MB2I)) actions.add(FichasBoard.MB2I);
				if (board.canMove(FichasBoard.MB3I)) actions.add(FichasBoard.MB3I);
				if (board.canMove(FichasBoard.MB1D)) actions.add(FichasBoard.MB1D);
				if (board.canMove(FichasBoard.MB2D)) actions.add(FichasBoard.MB2D);
				if (board.canMove(FichasBoard.MB3D)) actions.add(FichasBoard.MB3D);
				if (board.canMove(FichasBoard.MV1I)) actions.add(FichasBoard.MV1I);
				if (board.canMove(FichasBoard.MV2I)) actions.add(FichasBoard.MV2I);
				if (board.canMove(FichasBoard.MV3I)) actions.add(FichasBoard.MV3I);
				if (board.canMove(FichasBoard.MV1D)) actions.add(FichasBoard.MV1D);
				if (board.canMove(FichasBoard.MV2D)) actions.add(FichasBoard.MV2D);
				if (board.canMove(FichasBoard.MV3D)) actions.add(FichasBoard.MV3D);	
								
				return actions;
			}
		}
		
		private static class FichasResultFunction implements ResultFunction {
			
			// Devuelve un nuevo estado dado una acción válida
			public Object result(Object s, Action a) {
				FichasBoard board = (FichasBoard) s;
				FichasBoard newBoard = new FichasBoard(board);
			
				if (FichasBoard.MB1I.equals(a) && board.canMove(FichasBoard.MB1I))
					newBoard.move(I, 1);
				else if (FichasBoard.MB2I.equals(a) && board.canMove(FichasBoard.MB2I))
					newBoard.move(I, 2);
				else if (FichasBoard.MB3I.equals(a) && board.canMove(FichasBoard.MB3I))
					newBoard.move(I, 3);
				else if (FichasBoard.MB1D.equals(a) && board.canMove(FichasBoard.MB1D))
					newBoard.move(D, 1);
				else if (FichasBoard.MB2D.equals(a) && board.canMove(FichasBoard.MB2D))
					newBoard.move(D, 2);
				else if (FichasBoard.MB3D.equals(a) && board.canMove(FichasBoard.MB3D))
					newBoard.move(D, 3);
				else if (FichasBoard.MV1I.equals(a) && board.canMove(FichasBoard.MV1I))
					newBoard.move(I, 1);
				else if (FichasBoard.MV2I.equals(a) && board.canMove(FichasBoard.MV2I))
					newBoard.move(I, 2);
				else if (FichasBoard.MV3I.equals(a) && board.canMove(FichasBoard.MV3I))
					newBoard.move(I, 3);
				else if (FichasBoard.MV1D.equals(a) && board.canMove(FichasBoard.MV1D))
					newBoard.move(D, 1);
				else if (FichasBoard.MV2D.equals(a) && board.canMove(FichasBoard.MV2D))
					newBoard.move(D, 2);
				else if (FichasBoard.MV3D.equals(a) && board.canMove(FichasBoard.MV3D))
					newBoard.move(D, 3);
				// NoOp
				else return s;
				
				return newBoard;
			}
		}
}
