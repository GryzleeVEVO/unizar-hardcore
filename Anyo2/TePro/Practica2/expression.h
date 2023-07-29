/**
 * @file expression.h
 * @author your name (you@domain.com)
 * @brief Archivo de cabecera de una clase Expression, que contiene un árbol de
 * 			expresiones formado a partir de una expresión en formato RPN, 
 * 			ofreciendo métodos para evaluar la expresión y escritura en flujo
 */

#include <iostream>
#include "node.h"


class Expression {
	private:

		friend class Node;		// Clase Node que almacena el árbol sintáctico

		Node* root;				// Nodo raíz

		void parse(const string& s);
		string to_string() const;

	public:

		Expression(const string& s);
		~Expression();
		
		float eval(const SymbolTab& syms) const;
		friend ostream& operator<<(ostream& os,const Expression& e);
};

ostream& operator<<(ostream& os,const Expression& e);
