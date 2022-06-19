/* calcMejor.y fichero para la practica 3 de Teoria de la Computacion  */
%{
#include <stdio.h>
%}
%token NUMBER EOL CP OP // Terminales (tokens) que no son operandos
%start calclist			// No terminal inicial
%token ADD SUB			// Tokens operando
%token MUL DIV

%%
 // Imprime el resultado de la operacion en pantalla si reconoce una expresión matemática
calclist : /* nada */
	| calclist exp EOL { printf("=%d\n", $2); }
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