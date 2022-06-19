/* calcMejor.l fichero para la practica 3 de Teoria de la Computacion */

%{
#include "y.tab.h"
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

 /* Tokeniza el número, con su valor literal como atributo */
[0-9]+	        {yylval = atoi(yytext); return(NUMBER);}

 /* Tokeniza el fin de línea */
\n	            {return(EOL);}

 /* Ignora espacios (bison no analizará hasta el siguiente no espacio) */
[ \t]	        {/* ignorar espacios */}

 /* Carácter inesperado */
.	            {return(yytext[0]); /* caracter inesperado */}

%%
