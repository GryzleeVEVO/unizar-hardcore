    //  Teoría de la computación
    //  Práctica 2
    //  Ejercicio 1
    //  Autores: Dorian Boleslaw Wozniak        817570
    //           David Nicolás                  715753

%{
#include <stdio.h>
%}

%x linea_entera acotado
%%
<INITIAL>\"(.*(\/\/|\/\*).*)+\"	{printf("%s", yytext);}
<INITIAL>\/\/					{BEGIN(linea_entera);}
<INITIAL>\/\*					{BEGIN(acotado);}

<linea_entera>.*.*$				{BEGIN(INITIAL);}	
<acotado>.*\*\/					{BEGIN(INITIAL);}
<acotado>(.*\*[^/])*[^*]*\n		{}
%%

int main(){
	int error = yylex();
	return error;
}
