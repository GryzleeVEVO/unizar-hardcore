/**
 * @file expression.cc
 * @author Dorian Boleslaw Wozniak	817570	
 * @brief Implementación de la clase Expression, que almacena una expresión en
 * 			notación RPN, y ofrece las operaciones para evaluar la expresión
 * 			y mostrar la expresion en notación infija en flujo 
 */

#include "expression.h"
#include "symboltab.h"

#include <iostream>
#include <sstream>
#include <stack>
#include <limits>

using namespace std;

/**
 * @brief Constructor de una nueva expresión
 * @param s Expresión en formato RPN
 */
Expression::Expression(const string& s) { parse(s); } 

/**
 * @brief Destructor de una expresión. Destruye la raíz del nodo
 */
Expression::~Expression() { delete root; } 

/**
 * @brief Parser que analiza la sintáxis de una expresión RPN y crea un árbol
 * 			sintáctico a partir de este.
 * @param s Expresión en formato RPN
 */
void Expression::parse(const string& s) {
	
	istringstream ss(s);
	string tok;

	stack<Node*> stk;				// La pila ayudará a formar el árbol

	while (ss >> tok) {
		
		/*
			Operaciones: Desapila últimos dos nodos, y crea un nuevo nodo cuyos
			hijos sean los desapilados

			Valores: Apila el valor
		*/

		// Suma
		if (tok=="+") {
			Node* r = stk.top(); stk.pop();
			Node* l = stk.top(); stk.pop();
			stk.push(new Add(l ,r));
		}

		// Resta
		else if (tok=="-") {
			Node* r = stk.top(); stk.pop();
			Node* l = stk.top(); stk.pop();
			stk.push(new Sub(l ,r));
		}

		// Multiplicación
		else if (tok=="*") {
			Node* r = stk.top(); stk.pop();
			Node* l = stk.top(); stk.pop();
			stk.push(new Mul(l ,r));
		}

		// División
		else if (tok=="/") {
			Node* r = stk.top(); stk.pop();
			Node* l = stk.top(); stk.pop();
			stk.push(new Div(l ,r));
		}

		// Valor simbólico
		else if (isalpha(tok[0])) {
			stk.push(new Sval(tok.substr(0, 1)));
		}

		// Valor literal
		else {
			float f = stof(tok);
			stk.push(new Lval(f));
		}

	}

	// Root ahora apunta al nodo padre
	root = stk.top(); 
}

/**
 * @brief Devuelve el resultado de evaluar la expresión
 * 
 * @param syms Tabla de símbolos
 * @return Resultado de la operación, NaN en caso de encontrar un valor 
 * 			simbólico no definido en la tabla de símbolos 
 */
float Expression::eval(const SymbolTab& syms) const { 
	bool error = false;
	float res = root -> value(syms, error);
	
	if (!error) { return res; }
	else { return numeric_limits<float>::quiet_NaN(); }
}

/**
 * @return Expresión en notación infija
 */
string Expression::to_string() const { 
	return root -> to_string_root();
}

/**
 * @param os Flujo de salida
 * @param e Expresión en formato RPN
 * @return Expresión en notación infija en el flujo
 */
ostream& operator<<(ostream& os,const Expression& e) 
	{ os << e.to_string(); return os; }

