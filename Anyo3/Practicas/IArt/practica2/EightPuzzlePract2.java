/*
 * Inteligencia Artificial
 * Práctica 2
 * 
 * Autor: Dorian Boleslaw Wozniak (817570@unizar.es)
 * 
 * Descripción: Implementación de una clase que obtiene la media de nodos 
 * 					generados y factor de ramificación efectivo para una serie
 * 					de 100 experimentos aleatorios para produndidades de 2 a 24
 * 					con los siguientes algoritmos:
 * 
 * 					- Búsqueda por anchura
 * 					- Búsqueda por profundidad iterativa
 * 					- Búsqueda A*, heurística Manhattan
 * 					- Búsqueda A*, heurística Misplaced Tile 
 */
package aima.gui.demo.search;


import aima.core.environment.eightpuzzle.EightPuzzleBoard;
import aima.core.environment.eightpuzzle.EightPuzzleFunctionFactory;
import aima.core.environment.eightpuzzle.EightPuzzleGoalTest;
import aima.core.environment.eightpuzzle.ManhattanHeuristicFunction;
import aima.core.environment.eightpuzzle.MisplacedTilleHeuristicFunction;

import aima.core.search.framework.GraphSearch;
import aima.core.search.framework.Problem;
import aima.core.search.framework.Search;
import aima.core.search.framework.SearchAgent;

import aima.core.search.informed.AStarSearch;
import aima.core.search.uninformed.BreadthFirstSearch;
import aima.core.search.uninformed.IterativeDeepeningSearch;

import aima.core.util.math.Biseccion;

public class EightPuzzlePract2 {
	
	public static void main(String[] args) {
		
		// Cabecera tabla
		System.out.println("h(1) == Manhattan");
		System.out.println("h(2) == Misplaced tile");
		System.out.println();
		
		for (int i = 0; i < 90; i++) System.out.print("-"); System.out.print("\n");
			
		System.out.print("||    ||");
		System.out.format("%21s", "Nodos generados");
		System.out.print("                  ||");
		System.out.format("%21s", "b*");
		System.out.print("                  ||\n");
		
		for (int i = 0; i < 90; i++) System.out.print("-"); System.out.print("\n");
		
		System.out.format("||%4s||", "d");
		System.out.format("%7s  |", "BFS");
		System.out.format("%7s  |", "IDS");
		System.out.format("%7s  |", "A*h(1)");
		System.out.format("%7s  ||", "A*h(2)");
		System.out.format("%7s  |", "BFS");
		System.out.format("%7s  |", "IDS");
		System.out.format("%7s  |", "A*h(1)");
		System.out.format("%7s  ||\n", "A*h(2)");
		
		for (int i = 0; i < 90; i++) System.out.print("-"); System.out.print("\n");
		for (int i = 0; i < 90; i++) System.out.print("-"); System.out.print("\n");

		// Realiza experimentos desde la capa 2 hasta la 24
		for (int i = 2; i <= 24; i++) {
			// Medias de nodos generados por profundidad
			int nodos_BFS_media = 0;
			int nodos_IDS_media = 0;
			int nodos_AH1_media = 0;
			int nodos_AH2_media = 0;
			
			// Factores de ramificación efectivos medios
			double B_Estrella_BFS_media = 0;
			double B_Estrella_IDS_media = 0;
			double B_Estrella_AH1_media = 0;
			double B_Estrella_AH2_media = 0;
			
			// Realiza 100 experimentos aleatorios por cada profundidad
			for (int j = 0; j < 100; j++) {
				
				EightPuzzleGoalTest goal;
				EightPuzzleBoard board;
				boolean validDepth = false;
				int[] result = {0, 0};
				
				do {
					// Genera un nuevo tablero inicial y objetivo aleatorios
					goal = new EightPuzzleGoalTest(GenerateInitialEightPuzzleBoard.randomIni());
					board = new EightPuzzleBoard(GenerateInitialEightPuzzleBoard.random(i, goal.getGoalState()));
					
					// Si el A*, que garantiza encontrar la solución optima de forma relativamente rapida, 
					// devuelve una solución menor que la profundidad deseada, vuelve a generar un puzle aleatorio
					result = runSearch(board, new AStarSearch(new GraphSearch(), new ManhattanHeuristicFunction(goal.getGoalState())), goal);
					validDepth = result[1] == i;
				} while (!validDepth);
				
				// A* heurística Manhattan
				nodos_AH1_media += result[0]; 
				B_Estrella_AH1_media += Biseccion.B_Estrella(1.01, 4.0, result[0], i);
				
				// A* heurística Misplaced Tiles
				result = runSearch(board, new AStarSearch(new GraphSearch(), new MisplacedTilleHeuristicFunction(goal.getGoalState())), goal);
				nodos_AH2_media += result[0];
				B_Estrella_AH2_media += Biseccion.B_Estrella(1.01, 4.0, result[0], i);
				
				// BFS
				result = runSearch(board, new BreadthFirstSearch(new GraphSearch()), goal);					
				nodos_BFS_media += result[0]; 
				B_Estrella_BFS_media += Biseccion.B_Estrella(1.01, 4.0, result[0], i);

				// IDS
				if (i <= 10) {
					result = runSearch(board, new IterativeDeepeningSearch(), goal);
					nodos_IDS_media += result[0]; 
					B_Estrella_IDS_media += Biseccion.B_Estrella(1.01, 4.0, result[0], i);
				}
				
			}
			nodos_BFS_media /= 100;
			nodos_IDS_media /= 100;
			nodos_AH1_media /= 100;
			nodos_AH2_media /= 100;
			B_Estrella_BFS_media /= 100.0;
			B_Estrella_IDS_media /= 100.0;
			B_Estrella_AH1_media /= 100.0;
			B_Estrella_AH2_media /= 100.0;
			
			// Línea profundidad d
			
			System.out.format("||%4d||", i);
			System.out.format("%7d  |", nodos_BFS_media);
			System.out.format("%7d  |", nodos_IDS_media);
			System.out.format("%7d  |", nodos_AH1_media);
			System.out.format("%7d  ||", nodos_AH2_media);
			System.out.format("%7.2f  |", B_Estrella_BFS_media);
			System.out.format("%7.2f  |", B_Estrella_IDS_media);
			System.out.format("%7.2f  |", B_Estrella_AH1_media);
			System.out.format("%7.2f  ||\n", B_Estrella_AH2_media);
		}
		
		System.out.print("\n");
		for (int i = 0; i < 90; i++) System.out.print("-"); System.out.print("\n");
		for (int i = 0; i < 90; i++) System.out.print("-"); System.out.print("\n");
	}
	
	
	// Resuelve el problema usando un algortimo de búsqueda. Devuelve nodos generados y profundidad de solución
	private static int[] runSearch(EightPuzzleBoard board, Search search, EightPuzzleGoalTest goal) {
		int result[] = {0, 0}; 
		
		try {				
			// Genera un nuevo problema 8-puzzle
			Problem problem = new Problem (	board, 
											EightPuzzleFunctionFactory.getActionsFunction(), 
											EightPuzzleFunctionFactory.getResultFunction(), 
											goal
										  );
			
			// Trata de resolverlo.
			SearchAgent agent = new SearchAgent(problem, search);
				
			// Obtiene el número de nodos generados
			if (agent.getInstrumentation().getProperty("nodesGenerated") != null)
				result[0] = (int) Float.parseFloat(agent.getInstrumentation().getProperty("nodesGenerated"));
			if (agent.getInstrumentation().getProperty("pathCost") != null)
				result[1] = (int) Float.parseFloat(agent.getInstrumentation().getProperty("pathCost"));
 				
			return result;
		} catch (Exception e) { e.printStackTrace(); }
		
		return result;
	}
}
