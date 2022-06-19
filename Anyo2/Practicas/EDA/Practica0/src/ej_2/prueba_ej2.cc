#include "coleccionITF.h"
#include "fechas.h"
#include "programas.h"

#include <iostream>
#include <iomanip>
#include <fstream>

using namespace std;



void instrA(ofstream& f, coleccionITF<programa>& c, programa p){
    fecha f_aux;
    if(anyadir(c, p)){
        programa p_aux;
        obtenerITF(c, p_aux, identificador(p));
        suFecha(p_aux, f_aux);

        f << "INTRODUCIDO: " << identificador(p_aux) << ":::" << elAnyo(f_aux) << "-" << fixed <<setw(2) << setfill('0')
            << elMes(f_aux) << "-" << fixed <<setw(2) << elDia(f_aux) << setfill(' ') << ":::" << tema(p_aux) << ":::" << suDescripcion(p_aux)
            << endl;

    }

    else{
        suFecha(p, f_aux);
        f << "NO INTRODUCIDO: " << identificador(p) << ":::" << elAnyo(f_aux) << "-" << fixed <<setw(2) << setfill('0')
            << elMes(f_aux) << "-" << fixed <<setw(2) << elDia(f_aux) << setfill(' ') << ":::" << tema(p) << ":::" << suDescripcion(p)
            << endl;
    }
}

void instrO(ofstream& f, coleccionITF<programa>& c, string nom){
    programa p_aux;
    if(obtenerITF(c, p_aux, nom)){
        fecha f_aux;
        suFecha(p_aux, f_aux);

        f << "LOCALIZADO: " << identificador(p_aux) << ":::" << elAnyo(f_aux) << "-" << fixed <<setw(2) << setfill('0')
            << elMes(f_aux) << "-" << fixed <<setw(2) << elDia(f_aux) << setfill(' ') << ":::" << tema(p_aux) << ":::" << suDescripcion(p_aux)
            << endl;
    }

    else{
        f << "NO LOCALIZADO: " << nom << endl;
    }
}

void instrB(ofstream& f, coleccionITF<programa>& c, string nom){
    programa p_aux;

    if(borrar(c, nom)){
        f << "BORRADO: " << nom << endl;
    }

    else{
        f << "NO BORRADO: " << nom << endl;
    }
}

void instrP(ofstream& f, coleccionITF<programa>& c, fecha fe){
    if(purgar(c, fe)){
        f << "PURGADOS: " << elAnyo(fe) << "-" << fixed <<setw(2) << setfill('0') << elMes(fe) << "-" << fixed <<setw(2) << elDia(fe) << setfill(' ') << endl; 
    }

    else{
        f << "NO PURGADOS: " << elAnyo(fe) << "-" << fixed <<setw(2) << setfill('0') << elMes(fe) << "-" << fixed <<setw(2) << elDia(fe) << setfill(' ') << endl; 
    }
}

void instrNO(ofstream& f, coleccionITF<programa>& c, fecha fe){
    string s;
    obsoletos(c, fe, ":::", s);
    f << "ANTERIORES A: " << elAnyo(fe) << "-" << fixed <<setw(2) << setfill('0') << elMes(fe) << "-" << fixed <<setw(2) << elDia(fe) << setfill(' ');
    
    if(!s.empty()){
        f << s;
    }

    else{
        f << ":::";
    }

    f << endl;
}

void instrNL(ofstream& f, coleccionITF<programa>& c, string leng){
    string s;
    delTema(c, leng, ":::", s);
    f << "LENGUAJE: " << leng;
    if(!s.empty()){
        f << s;
    }

    else{
        f << ":::";
    }


    f << endl; 
}

void instrL(ofstream& f, coleccionITF<programa>& c){
    f << "-----LISTADO: " << tamanyo(c) << endl;

    programa p;
    fecha f_aux;
    iniciarIterador(c);
    while(siguiente(c, p)){
        suFecha(p, f_aux);
        f << identificador(p) << ":::" << elAnyo(f_aux) << "-" << fixed <<setw(2) << setfill('0')
            << elMes(f_aux) << "-" << fixed <<setw(2) << elDia(f_aux) << setfill(' ') << ":::" << tema(p) << ":::" << suDescripcion(p)
            << endl;
    }


    f << "-----" << endl;
}


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

void leerInstr(ifstream& f, string& opcode){
    getline(f, opcode);
}

void cerrarFichero(ifstream& f_ent, ofstream& f_sal){
    f_ent.close();
    f_sal.close();
}

bool realizarInstruccion(ifstream& f_ent, ofstream& f_sal, string opcode, coleccionITF<programa>& c){
    bool fallo = false;
    string nom, leng, desc, dia, mes, anyo;
    fecha f;
    programa p;

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
    coleccionITF<programa> c;
    crear(c);

    ifstream f_ent;
    ofstream f_sal;

    if(abrirFichero(f_ent, f_sal)){
        string opcode;
        bool fallo = false;
        while(!f_ent.eof() && !fallo){
            leerInstr(f_ent, opcode);
            fallo = realizarInstruccion(f_ent, f_sal, opcode, c);
        }
        cerrarFichero(f_ent, f_sal);
    }

    return 0;
}