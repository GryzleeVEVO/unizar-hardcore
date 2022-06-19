/*
    Teoría de la computación
    Práctica 3
    Ejercicio 2, apartado 1
    Autores:    Dorian Boleslaw Wozniak     817570
                David Nicolás               715753

    Descripción: Mejora de la calculadora donde se puede introducir
    números en bases entre 2 y 10 concatenando la letra 'b' tras un
    número (natural) sin espacios. Se asigna una nueva base de b de
    la forma "b = NUMERO". 'b' es por defecto 10.

    La conversión de una base arbitraria a decimal para su cálculo se
    realiza al tokenizar el número, siendo yylval el valor ya convertido
*/

%{
#include "y.tab.h"

int base = 10; /* La base quedará inicializada a 10 */
int aDecimal(int num, int base);
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

 /* Tokeniza el número tras convertirlo a base 10 desde la base b (si es válido)*/
[0-9]+b         {
                    yytext[yyleng - 1] = '\0';              /* Elimina la b de la cadena */
                    yylval = aDecimal(atoi(yytext), base);  /* El valor literal será su valor en base decimal */
                    return(NUMBER);
                }

 /* Tokeniza el número, con su valor literal como atributo */
[0-9]+	        {yylval = atoi(yytext); return(NUMBER);}

 /* Tokeniza el fin de línea */
\n	            {return(EOL);}

 /* Ignora espacios (bison no analizará hasta el siguiente no espacio) */
[ \t]	        {/* ignorar espacios */}

 /* Carácter inesperado */
.	            {return(yytext[0]);}
%%

/*
    Pre: 2 <= base <= 10
    Post: Devuelve numero, que está en una base entre 2 y 10, en base 10
            Si el número no es valido en la base dada, devuelve el mismo número
*/

int aDecimal(int num, int base){
    int n_aux = num, exp = 1, decimal = 0;

    while(1){
        if(n_aux % 10 < base){
            decimal = decimal + n_aux % 10 * exp;

            if(n_aux / 10 == 0){
                return decimal;
            }

            if(exp == 1){
                exp = base;
            }

            else{
                exp = base * exp;
            }

            n_aux = n_aux / 10;
        }

        else{
            return num;
        }
    }
}