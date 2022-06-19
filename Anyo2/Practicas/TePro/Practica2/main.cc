/**
 * @file main.cc
 * @author Dorian Boleslaw Wozniak	817570 
 * @brief Programa principal que prueba la implementación de una calculadora
 * 			que analiza expresiones matemáticas en formato RPN.
 */


#include "expression.h"
#include "symboltab.h"

#include <iostream>

using namespace std;

int main() {

	// Definición de mapa de símbolos
	SymbolTab syms;

	syms.insert(pair<string, float>("a", 7.0));
	syms.insert(pair<string, float>("b", 2.0));

	// 3 + 2*5 = 13
	Expression e1("3 2 5 * +");
	cout << e1 << " = " << e1.eval(syms) << endl;
	cout << endl;

	// (3+2) * 5 = 25
	Expression e2("3 2 + 5 *");
	cout << e2 << " = " << e2.eval(syms) << endl;
	cout << endl;

	// 3 - 2*a = -11
	Expression e3("3 2 a * -");
	cout << e3 << " = " << e3.eval(syms) << endl;
	cout << endl;

	// (3+a) / 2 = 5
	Expression e4("3 a + 2 /");
	cout << e4 << " = " << e4.eval(syms) << endl;
	cout << endl;

	// (1+2) * a / (4.1 - b) = 10
	Expression e5("1 2.0 + a * 4.1 b - /");
	cout << e5 << " = " << e5.eval(syms) << endl;
	cout << endl;

	// (1+c) * 2 ERROR!!! 'c' no esta definida
	Expression e6("1 c + 2 *");
	cout << e6 << " = " << e6.eval(syms) << endl;
	cout << endl;

	return 0;
}
