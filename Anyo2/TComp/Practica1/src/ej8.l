%{
    #include <stdio.h>
%}

%%
I(((E*N){6})+)?E*F        {printf("--%s--", yytext);}
I((E*N){3})+E*F           {printf("-%s-", yytext);} 
I(E*N)+E*F                {printf("*%s*", yytext);}
%%

int main(){
    int error = yylex();
    return error;
}