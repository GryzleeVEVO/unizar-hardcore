/*
    Estructura de datos y algoritmos
    Práctica 2

    Autores: Dorian Boleslaw Wozniak        817570
             Jorge Pérez Liesa              821011

    Descripción: Programa principal que, a partir de un fichero entrada.txt, 
    lee instrucciones y realiza las operaciones para gestionar una colección de 
    ITF de tipo programa con las operaciones definidas por los tipos 
    ColeccionITF, programa y fechas 
*/

#include "coleccionITF.h"
#include "fechas.h"
#include "programas.h"

#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

const string SEP = ":::";

/*
    Pre: true
    Post: Devuelve el identificador de un item (caso particular programa)
*/

string identificador(const Programa& p){
    return suNombre(p);
}

/*
    Pre: true
    Post: Devuelve el tema de un item (caso particular programa)
*/

string tema(const Programa& p){
    return suLenguaje(p);
}

/*
    Pre: true
    Post: Devuelve la fecha de un item (caso particular programa)
*/

Fecha fecha(const Programa& p){
    return suFecha(p);
}

/*
    Pre: Se ha leido una instrucción A
    Post: Trata de añadir el programa a la colección. Avisa del éxito de la operación
            y los datos del programa
*/

void instrA(ofstream& f_sal, ColeccionITF<Programa>& c, const Programa& p){
    Fecha f_aux = suFecha(p);

    if(anyadir(c, p)){
        f_sal << "INTRODUCIDO: ";
    }

    else{
        f_sal << "NO INTRODUCIDO: ";
    }

    f_sal << suNombre(p) << SEP << anyo(f_aux) << "-" << fixed << setw(2)
        << setfill('0') << mes(f_aux) << "-" << fixed << setw(2) << dia(f_aux)
        << setfill(' ') << SEP << tema(p) << SEP << suDescripcion(p) << endl;
}

/*
    Pre: Se ha leido una instrucción O
    Post: Trata de encontrar el programa según su nombre en la colección. Si tiene éxito, 
            lo dice y adjunta los datos del programa; avisa del fracaso y el nombre del programa
            buscado en caso contrario
*/

void instrO(ofstream& f_sal, ColeccionITF<Programa>& c, const string& nom){
    Programa p_aux;

    if(esta(c, p_aux, nom)){
        Fecha f_aux = suFecha(p_aux);
        f_sal << "LOCALIZADO: " << suNombre(p_aux) << SEP << anyo(f_aux) << "-"
            << fixed << setw(2) << setfill('0') << mes(f_aux) << "-" << fixed
            << setw(2) << dia(f_aux) << setfill(' ') << SEP << tema(p_aux) 
            << SEP << suDescripcion(p_aux) << endl;
    }

    else{
        f_sal << "NO LOCALIZADO: " << nom << endl;
    }
}

/*
    Pre: Se ha leído una instrucción B
    Post: Trata de borrar el programa según su nombre de la colección. Avisa si ha podido borrarlo o no
*/

void instrB(ofstream& f_sal, ColeccionITF<Programa>& c, const string& nom){
    Programa p_aux;

    if(borrar(c, nom)){
        f_sal << "BORRADO: ";
    }

    else{
        f_sal << "NO BORRADO: ";
    }

    f_sal << nom << endl;
}

/*
    Pre: Se ha leido una instrucción P
    Post: Trata de purgar todos los programas anteriores a una fecha determinada. Avisa si ha podido
            borrarlas o no
*/

void instrP(ofstream& f_sal, ColeccionITF<Programa>& c, const Fecha& f){
    if(purgar(c, f)){
        f_sal << "PURGADOS: ";
    }

    else{
        f_sal << "NO PURGADOS: ";
    }

    f_sal << anyo(f) << "-" << fixed << setw(2) << setfill('0') << mes(f) << "-"
        << fixed << setw(2) << dia(f) << setfill(' ') << endl;
}

/*
    Pre: Se ha leido la instrucción NO
    Post: Obtiene la lista de los programas anteriores a una fecha determinada, y los presenta
*/

void instrNO(ofstream& f_sal, ColeccionITF<Programa>& c, const Fecha& f){
    string s = obsoletos(c, f, SEP);

    f_sal << "ANTERIORES A: " << anyo(f) << "-" << fixed << setw(2) 
        << setfill('0') << mes(f) << "-" << fixed << setw(2) << dia(f)
        << setfill(' ') << SEP << s << endl; 
}

/*
    Pre: Se ha leido la instrucción NL
    Post: Obtiene la lista de los programas cuyo lenguaje sea el dado, y los presenta
*/

void instrNL(ofstream& f_sal, ColeccionITF<Programa>& c, const string len){
    string s = delTema(c, len, SEP);

    f_sal << "LENGUAJE: " << len << SEP << s << endl;
}

/*
    Pre: Se ha leido la instrucción L 
    Post: Lista todos los programas de la colección
*/

void instrL(ofstream& f_sal, ColeccionITF<Programa>& c){
    f_sal << "-----LISTADO: " << tamanyo(c) << endl;

    Programa p;
    Fecha f_aux;

    iniciarIterador(c);

    while(siguiente(c, p)){
        f_aux = suFecha(p);

        f_sal << suNombre(p) << SEP << anyo(f_aux) << "-" << fixed << setw(2)
            << setfill('0') << mes(f_aux) << "-" << fixed << setw(2) 
            << dia(f_aux) << setfill(' ') << SEP << tema(p) << SEP
            << suDescripcion(p) << endl;
    }

    f_sal << "-----" << endl;
}

/*
    Pre: true
    Post: Verdad solo si se ha logrado abrir el fichero "entrada.txt" y se ha logrado
            crear el fichero "salida.txt"
*/

bool abrirFichero(ifstream& f_ent, ofstream& f_sal){
    bool abierto = false;
    f_ent.open("entrada.txt");

    if(f_ent.is_open()){
        f_sal.open("salida.txt");

        if(f_sal.is_open()){
            abierto = true;
        }
    }

    return abierto;
}

/*
    Pre: La siguiente línea es una instrucción
    Post: Devuelve la instrucción
*/

void leerInstr(ifstream& f, string& opcode){
    opcode = "";
    getline(f, opcode);
}

/*
    Pre: La siguiente línea es una instrucción
    Post: Devuelve la instrucción
*/

void cerrarFichero(ifstream& f_ent, ofstream& f_sal){
    f_ent.close();
    f_sal.close();
}

/*
    Pre: Se ha leido una instrucción previamente
    Post: Obtiene los datos requeridos según la instrucción y realiza la operación pertinente
            según la instrucción. Devuelve verdadero si ha habido un fallo aldetectar la 
            instrucción, en cualquier otro caso falso 
*/

bool realizarInstruccion(ifstream& f_ent, ofstream& f_sal, const string& opcode, ColeccionITF<Programa>& c){
    bool fallo = false;

    string nom, leng, desc, dia, mes, anyo;
    Fecha f;
    Programa p;

    if(opcode == "A"){
        getline(f_ent, nom);
        getline(f_ent, anyo, '-');
        getline(f_ent, mes, '-');
        getline(f_ent, dia);
        getline(f_ent, leng);
        getline(f_ent, desc);
        crear(stoi(dia), stoi(mes), stoi(anyo), f);
        crear(p, nom, f, leng, desc);

        instrA(f_sal, c, p);
    }

    else if(opcode == "O" || opcode == "B"){
        getline(f_ent, nom);

        if(opcode == "O"){
            instrO(f_sal, c, nom);
        }

        else{
            instrB(f_sal, c, nom);
        }
    }

    else if(opcode == "P" || opcode == "NO"){
        getline(f_ent, anyo, '-');
        getline(f_ent, mes, '-');
        getline(f_ent, dia);
        crear(stoi(dia), stoi(mes), stoi(anyo), f);

        if(opcode == "P"){
            instrP(f_sal, c, f);
        }

        else{
            instrNO(f_sal, c, f);
        }
    }

    else if(opcode == "NL"){
        getline(f_ent, leng);
        instrNL(f_sal, c, leng);
    }

    else if(opcode == "L"){
        instrL(f_sal, c);
    }

    else{
        fallo = true;
    }

    return fallo;
}

int main(){
    ColeccionITF<Programa> c;
    crear(c);

    ifstream f_ent;
    ofstream f_sal;

    if(abrirFichero(f_ent, f_sal)){
        string opcode;
        bool fallo = false;
        while(!f_ent.eof() && !fallo){
            leerInstr(f_ent, opcode);
            fallo = realizarInstruccion(f_ent, f_sal, opcode, c);
            
            if(fallo){
                cerr << "Fallo al leer instruccion\n" << endl;
                return 2;
            }
        }
        cerrarFichero(f_ent, f_sal);
    }

    else{
        cerr << "Error al abrir fichero: compruebe que hay un fichero \"entrada.txt\" en el directorio del programa" << endl;

            return 1;
    }

    return 0;
}