/*
    Estructura de datos y algoritmos
    Práctica 1

    Autores: Dorian Boleslaw Wozniak        817570
             Jorge Pérez Liesa              821011

    Descripción: Programa principal que, a partir de un fichero entrada.txt, lee instrucciones
    y realiza las operaciones para gestionar una colección de ITF de tipo programa con las 
    operaciones definidas por los tipos coleccionITF, programa y fechas 
*/

#include "coleccionITF.h"
#include "fechas.h"
#include "programas.h"

#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;

/*
    Pre: true
    Post: Devuelve el identificador de un item (caso particular programa)
*/

string identificador(Programa p){
    return suNombre(p);
}

/*
    Pre: true
    Post: Devuelve el tema de un item (caso particular programa)
*/

string tema(Programa p){
    return suLenguaje(p);
}

/*
    Pre: true
    Post: Devuelve la fecha de un item (caso particular programa)
*/

Fecha fecha(Programa p){
    return suFecha(p);
}

/*
    Pre: Se ha leido una instrucción A
    Post: Trata de añadir el programa a la colección. Avisa del éxito de la operación
            y los datos del programa
*/

void instrA(ofstream& f, coleccionITF<Programa>& c, Programa p){
    Fecha f_aux = suFecha(p); 
    
    if(anyadir(c, p)){
        Programa p_aux;
        esta(c, p_aux, identificador(p));
        Fecha f_aux = suFecha(p_aux);

        f << "INTRODUCIDO: ";
    }

    else{

        f << "NO INTRODUCIDO: ";
    }

    f << identificador(p) << ":::" << anyo(f_aux) << "-" 
            << fixed << setw(2) << setfill('0') << mes(f_aux) << "-" << fixed 
            << setw(2) << dia(f_aux) << setfill(' ') << ":::" << tema(p) << ":::" 
            << suDescripcion(p) << endl;
}

/*
    Pre: Se ha leido una instrucción O
    Post: Trata de encontrar el programa según su nombre en la colección. Si tiene éxito, 
            lo dice y adjunta los datos del programa; avisa del fracaso y el nombre del programa
            buscado en caso contrario
*/

void instrO(ofstream& f, coleccionITF<Programa>& c, string nom){
    Programa p_aux;

    if(esta(c, p_aux, nom)){
        Fecha f_aux = suFecha(p_aux);
        f << "LOCALIZADO: " << identificador(p_aux) << ":::" << anyo(f_aux) << "-" 
            << fixed << setw(2) << setfill('0') << mes(f_aux) << "-" << fixed 
            << setw(2) << dia(f_aux) << setfill(' ') << ":::" << tema(p_aux) 
            << ":::" << suDescripcion(p_aux) << endl;
    }

    else{
        f << "NO LOCALIZADO: " << nom << endl;
    }
}

/*
    Pre: Se ha leído una instrucción B
    Post: Trata de borrar el programa según su nombre de la colección. Avisa si ha podido borrarlo o no
*/

void instrB(ofstream& f, coleccionITF<Programa>& c, string nom){
    Programa p_aux;

    if(borrar(c, nom)){
        f << "BORRADO: ";
    }

    else{
        f << "NO BORRADO: ";
    }

    f << nom << endl;
}

/*
    Pre: Se ha leido una instrucción P
    Post: Trata de purgar todos los programas anteriores a una fecha determinada. Avisa si ha podido
            borrarlas o no
*/


void instrP(ofstream& f, coleccionITF<Programa>& c, Fecha fe){
    if(purgar(c, fe)){
        f << "PURGADOS: ";
    }

    else{
        f << "NO PURGADOS: ";
    }

    f  << anyo(fe) << "-" << fixed << setw(2) << setfill('0') << mes(fe) 
        << "-" << fixed << setw(2) << dia(fe) << setfill(' ') << endl; 
}

/*
    Pre: Se ha leido la instrucción NO
    Post: Obtiene la lista de los programas anteriores a una fecha determinada, y los presenta
*/

void instrNO(ofstream& f, coleccionITF<Programa>& c, Fecha fe){
    string s = obsoletos(c, fe, ":::");

    f << "ANTERIORES A: " << anyo(fe) << "-" << fixed << setw(2) << setfill('0') 
        << mes(fe) << "-" << fixed << setw(2) << dia(fe) << setfill(' ') << ":::" << s << endl;
}

/*
    Pre: Se ha leido la instrucción NL
    Post: Obtiene la lista de los programas cuyo lenguaje sea el dado, y los presenta
*/

void instrNL(ofstream& f, coleccionITF<Programa>& c, string len){
    string s = delTema(c, len, ":::");

    f << "LENGUAJE: " << len << ":::" << s << endl;
}

/*
    Pre: Se ha leido la instrucción L 
    Post: Lista todos los programas de la colección
*/

void instrL(ofstream& f, coleccionITF<Programa>& c){
    f << "-----LISTADO: " << tamanyo(c) << endl;

    Programa p;
    Fecha f_aux;

    iniciarIterador(c);

    while(siguiente(c, p)){
        f_aux = suFecha(p);

        f << identificador(p) << ":::" << anyo(f_aux) << "-" << fixed << setw(2) << setfill('0')
            << mes(f_aux) << "-" << fixed << setw(2) << dia(f_aux) << setfill(' ') << ":::" 
            << tema(p) << ":::" << suDescripcion(p) << endl;
    }

    f << "-----" << endl;
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
    getline(f, opcode);
}

/*
    Pre: Se ha podido abrir previamente los ficheros "entrada.txt" y "salida.txt"
    Post: Se cierran los ficheros "entrada.txt" y "salida.txt"
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


bool realizarInstruccion(ifstream& f_ent, ofstream& f_sal, string opcode, coleccionITF<Programa>& c){
    bool fallo = false; // Indica si hubo fallo

    // Variables locales
    string nom, leng, desc, dia, mes, anyo;
    Fecha f;
    Programa p;

    /*
        CASO INSTR A

        Lee datos completos del programa a introducir (nombre, fecha, lenguaje, descripción)
        e intenta añadirlo a la colección
    */

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

    /*
        CASO INSTR O, B

        Lee un nombre
    */

    else if(opcode == "O" || opcode == "B"){
        getline(f_ent, nom);

        /*
            CASO INSTR O

            Trata de encontrar el programa con dicho nombre
        */

        if(opcode == "O"){
            instrO(f_sal, c, nom);
        }

        /*
            CASO INSTR B

            Borra, si existe, el programa con dicho nombre
        */

        else{
            instrB(f_sal, c, nom);
        }

    }

    /*
        CASO P, NO

        Lee una fecha
    */

    else if(opcode == "P" || opcode == "NO"){
        getline(f_ent, anyo, '-');
        getline(f_ent, mes, '-');
        getline(f_ent, dia);
        crear(stoi(dia), stoi(mes), stoi(anyo), f);

        /*
            CASO P

            Purga los programas anteriores a la fecha
        */
        if(opcode == "P"){
            instrP(f_sal, c, f);
        }

        /*
            CASO NO

            Lista los programas obsoletos
        */

        else{
            instrNO(f_sal, c, f);
        }
    }

    /*
        CASO NL

        Lee un lenguaje, y lista todos los programas en dicho lenguaje 
    */

    else if(opcode == "NL"){
        getline(f_ent, leng);
        instrNL(f_sal, c, leng);
    }

    /*
        CASO L

        Lista todos los programas de la colección
    */

    else if(opcode == "L"){
        instrL(f_sal, c);
    }

    /*
        CASO DE FALLO
    */

    else{
        fallo = true;
    }

    return fallo;
}


int main(){
    // Crea la colección de items
    coleccionITF<Programa> c;
    crear(c);   

    // Flujos de entrada/salida
    ifstream f_ent;
    ofstream f_sal;

    if(abrirFichero(f_ent, f_sal)){
        string opcode;
        bool fallo = false;
        while(!f_ent.eof() && !fallo){
            leerInstr(f_ent, opcode);
            fallo = realizarInstruccion(f_ent, f_sal, opcode, c);
            
            if(fallo){
                cerr << "Fallo al leer instruccion\n";
            }
        }
        cerrarFichero(f_ent, f_sal);
    }

}