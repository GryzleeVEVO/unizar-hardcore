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
#include <stdio.h>

int yyerror();
int yylex();

int aBase(int num, int base);

extern int base;
%}

%token NUMBER EOL CP OP BASE EQU END ENDB // Terminales (tokens) que no son operandos
%start calclist			// No terminal inicial
%token ADD SUB			// Tokens operando
%token MUL DIV

%%
 // No terminal inicial: vacío, una expresión matemática o una asignación de base (número, no expresión)
calclist : /* nada */
	| calclist exp END EOL { printf("=%d\n", $2); }
    | calclist exp ENDB EOL { printf("=%d\n", aBase($2, base));}
    | calclist BASE EQU NUMBER EOL {base = $4;} /* El número NO puede ser una expresión matemática calculable */
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

/*
    Pre: 2 <= base <= 10
    Post: Convierte un número en base decimal a base b
*/

int aBase(int num, int base){
    int n_aux = num, exp = 1, numb = 0;

    while(1){
        numb = numb + n_aux % base * exp;

        if(n_aux / base == 0){
            return numb;
        }

        if(exp == 1){
            exp = 10;
        }

        else{
            exp = exp * 10;
        }

        n_aux = n_aux / base;
    }
}

int yyerror(char* s) {
   printf("\n%s\n", s);
   return 0;
}

int main() {
  yyparse();
}
