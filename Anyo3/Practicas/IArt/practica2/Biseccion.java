/*
 * Inteligencia Artificial
 * Práctica 2
 * 
 * Autor: Dorian Boleslaw Wozniak (817570@unizar.es)
 * 
 * Descripción: Implementación de una clase para obtener la bisección de una
 * 					función, un método de aproximación de raíces basado en el
 * 					teorema de Bolzano
 */

package aima.core.util.math;

import java.lang.Math;

public class Biseccion {
	
	private static double nodosGenerados;
	private static double profundidadSolucion;
	
	public static double B_Estrella(double B_min, double B_max, int nodosGenerados, int profundidadSolucion) {		
		Biseccion.nodosGenerados = nodosGenerados;
		Biseccion.profundidadSolucion = profundidadSolucion;
		
		// Intervalo a considerar
		double a_0 = B_min;
		double b_0 = B_max;

		// Punto medio del intervalo
		double m = a_0 + Math.abs(b_0 - a_0) / 2;
				
		while (Math.abs(a_0 - b_0) >= Math.pow(10, -12)) {			
			// Ha encontrado la solución
			if (funcionBEstrella(m) == 0) return m; 
			
			// Recalcula intervalo
			else if (funcionBEstrella(a_0) * funcionBEstrella(m) < 0) b_0 = m;
			else a_0 = m;
				
			// Calcula nuevo punto medio
			m = a_0 + Math.abs(b_0 - a_0) / 2;
		}
		
		return m;
	}
	
	private static double funcionBEstrella(double x) {
		return (x * (Math.pow(x, Biseccion.profundidadSolucion) - 1) ) / (x - 1) - Biseccion.nodosGenerados;
	}
}
