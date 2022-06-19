/*
    Estructura de datos y algoritmos
    Práctica 2

    Autores: Dorian Boleslaw Wozniak        817570
             Jorge Pérez Liesa              821011

    Descripción: Archivo de especificación de un tipo fecha, que almacena una 
    fecha válida del calendario gregoriano junto a operaciones de acceso y 
    comparación de utilidad, según la especificación a continuación
*/

#ifndef FECHAS_H
#define FECHAS_H


/******************************************************************************/
/* ESPECIFIACIÓN                                                              */
/******************************************************************************/

/*
    Género fecha: Los valores del TAD fechas representan fechas válidas según 
                    las reglas del calendario gregoriano (adoptado en 1583)
*/

/******************************************************************************/
/* PREDECLARACIONES                                                           */
/******************************************************************************/

struct Fecha;

/******************************************************************************/
/* Operaciones del tipo                                                       */
/******************************************************************************/

/*
    parcial crear: entero d, entero m, entero a -> fecha

    Dados los tres valores enteros, se obtiene una fecha compuesta con los tres
    valores dados usando como día nes y año respectivamente

    Parcial: 1 ≤ d ≤ 31, 1 ≤ m ≤ 12, 1583 ≤ a, y además deben formar una fecha
    válida según el calendario gregoriano
*/

bool crear(int d, int m, int a, Fecha& f);

/*
    dia: fecha f -> entero

    Dada una fecha f, se obtiene el entero que corresponde al día en la fecha f
*/

int dia(const Fecha& f);

/*
    mes: fecha f -> entero

    Dada una fecha f, se obtiene el entero que corresponde al mes en la fecha f

*/

int mes(const Fecha& f);

/*
    año: fecha f -> entero

    Dada una fecha f, se obtiene el entero que corresponde al año en la fecha f
*/

int anyo(const Fecha& f);

/*
    iguales: fecha f1, fecha f2 -> booleano

    Dadas dos fechas f1 y f2, se obtienen un booleano con valor verdad si y solo
    si la fecha f1 es igual que la fecha f2, es decir, corresponden al mismo 
    día, mes y año
*/

bool iguales(const Fecha& f1, const Fecha& f2);

/*
    anterior: fecha f1, fecha f2 -> booleano

    Dadas dos fechas f1 y f2, se obtiene un booleano con valor verdad si y solo 
    si la fecha f1 es cronológicamente anterior a la fecha f2
*/

bool anterior(const Fecha& f1, const Fecha& f2);

/*
    posterior: fecha f1, fecha f2 -> booleano

    Dadas dos fechas f1 y f2, se obtiene un booleano con valor verdad si y solo 
    si la fecha f1 es cronológicamente posterior a la fecha f2
*/

bool posterior(const Fecha& f1, const Fecha& f2);

/******************************************************************************/
/* IMPLEMENTACIÓN                                                             */
/******************************************************************************/

/******************************************************************************/
/* Tipo Fecha                                                                 */
/******************************************************************************/

struct Fecha{
    friend bool crear(const int d, const int m, const int a, Fecha& f);
    friend int dia(const Fecha& f);
    friend int mes(const Fecha& f);
    friend int anyo(const Fecha& f);
    friend bool iguales(const Fecha& f1, const Fecha& f2);
    friend bool anterior(const Fecha& f1, const Fecha& f2);
    friend bool posterior(const Fecha& f1, const Fecha& f2);

    private:
        int suDia;
        int suMes;
        int suAnyo;
};

#endif