/**
 * 		Inteligencia Artificial
 * 		
 * 		Practica 1: Búsqueda no informada
 * 
 * 		Autor: Dorian Boleslaw Wozniak (817570@unizar.es)
 * 
 * 		Descripción:
 * 
 * 		Modificación de EightPuzzleDemo.java que obtiene una lista de 
 * 		estadísticas para la ejecución de algoritmos de búsqueda ciega para una
 * 		serie de 8-puzles con 3, 9 y 30 pasos mínimos requeridos para resolverlos.
 * 
 * 		El formato de la tabla es
 * 
 * 		Problema|	Profundidad|	Expand|		Q.Size|		MaxQS|		tiempo
 * 			    |			   |		  | 		  |			 | 		
 * 				|		       |		  | 		  |			 | 		
 * 				|		       |		  | 		  |			 | 		
 * */

package aima.gui.demo.search;

import aima.core.environment.eightpuzzle.EightPuzzleBoard;
import aima.core.environment.eightpuzzle.EightPuzzleFunctionFactory;
import aima.core.environment.eightpuzzle.EightPuzzleGoalTest;
import aima.core.search.framework.GraphSearch;
import aima.core.search.framework.Problem;
import aima.core.search.framework.Search;
import aima.core.search.framework.SearchAgent;
import aima.core.search.framework.TreeSearch;
import aima.core.search.uninformed.*;

public class EightPuzzlePract1 {
    
    // Tabla 3 pasos
    static EightPuzzleBoard boardWithThreeMoveSolution = 
            new EightPuzzleBoard(new int[] { 1, 2, 5, 3, 4, 0, 6, 7, 8 });

    // Tabla 9 pasos
    static EightPuzzleBoard random1 = 
            new EightPuzzleBoard(new int[] { 1, 4, 2, 7, 5, 8, 3, 0, 6 });

    // Tabla 30 pasos
    static EightPuzzleBoard extreme = 
            new EightPuzzleBoard(new int[] { 0, 8, 7, 6, 5, 4, 3, 2, 1 });

    public static void main(String[] args) {
        
        //Cabecera tabla
        System.out.format("%12s|", "Problema");
        System.out.format("%12s|", "Profundidad");
        System.out.format("%12s|", "Expand");
        System.out.format("%12s|", "Q.Size");
        System.out.format("%12s|", "MaxQS");
        System.out.format("%12s", "tiempo");
        System.out.println("\n");
        
        // Búsqueda en tabla de 3 pasos
        runSearch(boardWithThreeMoveSolution, new BreadthFirstSearch() 							, "BFS-G-3"	, 0);
        runSearch(boardWithThreeMoveSolution, new BreadthFirstSearch(new TreeSearch())			, "BFS-T-3"	, 0);
        runSearch(boardWithThreeMoveSolution, new DepthFirstSearch(new GraphSearch())			, "DFS-G-3"	, 0);
        runSearch(boardWithThreeMoveSolution, new DepthFirstSearch(new TreeSearch())			, "DFS-T-3"	, 2);  // No funciona
        runSearch(boardWithThreeMoveSolution, new DepthLimitedSearch(9)							, "DLS-9-3"	, 0);
        runSearch(boardWithThreeMoveSolution, new DepthLimitedSearch(3)							, "DLS-3-3"	, 0);
        runSearch(boardWithThreeMoveSolution, new IterativeDeepeningSearch()					, "IDS-3"	, 0);
        runSearch(boardWithThreeMoveSolution, new UniformCostSearch(new GraphSearch()) 			, "UCS-G-3"	, 0);		
        runSearch(boardWithThreeMoveSolution, new UniformCostSearch(new TreeSearch()) 			, "UCS-T-3"	, 0);
        
        // Búsqueda en tabla de 9 pasos
        runSearch(random1, new BreadthFirstSearch() 					, "BFS-G-9"	, 0);
        runSearch(random1, new BreadthFirstSearch(new TreeSearch())		, "BFS-T-9"	, 0);
        runSearch(random1, new DepthFirstSearch(new GraphSearch())		, "DFS-G-9"	, 0);
        runSearch(random1, new DepthFirstSearch(new TreeSearch())		, "DFS-T-9"	, 2); // No funciona
        runSearch(random1, new DepthLimitedSearch(9)					, "DLS-9-9"	, 0);
        runSearch(random1, new DepthLimitedSearch(3)					, "DLS-3-9"	, 0);
        runSearch(random1, new IterativeDeepeningSearch()				, "IDS-9"	, 0);
        runSearch(random1, new UniformCostSearch(new GraphSearch()) 	, "UCS-G-9"	, 0);		
        runSearch(random1, new UniformCostSearch(new TreeSearch()) 		, "UCS-T-9"	, 0);
        
        // Búsqueda en tabla de 30 pasos
        runSearch(extreme, new BreadthFirstSearch() 					, "BFS-G-30", 0);
        runSearch(extreme, new BreadthFirstSearch(new TreeSearch())		, "BFS-T-30", 2); // No funciona
        runSearch(extreme, new DepthFirstSearch(new GraphSearch())		, "DFS-G-30", 0);
        runSearch(extreme, new DepthFirstSearch(new TreeSearch())		, "DFS-T-30", 2); // No funciona
        runSearch(extreme, new DepthLimitedSearch(9)					, "DLS-9-30", 0);
        runSearch(extreme, new DepthLimitedSearch(3)					, "DLS-3-30", 0);
        runSearch(extreme, new IterativeDeepeningSearch()				, "IDS-30"	, 1); // No funciona 
        runSearch(extreme, new UniformCostSearch(new GraphSearch()) 	, "UCS-G-30", 0);		
        runSearch(extreme, new UniformCostSearch(new TreeSearch()) 		, "UCS-T-30", 2); // No funciona
    }
    
    
    /*
     * 		Resuelve un problema 8-puzle usando el algoritmo de búsqueda no informada dado
     * */
    private static void runSearch(EightPuzzleBoard board, Search search, String name, int error) 
    {
        try 
        {
            // Las búsquedas que se sabe que dan error solo muestran el tipo de error
            // 1 = El algoritmo no acaba en un tiempo razonable
            // 2 = El uso de memoria del algoritmo es excesivo
            
            if (error != 0) {
                System.out.format("%12s|", name);
                System.out.format("%12s|", "---");
                System.out.format("%12s|", "---");
                System.out.format("%12s|", "---");
                System.out.format("%12s|", "---");
                System.out.format("%10s", "(");
                System.out.format("%1d)" , error);
                System.out.println("\n");
                return;
                
            }
                
            // Genera un nuevo problema 8-puzzle
            Problem problem = new Problem (	board, 
                                            EightPuzzleFunctionFactory.getActionsFunction(), 
                                            EightPuzzleFunctionFactory.getResultFunction(), 
                                            new EightPuzzleGoalTest()
                                          );
            
            // Trata de resolverlo. Mide el tiempo tomado aproximado en ms
            long time = System.currentTimeMillis();
            SearchAgent agent = new SearchAgent(problem, search);
            time = System.currentTimeMillis() - time;
        
            int depth, expandedNodes, queueSize, maxQueueSize;
            
            /* Propidedades no definidas tienen un valor 0. Si no, devuelve su valor */
            
            // Coste de la solución
            String pathcostM = agent.getInstrumentation().getProperty("pathCost");
            if (pathcostM != null) 
                depth = (int) Float.parseFloat(pathcostM);
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

            // Imprime resultados
            System.out.format("%12s|", name);
            System.out.format("%12d|", depth);
            System.out.format("%12d|", expandedNodes);
            System.out.format("%12d|", queueSize);
            System.out.format("%12d|", maxQueueSize);
            System.out.format("%12d", time);
            System.out.println("\n");
            
            
        } 
        catch (Exception e) 
        {
            e.printStackTrace();
        }

    }
}