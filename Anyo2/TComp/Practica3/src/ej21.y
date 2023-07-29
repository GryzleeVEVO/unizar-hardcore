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
#include <stdio.h>

int yyerror();
int yylex();

extern int base;
%}

%token NUMBER EOL CP OP EQU BASE 	// Terminales (tokens) que no son operandos
%start calclist						// No terminal inicial
%token ADD SUB						// Tokens operando
%token MUL DIV

%%
 // No terminal inicial: vacío, una expresión matemática o una asignación de base (número, no expresión)
calclist : /* nada */
	| calclist exp EOL { printf("=%d\n", $2); }
	| calclist BASE EQU NUMBER EOL {base = $4;} 		/* El número NO puede ser una expresión matemática calculable */
	;

 // Una expresión es un factor, que puede sumar/restar a otra expresión
exp : 	factor 
	| exp ADD factor { $$ = $1 + $3; }
	| exp SUB factor { $$ = $1 - $3; }	
	;

 // Un factor es un factor simple, que puede multiplicar/dividir a otro factor
factor : 	factor MUL factorsimple { $$ = $1 * $3; }
		| factor DIV factorsimple { $$ = $1 / $3; }
		| factorsimple
		;

 // Un factor simple se genera con un número o una expresión entre paréntesis
factorsimple : 	OP exp CP { $$ = $2; }
		| NUMBER 
		;
		
%%

int yyerror(char* s) {
   printf("\n%s\n", s);
   return 0;
}

int main() {
  yyparse();
}
