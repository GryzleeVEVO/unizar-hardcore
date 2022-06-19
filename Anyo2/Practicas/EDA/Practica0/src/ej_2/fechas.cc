#include "fechas.h"

bool esBisiesto(int a) {
    return ((a % 4 == 0 && a % 100 != 0) || a % 400 == 0);
}

bool esFechaValida(int d, int m, int a) {
    bool valida = 1583 <= a && 1 <= m && m <= 12;
    if (valida) {
        switch (m) {
            case 4: case 6: case 9: case 11: 
                valida = 1 <= d && d <= 30; 
                break;
            case 1: case 3: case 5: case 7: case 8: case 10: case 12:
                valida = 1 <= d && d <= 31;
                break;
            case 2: 
                if (!esBisiesto(a)) {
                    valida = 1 <= d && d <= 28;
                }
                else
                    valida = 1 <= d && d <= 29;
                    break;
        }         
    }
    return valida;
}

bool crear(int dia, int mes, int anyo, fecha& f){
    if(esFechaValida(dia, mes, anyo)){
        f.suDia = dia;
        f.suMes = mes;
        f.suAnyo = anyo;
        return true;
    }

    else{
        return false;
    }
}

int elDia(const fecha f){
    return f.suDia;
}

int elMes(const fecha f){
    return f.suMes;
}

int elAnyo(const fecha f){
    return f.suAnyo;
}

bool iguales(const fecha f1, const fecha f2){
    return (elDia(f1) == elDia(f2) && elMes(f1) == elMes(f2) & elAnyo(f1) == elAnyo(f2));
}

bool anterior(const fecha f1, const fecha f2){
    return((elAnyo(f1) < elAnyo(f2)) || (elAnyo(f1) == elAnyo(f2) && elMes(f1) < elMes(f2))
        || (elAnyo(f1) == elAnyo(f2) && elMes(f1) == elMes(f2) && elDia(f1) < elDia(f2)));
}

bool posterior(const fecha f1, const fecha f2){
    return(!iguales(f1, f2) && !anterior(f1, f2));
}