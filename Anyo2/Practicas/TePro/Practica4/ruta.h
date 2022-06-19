/**
 * @file ruta.h
 * @author Dorian Boleslaw Wozniak
 * @brief Cabecera con clases que definen un sistema de archivos en RAM
 *          conformado por nodos que pueden ser directirios, ficheros o enlaces
 *          e implementa operaciones sobre estos y navegación del árbol de
 *          directorios mediente una ruta activa. 
 */

#pragma once

#include <iostream>
#include <iomanip>
#include <list>
#include <stack>
#include <queue> 
#include <memory>
#include <string>
#include <sstream>

using namespace std;

const string ROOTSYM = "/";     // Separador de directorios, estandar POSIX
const string PARENTDIR = "..";  // Representa directorio padre
const string CURRENTDIR = ".";  // Representa directorio actual

const string FICH = "Fichero";
const string DIR = "Directorio";
const string LNK = "Enlace";

/******************************************************************************/

class arbol_ficheros_error {
    public:
        virtual const char* what() const throw()
            { return ""; }
};

class invalid_node_name : public arbol_ficheros_error {
    public:
        const char* what() const throw() 
            { return "Error: Nombre inválido"; }
};

class invalid_node_size: public arbol_ficheros_error {
    public:
        const char* what() const throw()
            { return "Error: Tamaño inválido"; }
};

class name_is_directory: public arbol_ficheros_error {
    public:
        const char* what() const throw() 
            { return "Error: el nombre es un directorio"; }
};

class name_is_not_directory: public arbol_ficheros_error {
    public:
        const char* what() const throw() 
            { return "Error: el nombre no es un directorio"; }
};

class name_exists: public arbol_ficheros_error {
    public:
        const char* what() const throw() 
            { return "Error: el fichero o directorio ya existe"; }
};

class name_does_not_exist: public arbol_ficheros_error {
    public:
        const char* what() const throw() 
            { return "Error: no existe el fichero o directorio"; }
};

class illegal_rm: public arbol_ficheros_error {
    public:
        const char* what() const throw() 
            { return "Error: no se puede eliminar \"/\", \".\" o \"..\""; }
};

/******************************************************************************/

class Nodo {
    protected:
        string _name;   // Nombre del nodo 

        Nodo(const string& name) 
            : _name(name) {}

    public:
        /**
         * Devuelve el nombre del nodo
        */
        string name() const 
            { return _name; }

        /**
         * Devuelve el tamaño del nodo
        */
        virtual int size() const = 0;

        /**
         * Devuelve el tipo de nodo
        */
        virtual string type() const = 0;
};

class Directorio : public Nodo {
    private:
        list<shared_ptr<Nodo>> _subnodes;   // Lista de nodos contenidos

    public:
        Directorio(const string& name) 
            : Nodo(name) {}

        /**
         * Devuelve el tamaño del directorio (suma de tamaños de subnodos)
        */
        virtual int size() const override 
            { int i = 0; for (auto j : _subnodes) i += j -> size(); return i; }

        /**
         * Devuelve que es un directorio
        */
        virtual string type() const override { return DIR; }

        /**
         * Añade un nodo nuevo a la lista de subnodos
        */
        void add(const shared_ptr<Nodo> node) 
            { _subnodes.push_back(node); }
         
        /**
         * Borra un nodo conocido 
        */
        void remove(const shared_ptr<Nodo> node) 
            { _subnodes.remove(node); }

        /**
         * Encuentra un nodo por su nombre. Devuelve un puntero a este si lo
         * encuentra, un puntero nulo en caso contrario
        */
        shared_ptr<Nodo> find(const string& name) {
            for (auto i : _subnodes) if (i -> name() == name) return i;
            return nullptr;
        }
        
        /**
         * Devuelve la lista de subnodos conocidos
        */
        const list<shared_ptr<Nodo>> subnodes() const 
            { return _subnodes; }
};

class Fichero : public Nodo {
    private:
        int _size;  // Tamaño del fichero

    public:
        Fichero(const string& name, int size) 
            : Nodo(name), _size(size) {}

        /**
         * Devuelve el tamaño del fichero
        */
        int size() const override 
            { return _size; }

        /**
         * Devuelve que es un fichero
        */
        string type() const override 
            { return FICH; }

        /**
         * Modifica el tamaño del fichero
        */
       
        void set_size(int size) 
            { _size = size; }
};

class Enlace : public Nodo {
    private:
        shared_ptr<Nodo> _link; // Puntero a nodo enlazado

    public:
        Enlace(const shared_ptr<Nodo>& node, const string& name) 
            : Nodo(name), _link(node) {}
        
        /**
         * Devuelve el tamaño del nodo referenciado
        */
        int size() const override 
            { return _link -> size(); }

        /**
         * Devuelve que es un enlace
        */
        string type() const override 
            { return LNK; } 

        /**
         * Devuelve un puntero al nodo que referencia
        */
        shared_ptr<Nodo> references()
            { return _link; } 
};

/******************************************************************************/

class Ruta {
    private:
        // Pila de directorios recorridos
        stack<shared_ptr<Directorio>> _activedir;   
        
        /**
         * Devuelve verdad si el nombre es válido
        */
        bool validName(const string& name) { 
            if (name.size() == 0 || name == PARENTDIR || name == CURRENTDIR
                || name.find(ROOTSYM) != name.npos
                || name.find(" ") != name.npos) return false;
            else return true;
        }

        /**
         * Devuelve verdad si el tamaño es válido
        */
        bool validSize(int size) 
            { return size >= 0; }

        /**
         *  Devuelve, a partir de una ruta, una nueva pila de directorios salvo
         *  el último en la ruta, que se devuelve como nodo para realizar una
         *  operación particular según el método que la invoca
        */
        shared_ptr<Nodo> parse(const string& path, stack<shared_ptr<Directorio>>& stk) {            
            queue<string> parser;
            string aux;
            
            // Si empieza en la raíz, lo tiene en cuenta
            if (path[0] == '/') { parser.push("/"); aux = path.substr(1); }
            else aux = path;

            // Comprueba el resto de los nombres
            stringstream ss(aux);
            while (aux != "" && !ss.eof()) {
                string token = "";
                getline(ss, token, '/');
                if (token == "" || token.find(ROOTSYM) != token.npos)
                    throw invalid_node_name();
                parser.push(token);
            }

            // Analiza los nombres
            while (parser.size() != 0) {
                // Es el directorio actual: no hace nada
                if (parser.front() == CURRENTDIR) {}

                // Es el directorio padre: desciende al directirio anterior
                // salvo que ya esté en la raíz
                else if (parser.front() == PARENTDIR) 
                    { if (stk.size() > 1) stk.pop(); }

                // Es la raíz: desciende hasta esta
                else if (parser.front() == ROOTSYM) 
                    { while (stk.size() > 1) stk.pop(); }

                // Es un nombre: busca si existe
                else {
                    shared_ptr<Nodo> i = stk.top() -> find(parser.front());
                    
                    // Lo encuentra
                    if (i != nullptr) {
                        // Es el último término: lo devuelve para hacer una acción
                        // sobre él
                        if (parser.size() <= 1) return i;

                        // No es el último término: por tanto, debe ser directorio
                        else {
                            // Es un enlace: obtiene el nodo enlazado
                            while (i -> type() == LNK)
                                i = dynamic_pointer_cast<Enlace>(i) -> references();
                            
                            // Es un directorio
                            if (i -> type() == DIR)
                                stk.push(dynamic_pointer_cast<Directorio>(i));
                            else throw name_is_not_directory();;
                        }
                    }
                    // No lo encuentra
                    else throw name_does_not_exist();
                }

                parser.pop();
            }

            // Si ha salido del bucle, el último término era un "/", ".." ó "."
            return nullptr;
        }


    public:
        Ruta(const Nodo& root) {
            _activedir.push(make_shared<Directorio>(root.name()));
        }

        /**
         * Devuelve dirección textual a ruta activa 
        */
        string pwd() const { 
            if (_activedir.size() == 1) return "/";

            string s = "";
            stack<shared_ptr<Directorio>> stk = _activedir;
            stack<string> names;

            // Primero obtiene una pila de nombres con la dirección actual
            while (stk.size() != 1) {
                names.push(stk.top() -> name());
                stk.pop();
            }

            // Ahora obtiene una dirección 
            while(names.size() != 0) {
                s += "/" + names.top();
                names.pop();
            }

            return s;
        }

        /**
         *  Devuelve listado con nombres de la ruta activa
         */
        string ls() const {
            stringstream ss;      
            for (auto i : _activedir.top() -> subnodes()) ss << i -> name() << endl;
            return ss.str();
        }

        /**
         *  Devuelve listado con nombres y tamaño de la ruta activa
         */
        string du() const {
            stringstream ss;
            for (auto i : _activedir.top() -> subnodes())
                ss << setw(40) << left << i -> name() << setw(2) << "" 
                    << setw(10) << right << i -> size() << endl;
            return ss.str();
        }

        /**
         *  Modifica el tamaño de un fichero. Si no existe, crea un fichero con
         *  el nombre y tamaño indicados
         */
        void vi(const string& name, int size) {
            // Comprueba la validez del nombre y tamaño
            if (!validName(name)) throw invalid_node_name();
            if (!validSize(size)) throw invalid_node_size();

            // Busca el nodo con el mismo nombre
            shared_ptr<Nodo> i = _activedir.top() -> find(name);

            // Si lo encuentra, prueba que sea un fichero
            if(i != nullptr) {
                // Si es un enlace, primero obtiene el nodo referencia
                while (i -> type() == LNK) 
                    i = dynamic_pointer_cast<Enlace>(i) -> references();  
                // Si es un fichero, modifica su tamaño y acaba
                if (i -> type() == FICH) { 
                    dynamic_pointer_cast<Fichero>(i) -> set_size(size);
                    return; 
                }
                // Si es un directorio, lanza una excepción
                else throw name_is_directory();
            }

            // Si recorre la lista sin encontrar el nodo, crea uno nuevo
            _activedir.top() -> add(make_shared<Fichero>(name, size));
        }

        /**
         * Crea un nuevo directorio en la ruta activa
        */
        void mkdir(const string& name) {
            // Comprueba que el nombre sea válido
            if (!validName(name)) throw invalid_node_name();

            // Comprueba que no exista un fichero con el mismo nombre
            if (_activedir.top() -> find(name) != nullptr) throw name_exists();
            
            // Crea un directorio nuevo
            _activedir.top() -> add(make_shared<Directorio>(name));
        }

        void cd(const string& path) {
            // Obtiene una copia de la ruta activa temporal
            stack<shared_ptr<Directorio>> temp = _activedir;

            // Analiza la ruta: le devuelve el último término
            shared_ptr<Nodo> last_cd = parse(path, temp);
            
            // Si devuelve un nombre, debe ser un directorio donde cambiar
            // Si no, ya está en él
            if(last_cd != nullptr) {
                // Es un enlace
                while (last_cd -> type() == LNK)
                    last_cd = dynamic_pointer_cast<Enlace>(last_cd) -> references();
                // Es un directorio
                if (last_cd -> type() == DIR)
                    temp.push(dynamic_pointer_cast<Directorio>(last_cd));
                // Si no es un directorio, excepción
                else throw name_is_not_directory();
            }

            // Sustituye la ruta activa por la analizada 
            _activedir = temp;
        }

        /**
         * Crea un enlace a un nodo en una ruta específica
        */
        void ln(const string& path, const string& name) {
            // Comprueba si el nombre del enlace a crear es correxto
            if (!validName(name)) throw invalid_node_name();

            // Obtiene una copia de la ruta activa temporal
            stack<shared_ptr<Directorio>> temp = _activedir;

            // Analiza la ruta: obtiene el último término
            shared_ptr<Nodo> node_to_link = parse(path, temp);
    
            // El parser deberia devolver un nodo salvo que sea /, . ó ..
            // Si el puntero es nulo, se usa el directorio actual 
            if(node_to_link == nullptr) node_to_link = temp.top();

            // Añade el enlace
            _activedir.top() -> add(make_shared<Enlace>(node_to_link, name));
        }

        /**
         * Devuelve el tamaño del nodo en path
        */
        int stat(const string& path) {
            // Obtiene una copia de la ruta activa temporal
            stack<shared_ptr<Directorio>> temp = _activedir;

            // Analiza la ruta: obtiene el último término
            shared_ptr<Nodo> node_to_stat = parse(path, temp);

            // Si puntero es nulo, usa directorio actual 
            if(node_to_stat == nullptr) node_to_stat = temp.top();

            // Obtiene su tamaño
            return node_to_stat -> size();
        }
        
        /**
         * Elimina un nodo. 
         * Si es un directorio, la eliminación es recursiva.
         * Si se borra un enlace, se borra solo el enlace.
         * Si se borra un objeto enlazado, deberá seguir siendo accesible
         * para los enlaces
        */
        void rm(const string& path) {
            // Obtiene una copia de la ruta activa temporal
            stack<shared_ptr<Directorio>> temp = _activedir;
            shared_ptr<Nodo> node_to_rm = parse(path, temp);

            // Si puntero es nulo, no debe hacer nada porque no pueder borrar
            // la raíz, el directorio actual ni el padre
            if(node_to_rm == nullptr) {
                throw illegal_rm();
            }

            temp.top() -> remove(node_to_rm);
        }
};