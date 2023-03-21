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
//  Descripción: Programa de test que prueba las funciones integradas
//  de lectura y escritura de datos en pantalla
//  
//  El programa está hecho para probar el análisis sintáctico del compilador,
//  no para generar un programa util

void main() {
    char a, b, c, d, e, f, g, h, y;
    int x;


    print_ln("Introduce ", " tantos caracteres como quieras");
    read_ln(a);

    print_ln("He leido solo: ", a, " Introduce mas");
    read_ln(b, c, d);

    print("He leido: ", b, " ", c, " ", d, " ", " Introduce ahora un caracter ");
    read(e);

    print(" He leido: ", e, " Introduce ahora tres caracteres ");
    read(f, g, h);
    
    print_ln("He leido: ", f, " ", g, " ", h);

    x = char2int(a);
    y = int2char(x);
}