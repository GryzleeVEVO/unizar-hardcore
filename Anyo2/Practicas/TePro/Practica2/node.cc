/**
 * @file node.cc
 * @author your name (you@domain.com)
 * @brief Implementación de una serie de clases tipo Node, que implementan,
 *          mediante herencia, un árbol de expresiones a utilizar con Expression
 */

#include "node.h"

/******************************************************************************/
/* CONTRUCTORES                                                               */
/******************************************************************************/

/**
 * @brief Constructor de un nodo Valor Literal 
 * @param l Valor literal
 */
Lval::Lval(float l) 
    : lval(l) {}

/**
 * @brief Constructor de un nodo Valor Simbólico
 * @param s Símbolo
 */
Sval::Sval(const string& s) 
    : sval(s) {}

/**
 * @brief Constructor de un nodo Operación Binaria
 * @param l Miembro a la izquierda
 * @param r Miembro a la derecha
 */
Op::Op(Node* l, Node* r) 
    : left(l), right(r) {}

/**
 * @brief Constructor de un nodo Operador Suma
 * @param l Miembro a la izquierda
 * @param r Miembro a la derecha
 */
Add::Add(Node* l, Node* r) 
    : Op(l, r) {}

/**
 * @brief Constructor de un nodo Operador Resta
 * @param l Miembro a la izquierda
 * @param r Miembro a la derecha
 */
Sub::Sub(Node* l, Node* r) 
    : Op(l, r) {}

/**
 * @brief Constructor de un nodo Operador Multiplicación
 * @param l Miembro a la izquierda
 * @param r Miembro a la derecha
 */
Mul::Mul(Node* l, Node* r) 
    : Op(l, r) {}

/**
 * @brief Constructor de un nodo Operador División
 * @param l Miembro a la izquierda
 * @param r Miembro a la derecha
 */
Div::Div(Node* l, Node* r) 
    : Op(l, r) {}

/******************************************************************************/
/* DESTRUCTORES                                                               */
/******************************************************************************/

/**
 * @brief Destructor de un nodo. Virtual
 */
Node::~Node() {}

/**
 * @brief Destructor de un nodo Operación Binaria. Destruye miembros hijos
 */
Op::~Op() 
    { delete left; delete right; }

/******************************************************************************/
/* MÉTODOS                                                                    */
/******************************************************************************/

/**
 * @brief Calcula el valor del nodo
 * @param syms Tabla de símbolos
 * @param error Error en caso de encontrar un símbolo no definido en la tabla
 *              de símbolos
 * @return Valor literal 
 */
float Lval::value(const SymbolTab& syms, bool& error) const 
    { return lval; }

/**
 * @brief 
 * @param syms Tabla de símbolos
 * @param error Error en caso de encontrar un símbolo no definido en la tabla
 *              de símbolos
 * @return Valor simbólico
 */
float Sval::value(const SymbolTab& syms, bool& error) const {
    if (syms.find(sval) == syms.end()) { error = true; return 0; }
    else { return syms.find(sval) -> second; }
}

/**
 * @brief Calcula el valor del nodo
 * @param syms Tabla de símbolos
 * @param error Error en caso de encontrar un símbolo no definido en la tabla
 *              de símbolos
 * @return Suma de miembros 
 */
float Add::value(const SymbolTab& syms, bool& error) const 
    { return left -> value(syms, error) + right -> value(syms, error); }

/**
 * @brief Calcula el valor del nodo
 * @param syms Tabla de símbolos
 * @param error Error en caso de encontrar un símbolo no definido en la tabla
 *              de símbolos
 * @return Resta de miembros
 */
float Sub::value(const SymbolTab& syms, bool& error) const
    { return left -> value(syms, error) - right -> value(syms, error); }

/**
 * @brief Calcula el valor del nodo
 * @param syms Tabla de símbolos
 * @param error Error en caso de encontrar un símbolo no definido en la tabla
 *              de símbolos
 * @return Multiplicación de miembros
 */
float Mul::value(const SymbolTab& syms, bool& error) const
    { return left -> value(syms, error) * right -> value(syms, error); }

/**
 * @brief Calcula el valor del nodo
 * @param syms Tabla de símbolos
 * @param error Error en caso de encontrar un símbolo no definido en la tabla
 *              de símbolos
 * @return División de miembros 
 */
float Div::value(const SymbolTab& syms, bool& error) const 
    { return left -> value(syms, error) / right -> value(syms, error); }


/**
 * @brief Devuelve la expresión en notación infija
 * @return Valor literal
 */
string Lval::to_string() const 
    { return std::to_string(lval); }

/**
 * @brief Devuelve la expresión en notación infija
 * @return Símbolo
 */
string Sval::to_string() const 
    { return sval; }

/**
 * @brief Devuelve la expresión en notación infija
 * @return (izquierda + derecha)
 */
string Add::to_string() const 
    { return "(" + left -> to_string() + " + " + right -> to_string() + ")"; }

/**
 * @brief Devuelve la expresión en notación infija
 * @return (izquierda - derecha)
 */
string Sub::to_string() const
    { return "(" + left -> to_string() + " - " + right -> to_string() + ")"; }

/**
 * @brief Devuelve la expresión en notación infija
 * @return izquierda * derecha
 */
string Mul::to_string() const
    { return left -> to_string() + " * " + right -> to_string(); }

/**
 * @brief Devuelve la expresión en notación infija
 * @return izquierda / derecha
 */
string Div::to_string() const 
    { return left -> to_string() + " / " + right -> to_string(); }


/**
 * @brief Devuelve la expresión en notación infija. Caso particular de raíz
 * @return Valor literal
 */
string Lval::to_string_root() const 
    { return to_string(); }


/**
 * @brief Devuelve la expresión en notación infija. Caso particular de raíz
 * @return Símbolo
 */
string Sval::to_string_root() const 
    { return to_string(); }


/**
 * @brief Devuelve la expresión en notación infija. Caso particular de raíz
 * @return izquierda + derecha
 */
string Add::to_string_root() const 
    { return left -> to_string() + " + " + right -> to_string(); }


/**
 * @brief Devuelve la expresión en notación infija. Caso particular de raíz
 * @return izquierda - derecha
 */
string Sub::to_string_root() const
    { return left -> to_string() + " - " + right -> to_string(); }


/**
 * @brief Devuelve la expresión en notación infija. Caso particular de raíz
 * @return izquierda * derecha
 */
string Mul::to_string_root() const
    { return to_string(); }


/**
 * @brief Devuelve la expresión en notación infija. Caso particular de raíz
 * @return izquierda / derecha
 */
string Div::to_string_root() const 
    { return to_string(); }


