package aima.core.environment.eightpuzzle;

import aima.core.search.framework.HeuristicFunction;
import aima.core.util.datastructure.XYLocation;

/**
 * @author Ravi Mohan
 * 
 */
public class ManhattanHeuristicFunction implements HeuristicFunction {

	private EightPuzzleBoard goal;
	
	public ManhattanHeuristicFunction() { goal = new EightPuzzleBoard(new int[] {0, 1, 2, 3, 4, 5, 6, 7, 8}) ; }
	
	public ManhattanHeuristicFunction(EightPuzzleBoard goal) { this.goal = goal; }
	
	public double h(Object state) {
		EightPuzzleBoard board = (EightPuzzleBoard) state;
		int retVal = 0;
		for (int i = 1; i < 9; i++) {
			XYLocation locState = board.getLocationOf(i);
			XYLocation locGoal = goal.getLocationOf(i);
			retVal += evaluateManhattanDistanceOf(i, locState, locGoal);
		}
		return retVal;

	}

	public int evaluateManhattanDistanceOf(int i, XYLocation locState, XYLocation locGoal) {
		return Math.abs(locState.getXCoOrdinate() - locGoal.getXCoOrdinate()) 
				+ Math.abs(locState.getYCoOrdinate() - locGoal.getYCoOrdinate());
	}
}