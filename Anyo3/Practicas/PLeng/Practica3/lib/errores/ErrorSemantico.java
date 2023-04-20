//*****************************************************************
// Tratamiento de errores sintácticos
//
// Fichero:    ErrorSintactico.java
// Fecha:      03/03/2022
// Versión:    v1.0
// Asignatura: Procesadores de Lenguajes, curso 2021-2022
//*****************************************************************

package lib.errores;

import traductor.Token;

import lib.symbolTable.exceptions.*;
import lib.tools.exceptions.WrongAttrTypeException;

public class ErrorSemantico {

	final static String sep = "*************************************************************************";

	private static int contadorErrores = 0;
	private static int contadorWarnings = 0;

	public static void deteccion(AlreadyDefinedSymbolException e, Token t) {
		contadorErrores++;
		System.err.println(sep);
		System.err.println("ERROR SEMÁNTICO (" + t.beginLine + "," + t.beginColumn + "): " + "Símbolo: '" + t.image
				+ "'. No se puede redefinir el símbolo");
		System.err.println(sep);
	}

	public static void deteccion(SymbolNotFoundException e, Token t) {
		contadorErrores++;
		System.err.println(sep);
		System.err.println("ERROR SEMÁNTICO (" + t.beginLine + "," + t.beginColumn + "): " + "Símbolo: '" + t.image
				+ "'. El símbolo no está definido");
		System.err.println(sep);
	}

	// public static void deteccion(ActionInvocationException e, String mensaje,
	// Token t) {
	// contadorErrores++;
	// System.err.println(sep);
	// System.err.println("ERROR SEMÁNTICO (" + t.beginLine + "," + t.beginColumn +
	// "): " +
	// "Error al invocar a: '" + t.image + "'. " + mensaje);
	// System.err.println(sep);
	// }

	// Mensaje de error para cuando hay un atributo del tipo erróneo
	public static void deteccion(WrongAttrTypeException e, Token t) {
		contadorErrores++;

		System.err.println(sep);
		System.err.println("ERROR SEMÁNTICO (" + t.beginLine + "," + t.beginColumn + "): " + "Símbolo: '" + t.image
				+ "'. Expresión de tipo erróneo: se esperaba " + e.givenType() + " pero se ha recibido "
				+ e.expectedType() + ".");
	}

	public static void deteccion(String mensaje, Token t) {
		contadorErrores++;
		System.err.println(sep);
		System.err.println("ERROR SEMÁNTICO (" + t.beginLine + "," + t.beginColumn + "): " + "Símbolo: '" + t.image
				+ "'. " + mensaje);
		System.err.println(sep);
	}

	public static void warning(String mensaje, Token t) {
		contadorWarnings++;
		System.err.println(sep);
		System.err.println(
				"WARNING: (" + t.beginLine + "," + t.beginColumn + "): " + "Símbolo: '" + t.image + "'. " + mensaje);
		System.err.println(sep);
	}

	public static int getContadorErrores() {
		return contadorErrores;
	}
}
