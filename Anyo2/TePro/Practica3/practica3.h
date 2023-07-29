/**
 * @file practica3.h
 * @author Dorian Boleslaw Wozniak
 * @brief Esecificación e implementación de las clases y métodos necesarios
 *          para simular un servicio de paquetería siguieno las necesidades de
 *          main.cc 
 */

#pragma once

#include <iomanip>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

class Elemento {
    protected:
        double _vol, _peso;
        string _nom;

        Elemento(const string& n, double v, double p)
            : _vol(v), _peso(p), _nom(n) {}

    public:
        double volumen() const
            { return _vol; }

        virtual double peso() const 
            { return _peso; }

        string nombre() const
            { return _nom; }

        virtual string to_string() const {
            stringstream ss;
            ss << nombre() << " ["
                << fixed << setprecision(1) << std::to_string(volumen()) << " m3 ] ["
                << fixed << setprecision(1) << std::to_string(peso()) << " kg ]";
            return ss.str();
        }

        string tipo() const 
            { return " de Carga Estandar"; }

        friend ostream& operator<<(ostream& os, const Elemento& p);
};

ostream& operator<<(ostream& os, const Elemento& p) 
    { os << p.to_string(); return os; }

class Carga : public Elemento {
    public:
        using esAlmacenable = bool;
        Carga(const string& n, double v, double p) 
            : Elemento(n, v, p) {}
};

template <class T>
class Almacen {
    protected:
        typename T::esAlmacenable ok;
        double _cap;
        vector<T*> _cont;

        Almacen(double c) 
            : _cap(c) {}

    public:
        bool guardar(T& elemento) {
            double _ocup = 0.0;
            for (const T* e : _cont) _ocup += e -> volumen();

            if (_ocup + elemento.volumen() <= _cap) 
                { T* aux = &elemento; _cont.push_back(aux); return true; }

            return false;
        }
};

class Camion : public Almacen<Carga>, public Elemento {
    public:
        Camion(double c)
            : Almacen(c), Elemento("Camion", c, 0.0) {}

        double peso() const override { 
            double peso = 0.0; 
            for (auto e : _cont) peso += e -> peso();
            return peso; 
        }

        string to_string() const override {
            stringstream ss;

            ss << nombre() << " ["
                << fixed << setprecision(1) << _cap << " m3] ["
                << fixed << setprecision(1) << peso() << " kg]";
            
            for (auto e : _cont) ss << endl << e -> to_string();
            return ss.str();
        }
};

template <class T>
class Contenedor : public Almacen<T>, public Carga {
    public:
        Contenedor(double c)
            : Almacen<T>(c), Carga("Contenedor", c, 0.0) {}

        double peso() const override { 
            double peso = 0.0; 
            for (auto e :  Almacen<T>::_cont) peso += e -> peso();
            return peso;
        }

        string to_string() const override {
            stringstream ss;

            ss << nombre() << " ["
                << fixed << setprecision(1) << Almacen<T>::_cap << " m3] ["
                << fixed << setprecision(1) << peso() << " kg]"
                << Almacen<T>::_cont.front() -> tipo();
            
            for (const T* e :  Almacen<T>::_cont) ss << endl << e -> to_string();
            return ss.str();
        }
};


class Producto : public Carga {
    public:
        Producto(const string& n, double v, double p)
            : Carga(n, v, p) {}
};

class SerVivo : public Elemento {
    public:
        using esAlmacenable = bool;
        SerVivo(const string& n, double v, double p)
            : Elemento(n, v, p) {}
        string tipo() const 
            { return " de de Seres Vivos"; }
};

class Toxico : public Elemento {
    public:
        using esAlmacenable = bool;
        Toxico(const string& n, double v, double p)
            : Elemento(n, v, p) {}
        string tipo() const 
            { return " de Productos Toxicos"; }
};