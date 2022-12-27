/*
    BASES DE DATOS
    Práctica 1: Liga de fútbol
    Fichero: precalcularClasi.cpp
    Autores:    Álvaro Seral Gracia                 816456  816456@unizar.es
                Cristian Andrei Selivanov Dobrisan  819425  819425@unizar.es
                Dorian Boleslaw Wozniak             817570  817570@unizar.es
    Resumen: Programa que, a partir de un fichero con los partidos disputados
        y otro con todas las jornadas de todas las temporadas a insertar
        inicialmente, precalcula las clasificaciones para cada jornada de los
        equipos que disputan cada temporada de su división

    Entregado el 4 de abril de 2022
*/

#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <algorithm>

using namespace std;

int main() {

    // Abre archivos
    ifstream jornada_f, partido_f;
    ofstream compiten_f;

    jornada_f.open("jornada.csv"); partido_f.open("partido.csv");
    compiten_f.open("compiten.csv");

    // Formato de partido.csv: clvPartido;resLocal;resVisit;clvJornada;clvEqLocal;clvEqVisit
    string clvPartido, resLocal, resVisit, clvJornada, clvEqLocal, clvEqVisit;
    // Formato de jornada.csv: clvJornada;numero;clvTemporada
    string j_clvJornada, j_numero, j_clvTemporada;
    // Almacena la última jornada y temporada encontrada
    string last_clvJornada = "1", last_clvTemporada = "1";

    map<string, int> tabla;

    // Omite la primera linea de jornada
    getline(jornada_f, j_clvJornada, ',');
    getline(jornada_f, j_numero, ',');
    getline(jornada_f, j_clvTemporada);

    while (!partido_f.eof()) {
        // Lee siguiente línea de partido
        getline(partido_f, clvPartido, ',');
        getline(partido_f, resLocal, ',');
        getline(partido_f, resVisit, ',');
        getline(partido_f, clvJornada, ',');
        getline(partido_f, clvEqLocal, ',');
        getline(partido_f, clvEqVisit);

        // Nueva clave jornada
        if (last_clvJornada != clvJornada) {
            // Obtiene una nueva tabla con las clasificaciones actualizadas para la jornada
            multimap<int, string, greater<int>> tabla_ordenada;

            for (auto &pair : tabla) {
                tabla_ordenada.insert(make_pair(pair.second, pair.first));
            }

            int i = 1;

            for (auto &pair : tabla_ordenada) {
                compiten_f << last_clvJornada << "," << pair.second << "," << pair.first << "," << i << endl;
                i++;
            }

            // Como resultado, devuelve una tabla ordenada por puntos

            last_clvJornada = clvJornada;
            
            // Lee siguiente linea de jornada
            getline(jornada_f, j_clvJornada, ',');
            getline(jornada_f, j_numero, ',');
            getline(jornada_f, j_clvTemporada);

            // Nueva clave temporada
            if (last_clvTemporada != j_clvTemporada) {
                last_clvTemporada = j_clvTemporada;
                // Limpia el mapa para nueva temporada
                tabla.clear();            
            }
        }

        // Introduce en el mapa los equipos si no estan
        if (tabla.find(clvEqLocal) == tabla.end() ) {
            tabla.insert(pair<string, int>(clvEqLocal, 0));
        }
        
        if (tabla.find(clvEqVisit) == tabla.end() ) {
            tabla.insert(pair<string, int>(clvEqVisit, 0));
        }

        // Actualiza los resultados de la tabla según la victoria o derrota
        // Nota: no tiene en cuenta que anteriormente se otorgaban 2 puntos por victoria

        if (stoi(resLocal) > stoi(resVisit)) {
        tabla[clvEqLocal] +=3;  
        tabla[clvEqVisit] +=0;  
        }

        else if (stoi(resLocal) < stoi(resVisit)) {
        tabla[clvEqLocal] +=0;  
        tabla[clvEqVisit] +=3;   
        }

        else {
        tabla[clvEqLocal] +=1;  
        tabla[clvEqVisit] +=1;  
        }
    }

    // Saca tabla de última liga
    multimap<int, string, greater<int>> tabla_ordenada;

    for (auto &pair : tabla) {
        tabla_ordenada.insert(make_pair(pair.second, pair.first));
    }

    int i = 1;

    for (auto &pair : tabla_ordenada) {
        compiten_f << last_clvJornada << "," << pair.second << "," << pair.first << "," << i << endl;
        i++;
    }

    // Cierra los archivos
    jornada_f.close();
    partido_f.close();
    compiten_f.close();

    return 0;
}
