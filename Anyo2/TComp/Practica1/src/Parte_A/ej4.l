%{
    #include <stdio.h>
%}

%%
[0-9]   {printf("%d", atoi(yytext) + 1);}
\n      {printf("\n\n");}
%%

int main(){
    int error = yylex();
    return error;
}