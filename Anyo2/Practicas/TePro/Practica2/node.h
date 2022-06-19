/**
 * @file node.h
 * @author Dorian Boleslaw Wozniak  817570 
 * @brief Cabecera para una serie de clases de tipo Node, que implementan,
 *          mediante herencia, un árbol de expresiones a utilizar con Expression
 */

#pragma once

#include "symboltab.h"

/**
 * @brief Clase base Node, que almacena un nodo del árbol de expresión
 */
class Node {

    public:

        virtual ~Node();

        // Devuelve el valor del nodo
        virtual float value(const SymbolTab& syms, bool& error) const = 0;
        // Devuelve una cadena con la expresión en notación infija
        virtual string to_string() const = 0;
        // Caso particular de to_string() en caso de ser la raíz del árbol
        virtual string to_string_root() const = 0;
};

/**
 * @brief Nodo de tipo Valor Literal
 */
class Lval : public Node {

    private:

        float lval;         // Valor literal

    public:

        Lval(float l);

        float value(const SymbolTab& syms, bool& error) const override;
        string to_string() const override;
        string to_string_root() const override;
};

/**
 * @brief Nodo de tipo Valor Simbólico
 */
class Sval : public Node {
    
    private:

        string sval;        // Símbolo

    public:
        
        Sval(const string& s);

        float value(const SymbolTab& syms, bool& error) const override;
        string to_string() const override;
        string to_string_root() const override; 

};

/**
 * @brief Nodo de tipo Operación Binaria
 */
class Op : public Node {

    protected:

        Node* left;             // Miembro a la izquierda del operador
        Node* right;            // Miembro a la derecha del operador

    public:

        Op(Node* l, Node* r);
        virtual ~Op();

};

/**
 * @brief Nodo de tipo Operador Suma 
 */
class Add : public Op {
    
    public:

        Add(Node* l, Node* r);

        float value(const SymbolTab& syms, bool& error) const override;
        string to_string() const override;
        string to_string_root() const override; 

};

/**
 * @brief Nodo de tipo Operador Resta 
 */
class Sub : public Op {
    
    public:

        Sub(Node* l, Node* r);

        float value(const SymbolTab& syms, bool& error) const override;
        string to_string() const override;
        string to_string_root() const override;
};

/**
 * @brief Nodo de tipo Operador Multiplicación 
 */
class Mul : public Op {
    
    public:

        Mul(Node* l, Node* r);

        float value(const SymbolTab& syms, bool& error) const override;
        string to_string() const override;
        string to_string_root() const override;
};

/**
 * @brief Nodo de tipo Operador División 
 */
class Div : public Op {
    
    public:

        Div(Node* l, Node* r);

        float value(const SymbolTab& syms, bool& error) const override;
        string to_string() const override;
        string to_string_root() const override;
};
