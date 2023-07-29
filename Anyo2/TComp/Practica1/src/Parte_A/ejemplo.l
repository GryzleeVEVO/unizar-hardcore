    // Ejemplo: sustituye todas las instancias de "aa" por "a"    

%{
    // 1°: Directivas de inclusión y variables globales (CON UN ESPACIO EN BLANCO MÍNIMO)
    #include <stdio.h>
    int contador = 0;
%}

%%

    // 2°: Expresiones regulares + acciones a realizar en caso de cumplirse
    // (SIN ESPACIOS EN BLANCO)

aa  {contador ++; printf("a");}

%%

    // 3°: El programa de C en sí

int main(){
    int error = yylex();

    if(error == 0){
        printf ("Total de aa : %d\n", contador);
    }

    return error;
}