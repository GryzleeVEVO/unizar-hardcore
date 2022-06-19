%{
    #include <stdio.h>
%}

%%
@hotmail.com    {printf("@gmail.com");}
%%

int main(){
    int error = yylex();
    return error;
}