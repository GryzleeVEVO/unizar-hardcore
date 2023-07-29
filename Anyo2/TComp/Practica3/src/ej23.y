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
#include <stdio.h>

int yyerror();
int yylex();

extern int acum;

%}

%token NUMBER EOL CP OP ACUM ASIGN  // Terminales (tokens) que no son operandos
%start calclist			            // No terminal inicial
%token ADD SUB			            // Tokens operando
%token MUL DIV

%%
 // No terminal inicial: vacío, una expresión matemática o una asignación de acum
calclist : /* nada */
	| calclist exp EOL {printf("=%d\n", $2);}
    | calclist asignacion exp EOL {acum = $3;}
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
        | acumulador
		;
 // Una asignación es un caracter "acum" seguido de ":"
asignacion : acumulador ASIGN
        ;

 // Un no terminal acum solo puede ser la variable "acum"
acumulador : ACUM
        ;
		
%%

int yyerror(char* s) {
   printf("\n%s\n", s);
   return 0;
}

int main() {
  yyparse();
}