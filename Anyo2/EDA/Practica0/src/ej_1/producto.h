#ifndef PRODUCTO_H
#define PRODUCTO_H

#include<iostream>
using namespace std;

// PREDECLARACIONES

struct producto;

void crear(string nombre, int cantidad, producto& p);
string nombre(const producto& p);
int cantidad(const producto& p);
bool operator==(const producto& p1, const producto& p2);

// DECLARACION DEL STRUCT

struct producto {
  friend void crear(string nombre, int cantidad, producto& p);
  friend string nombre(const producto& p);
  friend int cantidad(const producto& p);
  friend bool operator==(const producto& e1, const producto& e2);
  
  private:
      string nombre;
      int cantidad;
};

#endif

