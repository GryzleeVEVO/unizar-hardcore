package aima.core.environment.eightpuzzle;

import aima.core.search.framework.HeuristicFunction;

/**
 * @author Ravi Mohan
 * 
 */
public class MisplacedTilleHeuristicFunction implements HeuristicFunction {

	private EightPuzzleBoard goal;
	
	public MisplacedTilleHeuristicFunction() { goal = new EightPuzzleBoard(new int[] {0, 1, 2, 3, 4, 5, 6, 7, 8}) ; }
	
	public MisplacedTilleHeuristicFunction(EightPuzzleBoard goal) { this.goal = goal; }
	
	public double h(Object state) {
		EightPuzzleBoard board = (EightPuzzleBoard) state;
		return getNumberOfMisplacedTiles(board);
	}

	private int getNumberOfMisplacedTiles(EightPuzzleBoard board) {
		int numberOfMisplacedTiles = 0;
		if (!(board.getLocationOf(0).equals(goal.getLocationOf(0)))) {
			numberOfMisplacedTiles++;
		}
		if (!(board.getLocationOf(1).equals(goal.getLocationOf(1)))) {
			numberOfMisplacedTiles++;
		}
		if (!(board.getLocationOf(2).equals(goal.getLocationOf(2)))) {
			numberOfMisplacedTiles++;
		}
		if (!(board.getLocationOf(3).equals(goal.getLocationOf(3)))) {
			numberOfMisplacedTiles++;
		}
		if (!(board.getLocationOf(4).equals(goal.getLocationOf(4)))) {
			numberOfMisplacedTiles++;
		}
		if (!(board.getLocationOf(5).equals(goal.getLocationOf(5)))) {
			numberOfMisplacedTiles++;
		}
		if (!(board.getLocationOf(6).equals(goal.getLocationOf(6)))) {
			numberOfMisplacedTiles++;
		}
		if (!(board.getLocationOf(7).equals(goal.getLocationOf(7)))) {
			numberOfMisplacedTiles++;
		}
		if (!(board.getLocationOf(8).equals(goal.getLocationOf(8)))) {
			numberOfMisplacedTiles++;
		}
		// Subtract the gap position from the # of misplaced tiles
		// as its not actually a tile (see issue 73).
		if (numberOfMisplacedTiles > 0) {
			numberOfMisplacedTiles--;
		}
		return numberOfMisplacedTiles;
	}
}