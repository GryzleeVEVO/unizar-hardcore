/*
    BASES DE DATOS
    Práctica 1: Liga de fútbol
    Fichero: obtenerNombresCortos.cpp
    Autores:    Álvaro Seral Gracia                 816456  816456@unizar.es
                Cristian Andrei Selivanov Dobrisan  819425  819425@unizar.es
                Dorian Boleslaw Wozniak             817570  817570@unizar.es
    Resumen: Programa que, una vez obtenido un archivo "nombresCortos" a
        partir de LigaHost.txt, obtenga una lista de nombres cortos únicos de
        cada participante en el sistema de ligas

        Nota: El resultado crea una entrada "Rayo Vallecano     1" que se debe
        eliminar a mano

    Entregado el 4 de abril de 2022
*/

#include <iostream>
#include <fstream>
#include <regex>
#include <vector>
#include <algorithm>

using namespace std;

int main() {

    // Abre los archivos a leer y escribir
    ifstream f;
    ofstream g;

    f.open("nombresCortos");
    g.open("nombresCortosLimpio");

    // s almacena la línea leida del archivo, t almacena la línea tras
    // aplicarle una sustitución
    string s, t;

    // Vector de cadenas que almacena los nombres únicos de los equipos
    vector<string> v;

    while(!f.eof()) {
        // Obtiene siguiente línea
        getline(f, s);
        // Elimina espacios en blanco al principio y al final de la línea
        t = regex_replace(s, regex("^ +| +$|( ) +"), "$1");

        // Si no encuentra el nombre en el vector, lo añade
        if (find(v.begin(), v.end(), t) == v.end()) {
            v.push_back(t);
        }
    }

    // Vacía el vector, escribiendo sus contenidos en "nombresCortosLimpio"
    while(!v.empty()){
        g << v.at(v.size() - 1) << endl;
        v.pop_back();
    }

    // Cierra archivos
    f.close();
    g.close();

    return 0;
}
