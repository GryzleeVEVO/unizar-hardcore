//
// Procesadores de lenguajes
// Práctica 2
// 
// Fichero: test2.cl
// 
// Autores:
//         Wozniak, Dorian Boleslaw  (817570@unizar.es)
//         Jaime Royo, Oscar    (818961@unizar.es)
//
//  Descripción: Programa de test que prueba la correcta declaración de
//  variables tanto globales como locales, funciones y asignaciones.
//  
//  El programa está hecho para probar el análisis sintáctico del compilador,
//  no para generar un programa util

int a, b[40], c, d[11];

bool e[30], f;

char g, h[11];


void proc1() {
    print_ln("No hago nada");
}

void proc2(int a) {
    print(int2char(a));
}

void proc3(bool a) {
    print(a);
}

void proc4(char a) {
    print(a);
}

void proc5(int a[5]) {
    print(int2char(a[2]));
}

void proc6(bool a[5]) {
    print(a[2]);
}

void proc7(char a[5]) {
    print(a[3]);
}

void proc8(int& a) {
    a = 1;
}

void proc9(bool& a) {
    a = true;
}

void proc10(char& a) {
    a = 'a';
}

void proc11(int& a[5]) {
    a[2] = 1;
}

void proc12(bool& a[5]) {
    a[2] = true;
}

void proc13(char& a[5]) {
    a[2] = 'a';
}

void proc14(bool a, char& b, int c[70], char& d[150]) {
    if (a) {
        print_ln("Escribo: ", b, " ", int2char(c[1]), " ", d[5]);
    }

    b = 'b';
    d[4] = 'a';
}

int func1() {
    return 1;
}

int func2(int a) {
    return a;
}

bool func3(bool a) {
    return a;
}

char func4(char a) {
    return a;
}

int func5(int a[5]) {
    return a[3];
}

bool func6(bool a[5]) {
    return a[3];
}

char func7(char a[5]) {
    return a[3];
}

int func8(int& a) {
    int aux;
    
    aux = a;
    a = 3;
    return aux;
}

bool func9(bool& a) {
    bool aux;
    
    aux = false;
    a = true;
    return aux;
}

char func10(char& a) {
    char aux;
    
    aux = a;
    a = 'a';
    return aux;
}

int func11(int& a[5]) {
    int aux;
    
    aux = a[3];
    a[3] = 3;
    return aux;
}

bool func12(bool& a[5]) {
    bool aux;
    
    aux = a[3];
    a[3] = false;
    return aux;
}

char func13(char& a[5]) {
    char aux;
    
    aux = a[3];
    a[3] = 'a';
    return aux;
}

int func14(bool a, char& b, int c[70], char& d[150]) {
    if (a) {
        print_ln("Escribo: ", b, " ", int2char(c[1]), " ", d[5]);
    }

    b = 'b';
    d[4] = 'a';

    return c[0];
}

void main() {
    int i, j[40], k, l[11];

    bool m[30], n;

    char o, p[11];

    int x[70];
    char y[150];

    a = 1;
    b[a] = 40;
    c = 15 + a;
    d[c % 2] = a * b[1];

    e[0] = 2 <= 3 || 15 >= 4;
    e[1] = a > c && c < d[0];
    e[2] = a == b[1] && c != d[0] || !e[0];

    f = !e[2] == true || e[1] == false;

    g = int2char(a);
    h[1] = 'Y'; h[2] = 'E'; h[3] = 'S';

    proc1();
    proc2(a);
    proc3(f);
    proc4(g);

    proc14(f, g, x, y);

    i = func1();
    j[0] = func2(a);

    n = func3(e[0]);

    k = func14(f, g, x, y);
}