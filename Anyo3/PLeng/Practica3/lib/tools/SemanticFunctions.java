//*****************************************************************
// Tratamiento de errores sintácticos
//
// Fichero:    SemanticFunctions.java
// Fecha:      03/03/2022
// Versión:    v1.0
// Asignatura: Procesadores de Lenguajes, curso 2021-2022
//*****************************************************************

package lib.tools;

import java.util.*;
import traductor.Token;
import lib.attributes.*;
import lib.symbolTable.*;
import lib.symbolTable.Symbol.Types;
import lib.symbolTable.exceptions.*;
import lib.tools.exceptions.*;
import lib.errores.*;

public class SemanticFunctions {

	private ErrorSemantico errSem; // clase común de errores semánticos
	private SymbolTable st; // Tabla de símbolos

	public SemanticFunctions(SymbolTable st) {
		errSem = new ErrorSemantico();
		this.st = st;
	}

	public void expresion(Token t, Attributes at, Attributes at1, Attributes at2) {
		// Comprueba la expresión a la izquierda
		try {
			at1.checkType(Symbol.Types.BOOL);
		} catch (WrongAttrTypeException e) {
			at.type = Symbol.Types.UNDEFINED;
			error(e, t);
		}

		// Comprueba la expresión a la derecha
		try {
			at2.checkType(Symbol.Types.BOOL);
		} catch (WrongAttrTypeException e) {
			at.type = Symbol.Types.UNDEFINED;
			error(e, t);
		}
	}

	public void relacion_logica(Token t, Attributes at, Attributes at1) {
		// Siempre comprueba expresión a la derecha
		try {
			at1.checkType(Symbol.Types.BOOL);
		} catch (WrongAttrTypeException e) {
			at.type = Symbol.Types.UNDEFINED;
			error(e, t);
		}
	}

	public void relacion_aritmetica(Token t, Attributes at, Attributes at1, Attributes at2) {
		// Comprueba la expresión a la izquierda
		try {
			at1.checkType(Symbol.Types.INT);
		} catch (WrongAttrTypeException e) {
			at.type = Symbol.Types.UNDEFINED;
			error(e, t);
		}

		// Comprueba la expresión a la derecha
		try {
			at2.checkType(Symbol.Types.INT);
		} catch (WrongAttrTypeException e) {
			at.type = Symbol.Types.UNDEFINED;
			error(e, t);
		}
	}


	public void error(Exception e, Token t) {
		errSem.deteccion(e, t);
	}

	public void error(String s, Token t) {
		errSem.error(s, t);
	}

	public void warning(String s, Token t) {
		errSem.warning(s, t);
	}

}
