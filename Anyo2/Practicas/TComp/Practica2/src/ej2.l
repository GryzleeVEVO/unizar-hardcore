    //  Teoría de la computación
    //  Práctica 2
    //  Ejercicio 2
    //  Autores: Dorian Boleslaw Wozniak        817570
    //           David Nicolás                  715753

%{
    #include <stdio.h>
    int dimTotal = 0;
    int dimAux = 1;
%}

%x analizarDimensiones
%%
<INITIAL>#{5}                     {BEGIN(analizarDimensiones); dimAux = 1;}
<INITIAL>[^#]*                  {}

<analizarDimensiones>[0-9]+     {dimAux = dimAux * atoi(yytext);}
<analizarDimensiones>\n         {dimTotal = dimTotal + dimAux; BEGIN(INITIAL);}
<analizarDimensiones>[^0-9\n]   {}
%%

int main(){
    int error = yylex();
    if(error == 0){
        printf("D: %d", dimTotal);
    }
    return error;
}
