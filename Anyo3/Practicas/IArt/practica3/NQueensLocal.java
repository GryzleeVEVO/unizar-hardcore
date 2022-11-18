/*
 * Inteligencia Artificial
 * Práctica 3
 * 
 * Autor: Dorian Boleslaw Wozniak (817570@unizar.es)
 * 
 * Descripción:
 * 		Implementación de una clase con una serie de funciones que utilizan
 * 		una serie de algoritmos de búsqueda local para resolver el problema
 * 		de las N reinas:
 * 
 * 		-nQueensHillClimbingSearch_Statistics(numExperimetos)
 * 			
 * 			Realiza una serie de experimentos a partir de tableros no repetidos
 * 			para tratar de resolver el problema mediante un algoritmo de 
 * 			escalada. Devuelve el procentaje de éxitos, fallos y su coste medio
 * 
 * 		- nQueensRandomRestartHillClimbing():
 * 
 * 			Trata de resolver un problema mediante el algoritmo de escalada
 * 			reiniciando el estado inicial hasta hallar una solución. Muestra
 * 			el número de intentos, el coste medio de fallos y el coste de la 
 * 			solución
 * 
 *  	- nQueensSimulatedAnnealingSearch_Statistics(numExperimetos)
 * 			
 * 			Realiza una serie de experimentos a partir de tableros no repetidos
 * 			para tratar de resolver el problema mediante un algoritmo de 
 * 			enfriamiento simulado. Devuelve el procentaje de éxitos, fallos y 
 * 			su coste medio
 * 
 * 		- nQueensSimulatedAnnealingRestart():
 * 
 * 			Trata de resolver un problema mediante el algoritmo de enfiamiento
 * 			simulado reiniciando el estado inicial hasta hallar una solución. 
 * 			Muestra el número de intentos, el coste medio de fallos y el coste 
 * 			de la solución 
 * 
 * 		- nQueensGeneticAlgorithmSearch()
 * 		
 * 			Trata de resolver un problema mediante un algoritmo genético
 * 			obteniendo mutaciones sobre una población inicial hasta obtener un
 * 			algoritmo objetivo
 * */

package aima.gui.demo.search;

import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Properties;
import java.util.Set;

import aima.core.agent.Action;
import aima.core.environment.nqueens.AttackingPairsHeuristic;
import aima.core.environment.nqueens.NQueensBoard;
import aima.core.environment.nqueens.NQueensFitnessFunction;
import aima.core.environment.nqueens.NQueensFunctionFactory;
import aima.core.environment.nqueens.NQueensGoalTest;
import aima.core.search.framework.Problem;
import aima.core.search.framework.SearchAgent;
import aima.core.search.local.GeneticAlgorithm;
import aima.core.search.local.HillClimbingSearch;
import aima.core.search.local.HillClimbingSearch.SearchOutcome;
import aima.core.search.local.Individual;
import aima.core.search.local.Scheduler;
import aima.core.search.local.SimulatedAnnealingSearch;
import aima.core.util.datastructure.XYLocation;

public class NQueensLocal {

	private static final int _boardSize = 8;
	
	public static void main(String[] args) { 
		
		nQueensHillClimbingSearch_Statistics(10000); 
		System.out.println();
		nQueensRandomRestartHillCLimbing();
		System.out.println();
		nQueensSimulatedAnnealingSearch_Statistics(1000);
		System.out.println();		
		nQueensSimulatedAnnealingRestart();
		System.out.println();
		nQueensGeneticAlgorithmSearch();
	}
	
	
	/*
	 * Devuelve un procentaje, siendo frac la parte, y total el todo
	 * */
	private static double procentaje(int f, int t) {
		double frac_ = f, total_ = t;
		return frac_ / total_ * 100;
	}
	
	/*
	 * Devuelve la media aritmética, siendo sum la suma de los elementos
	 * y N el total de elementos 
	 * */
	private static double mediaAritmetica(int s, int N) {
		double sum_ = s, N_ = N;
		if (N_ == 0) return 0; else return sum_ / N_;
	}
	
	/*
	 * Genera un tablero aleatorio colocando una reina en una posición 
	 * aleatoria para cada columna del tablero
	 * */
	private static NQueensBoard generarTablero() {
		NQueensBoard b = new NQueensBoard(_boardSize);
		
		for (int i = 0; i < _boardSize; i++)
			b.addQueenAt(new XYLocation(i, (int) (Math.random() * _boardSize)));
		
		return b;
	}
	
	/*
	 * Genera un tablero N reinas único dada una lista de tableros ya generados
	 * */
	private static NQueensBoard generarTableroUnico(List<NQueensBoard> tablas) {
		NQueensBoard b; boolean unico;
		
		do {
			b = generarTablero(); unico = true;
			for (NQueensBoard i : tablas) if (i.equals(b)) { unico = false; break; }
		} while (!unico);

		tablas.add(b); return b;
	}
	
	
	/*
	 * Trata de resolver numExperimentos problemas de N reinas únicos usando el
	 * método de búsqueda mediante escalada. Al finalizar devuelve estadísticas
	 * sobre el procentaje de éxitos y fallos, y su coste medio en pasos
	 * */
	private static void nQueensHillClimbingSearch_Statistics(int numExperimentos) {
		
		// La lista almacenará las tablas generadas
		List<NQueensBoard> tablas = new ArrayList<>();

		int exitos = 0, fallos = 0;
		int costeExitos = 0, costeFallos = 0;
		
		// Realiza numExperimentos
		for (int i = 0; i < numExperimentos; i++) { 
			try {

				// Genera un nuevo tablero N reinas único
				NQueensBoard b = generarTableroUnico(tablas);
								
				// Genera un nuevo problema de N Reinas
				Problem p = new Problem(
									b,
									NQueensFunctionFactory.getCActionsFunction(),
									NQueensFunctionFactory.getResultFunction(),
									new NQueensGoalTest()
						);
				// Genera una nueva búsqueda de escalada
				HillClimbingSearch s = new HillClimbingSearch 
												(new AttackingPairsHeuristic()); 
				
				// Resuelve el problema
				SearchAgent a = new SearchAgent(p, s);
				
				if (s.getOutcome() == SearchOutcome.SOLUTION_FOUND) 
					{ exitos++; costeExitos += s.getNodesExpanded(); }
				else 
					{ fallos++; costeFallos += s.getNodesExpanded(); }
				
			} catch (Exception e) { e.printStackTrace(); } 
		}
		
		// Imprime resultados
		System.out.println("NQueens HillClimbing con " + numExperimentos + " estados iniciales diferentes -->"); 
		System.out.format("Fallos: %.2f\n", procentaje(fallos, numExperimentos));
		System.out.format("Coste medio fallos: %.2f\n", mediaAritmetica(costeFallos, numExperimentos - exitos));
		System.out.format("Éxitos: %.2f\n", procentaje(exitos, numExperimentos));
		System.out.format("Coste medio Éxitos: %.2f\n", mediaAritmetica(costeExitos, numExperimentos - fallos));
	}
	
	/*
	 * Resuelve un problema N reinas mediante una búsqueda por escalada con 
	 * reinicios aleatorios en caso de fallo. Muestra la solución encontrada
	 * junto al número de intentos y su coste, y el coste del caso de éxito
	 */
	private static void nQueensRandomRestartHillCLimbing() {
		HillClimbingSearch s = null;
		
		int intentos = 0;
		int costeFallos = 0, costeExito = 0;
		
		do {
			try {
				// Genera un nuevo tablero N reinas único
				NQueensBoard b = generarTablero();
								
				// Genera un nuevo problema de N Reinas
				Problem p = new Problem(
									b,
									NQueensFunctionFactory.getCActionsFunction(),
									NQueensFunctionFactory.getResultFunction(),
									new NQueensGoalTest()
						);
				// Genera una nueva búsqueda de escalada
				s = new HillClimbingSearch(new AttackingPairsHeuristic()); 
				
				// Resuelve el problema
				SearchAgent agent = new SearchAgent(p, s);
				
				intentos++;
				
				if (s.getOutcome() == SearchOutcome.SOLUTION_FOUND) 
					costeExito += s.getNodesExpanded();
				else 
					costeFallos += s.getNodesExpanded();
			} catch (Exception e) { e.printStackTrace();}
		} while (s.getOutcome() != SearchOutcome.SOLUTION_FOUND);
		
		// Imprime resultado
		System.out.println("Search Outcome=" + s.getOutcome());
		System.out.println("Final State=\n" + s.getLastSearchState());
		System.out.println("Número de intentos: " +  intentos);
		System.out.format("Coste medio fallos: %.2f\n", mediaAritmetica(costeFallos, intentos - 1));
		System.out.println("Coste éxito: " + costeExito);
	}
	
	/*
	 * Trata de resolver numExperimentos problemas de N reinas únicos usando el
	 * método de enfriamiento simulado. Al finalizar devuelve estadísticas
	 * sobre el procentaje de éxitos y fallos, y su coste medio en pasos
	 * */
	private static void nQueensSimulatedAnnealingSearch_Statistics(int numExperimentos) {
		
		// La lista almacenará las tablas generadas
		List<NQueensBoard> tablas = new ArrayList<>();

		int exitos = 0, fallos = 0;
		int costeExitos = 0, costeFallos = 0;
		
		// Genera un nuevo planificador
		int k = 5; double lam = 0.5; int limit = 200;		
		Scheduler sch = new Scheduler(k, lam, limit);
		
		// Realiza numExperimentos
		for (int i = 0; i < numExperimentos; i++) { 
			try {

				// Genera un nuevo tablero N reinas único
				NQueensBoard b = generarTableroUnico(tablas);
								
				// Genera un nuevo problema de N Reinas
				Problem p = new Problem(
									b,
									NQueensFunctionFactory.getCActionsFunction(),
									NQueensFunctionFactory.getResultFunction(),
									new NQueensGoalTest()
						);
				
				// Genera una nueva búsqueda por enfriamiento simulado
				SimulatedAnnealingSearch s = new SimulatedAnnealingSearch
												(new AttackingPairsHeuristic(), sch); 
				
				// Resuelve el problema
				SearchAgent a = new SearchAgent(p, s);
				
				if (s.getOutcome() == SimulatedAnnealingSearch.SearchOutcome.SOLUTION_FOUND) 
					{ exitos++; costeExitos += s.getNodesExpanded(); }
				else 
					{ fallos++; costeFallos += s.getNodesExpanded(); }
				
			} catch (Exception e) { e.printStackTrace(); } 
		}
		
		// Imprime resultados
		System.out.println("NQueens Simulated Annealing con " + numExperimentos + " estados iniciales diferentes -->"); 
		System.out.format("Parámetros Scheduler: Scheduler (%d, %f, %d)\n", k, lam, limit);
		System.out.format("Fallos: %.2f\n", procentaje(fallos, numExperimentos));
		System.out.format("Coste medio fallos: %.2f\n", mediaAritmetica(costeFallos, numExperimentos - exitos));
		System.out.format("Éxitos: %.2f\n", procentaje(exitos, numExperimentos));
		System.out.format("Coste medio Éxitos: %.2f\n", mediaAritmetica(costeExitos, numExperimentos - fallos));
	}
	
	/*
	 * Resuelve un problema N reinas mediante enfriamiento simulado con 
	 * reinicios aleatorios en caso de fallo. Muestra la solución encontrada
	 * junto al número de intentos y su coste, y el coste del caso de éxito
	 */
	private static void nQueensSimulatedAnnealingRestart() {
		SimulatedAnnealingSearch search = null;
		
		int intentos = 0;
		int costeFallos = 0, costeExito = 0;
		
		// Genera un nuevo planificador
		int k = 5; double lam = 0.5; int limit = 200;		
		Scheduler scheduler = new Scheduler(k, lam, limit);
		
		do {
			try {
				// Genera un nuevo tablero N reinas único
				NQueensBoard board = generarTablero();
								
				// Genera un nuevo problema de N Reinas
				Problem problem = new Problem(
									board,
									NQueensFunctionFactory.getCActionsFunction(),
									NQueensFunctionFactory.getResultFunction(),
									new NQueensGoalTest()
						);
				// Genera una nueva búsqueda de escalada
				search = new SimulatedAnnealingSearch(new AttackingPairsHeuristic(), scheduler); 
				
				// Resuelve el problema
				SearchAgent agent = new SearchAgent(problem, search);
				
				intentos++;
				
				if (search.getOutcome() == SimulatedAnnealingSearch.SearchOutcome.SOLUTION_FOUND) 
					costeExito += search.getNodesExpanded();
				else 
					costeFallos += search.getNodesExpanded();
			} catch (Exception e) { e.printStackTrace();}
		} while (search.getOutcome() != SimulatedAnnealingSearch.SearchOutcome.SOLUTION_FOUND);
		
		// Imprime resultado
		System.out.println("Search Outcome=" + search.getOutcome());
		System.out.println("Final State=\n" + search.getLastSearchState());
		System.out.format("Parámetros Scheduler: Scheduler (%d, %f, %d)\n", k, lam, limit);
		System.out.println("Número de intentos: " +  intentos);
		System.out.format("Coste medio fallos: %.2f\n", mediaAritmetica(costeFallos, intentos - 1));
		System.out.println("Coste éxito: " + costeExito);
	}
	
	/**
	 * Resuelve un problema N reinas mediante algoritmo genético. Genera 
	 * mutaciones a partir de una población inicial hasta encontrar el
	 * objetivo. Imprime por pantalla los parámetros usados, el indivíduo
	 * encontrado, el valor de la fitness function, el tamaño de la población,
	 * el número de iteraciones requeridas y el tiempo
	 */
	public static void nQueensGeneticAlgorithmSearch() {
		
		int tam = 50; double mut = 0.35;
		
		try {
			NQueensFitnessFunction ff = new NQueensFitnessFunction();
			
			// Genera una población inicial
			Set<Individual<Integer>> pop = new HashSet<Individual<Integer>>();
			
			for (int i = 0; i < tam; i++)
				pop.add(ff.generateRandomIndividual(_boardSize));
			
			// Crea un nuevo algoritmo genético y lo ejecuta durante un tiempo
			GeneticAlgorithm<Integer> ga = new GeneticAlgorithm<Integer>(
												_boardSize,
												ff.getFiniteAlphabetForBoardOfSize(_boardSize),
												mut
												);
			// Ejecuta hasta encontrar objetivo
			Individual<Integer> best = ga.geneticAlgorithm(pop, ff, ff, 0L);
		
			// Imprime resultados
			System.out.println("Genetic algorithm");
			System.out.format("Parámetros iniciales: \tPoblación: %d, Probabilidad mutación: %.2f\n", tam, mut);
			System.out.println("Mejor indivíduo=\n" + ff.getBoardForIndividual(best));
			System.out.println("Tamaño tablero\t\t= " + _boardSize);
			System.out.format("Fitness\t\t\t= %.2f\n", ff.getValue(best));
			System.out.println("Es objetivo\t\t= " + ff.isGoalState(best));
			System.out.println("Tamaño de población\t= " + ga.getPopulationSize());
			System.out.println("Iteraciones\t\t= " + ga.getIterations());
			System.out.println("Tiempo\t\t\t= " + ga.getTimeInMilliseconds() + "ms");
			
		} catch (Exception e) { e.printStackTrace(); }
	}
}