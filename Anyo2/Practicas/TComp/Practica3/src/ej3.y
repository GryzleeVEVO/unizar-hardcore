%{
#include <stdio.h>

int yylex();
int yyerror();
%}

%token X Y Z
%start s

%%

s: /* Nada */
    | c X s
    ;

b: X c Y
    | X c
    ;

c: X b X
    | Z
    ;

%%

int yyerror(char* s){
    printf("\n%s\n", s);
    return 0;
}

int main(){
    yyparse();
}
