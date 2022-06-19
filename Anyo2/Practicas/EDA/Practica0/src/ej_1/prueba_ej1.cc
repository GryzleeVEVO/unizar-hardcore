#include "producto.h"
#include "contacto.h"
#include "agrupacion.h"
#include <iostream>
#include <iomanip>

using namespace std;

void imprimirCesta(agrupacion<producto>& cesta){
    cout << "CESTA" << endl;
    cout << setw(20) << left << "Nombre" << setw(20) << left << "Cantidad" << endl;

    iniciarIterador(cesta);
    producto p;
    bool seguir = siguiente(cesta, p);

    while(seguir){
        cout << setw(20) << left << nombre(p) << setw(20) << left << cantidad(p) << endl;
        seguir = siguiente(cesta, p);
    }

    cout << endl;
}

void imprimirAgenda(agrupacion<contacto> agenda){
    cout << "AGENDA" << endl;
    cout << setw(20) << left << "Nombre" << setw(40) << left << "Direccion" << setw(20) << left << "Numero" << endl;

    iniciarIterador(agenda);
    contacto c;
    bool seguir = siguiente(agenda, c);

    while(seguir){
        cout << setw(20) << left << nombre(c) << setw(40) << left << direccion(c) << setw(20) << left << telefono(c) << endl;
        seguir = siguiente(agenda, c);
    }

    cout << endl;   
}

void pertenece(bool b, string nom){
    if(b){
        cout << "El elemento " << nom << " esta en la agrupacion" << endl << endl;
    }

    else{
        cout << "El elemento " << nom << " NO esta en la agrupacion" << endl << endl;
    }
}


int main(){

    // CREAR AGRUPACION CESTA Y AÑADIR PRODUCTOS

    agrupacion<producto> cesta;
    producto nar, per;
    iniciar(cesta);
    crear("Naranjas", 5, nar);
    crear("Peras", 8, per);
    anyadir(cesta, nar);
    anyadir(cesta, per);

    // ESCRIBIR EN PANTALLA LA CESTA

    imprimirCesta(cesta);

    // BORRAR EL ULTIMO ELEMENTO Y VOLVER A ESCRIBIR EN PANTALLA

    cout << "Eliminando elemento Peras..." << endl;
    borrarUltimo(cesta);
    cout << endl;

    // COMPROBAR QUE ESTA EL ELEMENTO NARANJA Y EL ELEMENTO PERA

    pertenece(esta(cesta, nar), nombre(nar));
    pertenece(esta(cesta, per), nombre(per));

    // LO MISMO PARA CONTACTOS

    agrupacion<contacto> agenda;
    contacto a, b;
    iniciar(agenda);
    crear("Alice", "Calle Alfonso X 13", 976877323, a);
    crear("Bob", "Gran Via 53 2A", 675234400, b);
    anyadir(agenda, a);
    anyadir(agenda, b);

    imprimirAgenda(agenda);

    cout << "Eliminando contacto Bob" << endl;
    borrarUltimo(agenda);
    cout << endl;

    pertenece(esta(agenda, a), nombre(a));
    pertenece(esta(agenda, b), nombre(b));

    return 0;
}
