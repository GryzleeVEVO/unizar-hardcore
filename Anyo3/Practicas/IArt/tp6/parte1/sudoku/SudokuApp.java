/* 
 * Inteligencia Artificial
 * Trabajo TP6 - Primera parte
 * 
 * Fichero: SudokuApp.java
 * 
 * Autor: Dorian Boleslaw Wozniak (817570@unizar.es)
 * 
 * Descripción: El programa lee los contenidos de los ficheros "easy50.txt", 
 * 					"top95.txt" y "hardest.txt" que contienen una serie de
 *  				sudokus de variada dificultad. El programa, para cada
 *  				sudoku obtenido, trata de resolverlo utilizando búsqueda
 *					con propagación de restricciones. El programa también
 *					imprime información relevante acerca de la resolución
 *					de cada sudoku
 * */

package aima.gui.demo.search.TP6;

import aima.core.search.csp.Assignment;
import aima.core.search.csp.BacktrackingStrategy;
import aima.core.search.csp.CSP;
import aima.core.search.csp.CSPStateListener;
import aima.core.search.csp.ImprovedBacktrackingStrategy;
import aima.gui.sudoku.csp.*;

public class SudokuApp {

	public static void main(String args[]) {
		
		// Obtiene los Sudokus a resolver
		Sudoku[] lista1 = Sudoku.listaSudokus2("easy50.txt");
		Sudoku[] lista2 = Sudoku.listaSudokus2("top95.txt");
		Sudoku[] lista3 = Sudoku.listaSudokus2("hardest.txt");
	
		int solucionados = 0;
		
		// Resuelve los sudokus obtenidos
		for (Sudoku s : lista1) { if (solve(s, true, true, true, true)) solucionados++; }
		for (Sudoku s : lista2) { if (solve(s, true, true, true, true)) solucionados++; }
		for (Sudoku s : lista3) { if (solve(s, true, true, true, true)) solucionados++; }

		
		// Devuelve los sudokus solucionadoss
		System.out.println("Número sudokus solucionados: " + solucionados);
	}
	
	// Resuelve el Sudoku leido con la estrategia dada
	private static boolean solve(Sudoku s, boolean MRV, boolean Deg, boolean AC3, boolean LCV) {
		System.out.println("---------");
		
		// Comprueba si el Sudoku dado ya es completo
		//s.asigna_completo(); s.asigna_correccion();
		
		// Imprime sudoku inicial
		s.imprimeSudoku();
		
		if (s.correcto() && s.completo()) {
			System.out.println("SUDOKU DADO CORRECTO - Ignorando");
			System.out.println("+++++++++");
			return true;
		} else System.out.println("SUDOKU INCOMPLETO - Resolviendo");

		
		// Obtiene las celdas dadas por el sudoku
		AvailableCells ac = s.pack_celdasAsignadas();
		
		// Genera un nuevo problema a partir de las celdas dadas
		SudokuProblem sp = new SudokuProblem(ac);
		
		// Define una búsqueda con backtracking
		// (false, false, false, false) = Búsqueda en anchura
		// MRV -> Valores restantes minimos primero
		// Deg -> Heurística de grado (variable que restringe menos primero)
		// AC3 -> Consistencia de arco en vez de foward checking
		// LCV -> Variable menos restringida primero
		BacktrackingStrategy bts = new ImprovedBacktrackingStrategy(MRV, Deg, AC3, LCV);
		
		// Arranca listener para mostrar la búsqueda en acción. No recomendado
		//setupCSPListener(bts);
		
		// Resuelve y mide el tiempo
		double start = System.currentTimeMillis();
		Assignment sol = bts.solve(sp);
		double end = System.currentTimeMillis();
		
		
		// Genera un Sudoku a partir del problema resuelto
		Sudoku ssol = new Sudoku(sol);
		
		// Comprueba si el problema es correcto y completo
		ssol.asigna_completo(); ssol.asigna_correccion();
		
		// Imprime resultado
		System.out.println("Time to solve: " + (end - start) / 1000F);
		System.out.println("SOLUCIÓN:");
		ssol.imprimeSudoku();
		
		if (ssol.correcto() && ssol.completo()) {
			System.out.println("Sudoku solucionado correctamente");
			System.out.println("+++++++++");
			return true;
		} else {
			System.out.println("!!!!! SUDOKU NO RESUELTO CORRECTAMENTE !!!!!");
			System.out.println("+++++++++");
			return true;
		}
		
	}
	
	// Inicia un listener para la búsqueda
	private static void setupCSPListener(BacktrackingStrategy bts) {
		bts.addCSPStateListener(new CSPStateListener() {
			@Override
			public void stateChanged(Assignment assignment, CSP csp) 
				{ System.out.println("Assigment evolved : " + assignment); }

			@Override
			public void stateChanged(CSP csp) {
				{ System.out.println("CSP evolved : " + csp); }
				
			}
		});
	}
}
