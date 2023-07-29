/*
    Estructura de datos y algoritmos
    Práctica 1

    Autores: Dorian Boleslaw Wozniak        817570
             Jorge Pérez Liesa              821011

    Descripción: Archivo de implementación de un tipo fecha, que almacena una fecha
    válida del calendario gregoriano junto a operaciones de acceso y comparación de
    utilidad
*/

#include "fechas.h"

// IMPLEMENTACIÓN DEL MÓDULO fechas

// OPERACIONES AUXILIARES

/*
    Pre: true
    Post: Verdad si y solo si el entero es múltiplo de 4 y simultaneamente no
            es múltiplo de 100, o si el entero es múltiplo de 400 
*/

bool esBisiesto(int a) {
    return ((a % 4 == 0 && a % 100 != 0) || a % 400 == 0);
}

/*
    Pre: true
    Post: Verdad si y solo si la fecha es válida en el calendario gregoriano:

    Si es el 4°, 6°, 8° o 11° mes, solo si 1 ≤ d ≤ 30
    Si es el 1er, 3°, 5°, 7°, 8°, 10° o 12° mes, solo si 1 ≤ d ≤ 31
    Si es el 2° mes en un año bisiesto, solo si 1 ≤ d ≤ 29; en caso
    contrario, solo si 1 ≤ d ≤ 28
*/

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

// OPERACIONES EXPORTADAS

/*
    Pre: true
    Post: Verdad solo si f contiene una fecha valida cuyos valores son
    los enteros dia, mes y anyo
*/

bool crear(int dia, int mes, int anyo, Fecha& f){
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

/*
    Pre: true
    Post: Devuelve el dia de f
*/

int dia(const Fecha f){
    return f.suDia;
}

/*
    Pre: true
    Post: Devuelve el dia de f
*/

int mes(const Fecha f){
    return f.suMes;
}

/*
    Pre: true
    Post: Devuelve el mes de f
*/

int anyo(const Fecha f){
    return f.suAnyo;
}

/*
    Pre: true
    Post: Verdad solo si las fechas f1 y f2 son iguales, es decir, sus valores
            día, mes y año son iguales 
*/

bool iguales(const Fecha f1, const Fecha f2){
    return (f1.suDia == f2.suDia && f1.suMes == f2.suMes && f1.suAnyo == f2.suAnyo);
}

/*
    Pre: true
    Post: Verdad solo si la fecha f1 es anterior a la fecha f2, es decir:

    Solo si el año de f1 es menor que el año de f2; en caso de ser iguales,
    solo si el mes fe f1 es menor que el mes de f2; si tambien son iguales,
    solo si el dia de f1 es menor que el día de f2.
*/

bool anterior(const Fecha f1, const Fecha f2){
    return(
        (f1.suAnyo < f2.suAnyo) 
        || (f1.suAnyo == f2.suAnyo && f1.suMes < f2.suMes)
        || (f1.suAnyo == f2.suAnyo && f1.suMes == f2.suMes && f1.suDia < f2.suDia)
        );
}

/*
    Pre: true
    Post: Verdad solo si la fecha f1 es posterior es anterior a f2, es decir, ni
            son iguales ni f1 es anterior a f2
*/

bool posterior(const Fecha f1, const Fecha f2){
    return(!iguales(f1, f2) && !anterior(f1, f2));
}