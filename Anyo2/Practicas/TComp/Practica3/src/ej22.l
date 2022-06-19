/*
    Teoría de la computación
    Práctica 3
    Ejercicio 2, apartado 2
    Autores:    Dorian Boleslaw Wozniak     817570
                David Nicolás               715753

    Descripción: Mejora de la calculadora donde las expresiones matemáticas
    acaban con un ';'. Si acaba con ";b", mostrará el resultado en una base
    entre 2 y 10 (por defecto 10), asignable de la forma "base = NUMERO" (no
    expresión) 
*/

%{
#include "y.tab.h"

int base = 10;
%}

%%
 /* Tokeniza el operando suma */
"+"	            {return(ADD);}

 /* Tokeniza el operando resta */
"-"	            {return(SUB);}

 /* Tokeniza el operando multiplicación */
"*"	            {return(MUL);}

 /* Tokeniza el operando división */
"/"	            {return(DIV);}

 /* Tokeniza la apertura de paréntesis */
"("	            {return(OP);}

 /* Tokeniza la cerradura de paréntesis */
")"	            {return(CP);}

 /* Tokeniza el operando igualdad (asignaciíon de base)*/
"="             {return(EQU);}

 /* Tokeniza el simbolo de base*/
"b"             {return(BASE);}

 /* Tokeniza el final de expresión */
";"             {return(END);}

 /* Tokeniza el final de expresión con conversión*/
";b"            {return(ENDB);}

 /* Tokeniza el número, con su valor literal como atributo */
[0-9]+	        {yylval = atoi(yytext); return(NUMBER);}

 /* Tokeniza el fin de línea */
\n	            {return(EOL);}

 /* Ignora espacios (bison no analizará hasta el siguiente no espacio) */
[ \t]	        {/* ignorar espacios */}

 /* Carácter inesperado */
.	            {return(yytext[0]);}
%%

