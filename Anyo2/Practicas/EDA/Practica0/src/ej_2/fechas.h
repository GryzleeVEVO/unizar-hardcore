#ifndef FECHAS_H
#define FECHAS_H

struct fecha;

bool crear(int d, int m, int a, fecha& f);
int elDia(const fecha f);
int elMes(const fecha f);
int elAnyo(const fecha f);
bool iguales(const fecha f1, const fecha f2);
bool anterior(const fecha f1, const fecha f2);
bool posterior(const fecha f1, const fecha f2);

struct fecha{
    friend bool crear(const int d, const int m, const int a, fecha& f);
    friend int elDia(const fecha f);
    friend int elMes(const fecha f);
    friend int elAnyo(const fecha f);
    friend bool iguales(const fecha f1, const fecha f2);
    friend bool anterior(const fecha f1, const fecha f2);
    friend bool posterior(const fecha f1, const fecha f2);

    private:
        int suDia;
        int suMes;
        int suAnyo;
};

#endif