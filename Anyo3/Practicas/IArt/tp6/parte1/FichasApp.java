/*	
 * 		Inteligencia Artificial
 * 		Trabajo práctico (parte 1)
 * 		
 * 		Autor: Dorian Boleslaw Wozniak	(817570@unizar.es)
 *
 *		Descripción: Programa que resuelve el problema de las fichas
 *						mediante una serie de algoritmos de búsqueda no 
 *						informada, y muestra estadísticas y los pasos
 *						realizados para encontrar la solución por pantalla
 */

package aima.gui.demo.search;

import java.util.List;

import aima.core.agent.Action;

import aima.core.environment.fichas.FichasBoard;
import aima.core.environment.fichas.FichasFunctionFactory;
import aima.core.environment.fichas.FichasGoalTest;

import aima.core.search.framework.GraphSearch;
import aima.core.search.framework.Problem;
import aima.core.search.framework.ResultFunction;
import aima.core.search.framework.Search;
import aima.core.search.framework.SearchAgent;
import aima.core.search.framework.TreeSearch;

import aima.core.search.uninformed.BreadthFirstSearch;
import aima.core.search.uninformed.DepthFirstSearch;
import aima.core.search.uninformed.DepthLimitedSearch;
import aima.core.search.uninformed.IterativeDeepeningSearch;
import aima.core.search.uninformed.UniformCostSearch;

public class FichasApp {

	static FichasBoard board = new FichasBoard();
	
	public static void main(String[] args) {
		// Búsquedas
		runSearch(board, new BreadthFirstSearch(new GraphSearch())	, "BFS-G"	, 0);
		runSearch(board, new BreadthFirstSearch(new TreeSearch())	, "BFS-T"	, 0);
		runSearch(board, new DepthFirstSearch(new GraphSearch())	, "DFS-G"	, 0);
		//runSearch(board, new DepthFirstSearch(new TreeSearch())	, "DFS-T"	, 0);  
		runSearch(board, new DepthLimitedSearch(10)					, "DLS-10"	, 0);
		runSearch(board, new IterativeDeepeningSearch()				, "IDLS"	, 0);
		runSearch(board, new UniformCostSearch(new GraphSearch()) 	, "UCS-G"	, 0);		
		//runSearch(board, new UniformCostSearch(new TreeSearch()) 	, "UCS-T"	, 0);
	}
	
	// Devuelve la secuencia de acciones encontrada por un agente de búsqueda
	// al problema de las fichas
	public static void executeActions(List<Action> actions, Problem problem) {
		Object initialState = problem.getInitialState();
		ResultFunction resultFunction = problem.getResultFunction();
		
		Object state = initialState;
		
		System.out.println("SOLUCIÓN:");
		System.out.println("GOAL STATE");
		System.out.println(state);
		System.out.println("CAMINO ENCONTRADO");
		System.out.println("ESTADO INCIAL");
		System.out.println(state);
		
		for(Action action : actions) {
			System.out.println(action.toString());
			state = resultFunction.result(state, action);
			System.out.println(state);
		}
	}
	
	// Resuelve un problema de las fichas con el algoritmo de búsqueda dado
	// e imprime los resultados por pantalla
	private static void runSearch(FichasBoard board, Search search, String name, int error) 
	{
		try 
		{				
			// Genera un nuevo problema de los canibales
			Problem problem = new Problem (	board, 
											FichasFunctionFactory.getActionsFunction(), 
											FichasFunctionFactory.getResultFunction(), 
											new FichasGoalTest()
										  );
			
			// Trata de resolverlo. Mide el tiempo tomado aproximado en ms
			long time = System.currentTimeMillis();
			SearchAgent agent = new SearchAgent(problem, search);
			time = System.currentTimeMillis() - time;
		
			
			/* Propidedades no definidas tienen un valor 0. Si no, devuelve su valor */
			int depth, expandedNodes, queueSize, maxQueueSize;
			
			// Coste de la solución
			if (agent.getInstrumentation().getProperty("pathCost") != null) 
				depth = (int) Float.parseFloat(agent.getInstrumentation().getProperty("pathCost"));
			else 
				depth = 0;
			
			// Nodos totales expandidos
			if (agent.getInstrumentation().getProperty("nodesExpanded") == null) 
				expandedNodes= 0;
			else 
				expandedNodes = (int) Float.parseFloat(agent.getInstrumentation().getProperty("nodesExpanded"));
			
			// Tamaño de la frontera 
			if (agent.getInstrumentation().getProperty("queueSize") == null) 
				queueSize = 0;
			else 
				queueSize = (int) Float.parseFloat(agent.getInstrumentation().getProperty("queueSize"));
			
			// Tamaño máximo de la frontera
			if (agent.getInstrumentation().getProperty("maxQueueSize") == null) 
				maxQueueSize= 0;
			else 
				maxQueueSize = (int) Float.parseFloat(agent.getInstrumentation().getProperty("maxQueueSize"));

			// Imprime la cabecera del resultado
			System.out.format("Problema de las fichas %s -->\n", name);
			System.out.format("pathCost : %d\n", depth);
			System.out.format("nodesExpanded : %d\n", expandedNodes);
			if (queueSize != 0) System.out.format("queueSize : %d\n", queueSize);
			if (maxQueueSize != 0) System.out.format("maxQueueSize : %d\n", maxQueueSize);
			System.out.format("Tiempo: %d\n\n", time);

			// Obtiene lista de movimientos
			executeActions(agent.getActions(), problem);
		} 
		catch (Exception e) 
		{
			e.printStackTrace();
		}

	}
	
}
