/*
    Teoría de la computación
    Práctica 3
    Ejercicio 2, apartado 3
    Autores:    Dorian Boleslaw Wozniak     817570
                David Nicolás               715753

    Descripción: Mejora de la calculadora donde hay una variable acumulador
    (inicialmente 0) a la que se puede asignar como valor una expresión
    matemática. "acum" también puede actuar como número, si no está seguido
    de un símbolo de asignación ":="
*/

%{
#include "y.tab.h"

int acum = 0;
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

 /* Tokeniza el operando de asignación */
":="            {return(ASIGN);}

 /* Tokeniza el identificador acumulador */
"acum"          {yylval = acum ; return(ACUM);}

 /* Tokeniza el número, con su valor literal como atributo */
[0-9]+	        {yylval = atoi(yytext); return(NUMBER);}

 /* Tokeniza el fin de línea */
\n	            {return(EOL);}

 /* Ignora espacios (bison no analizará hasta el siguiente no espacio) */
[ \t]	        {/* ignorar espacios */}

 /* Carácter inesperado */
.	            {return(yytext[0]);}

%%