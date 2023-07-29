%{
    #include <stdio.h>
    int contador = 0;
%}

%%
@hotmail.com    {printf("%s", yytext); contador++;}
%%

int main(){
    int error = yylex();
    if(error == 0){
        printf("\nNumero de correos hotmail: %d", contador);
    }

    return error;
}