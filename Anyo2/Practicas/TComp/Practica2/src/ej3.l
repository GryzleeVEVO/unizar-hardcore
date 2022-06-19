    //  Teoría de la computación
    //  Práctica 2
    //  Ejercicio 3
    //  Autores: Dorian Boleslaw Wozniak        817570
    //           David Nicolás                  715753

%{
    #include <stdio.h>
    int cifras = 0, caracteres = 0, tb = 0, tt = 0, estadoAnterior;
%}

%s dominio otro_recurso recurso_bitly recurso_tiny comentario
%%

http:\/\/                                                           {BEGIN(dominio);}

<dominio>bit\.ly[ ]*\n                                              {BEGIN(recurso_bitly);}
<dominio>tinyurl\.com[ ]*\n                                         {BEGIN(recurso_tiny);}
<dominio>[a-zA-Z0-9]+(\.[a-zA-Z]+)?[ ]*\n                           {BEGIN(otro_recurso);}

<recurso_bitly,recurso_tiny,otro_recurso>[a-zA-Z]                   {caracteres++;}
<recurso_bitly,recurso_tiny,otro_recurso>[0-9]                      {cifras++;}

<recurso_bitly>[ \n]*                                               {
                                                                        if(caracteres % 3 == 0 && cifras % 2 != 0){
                                                                            tb++;
                                                                        }
                                                                        caracteres = 0; cifras = 0;
                                                                    }
                                                                    
<recurso_tiny>[ \n]*                                                {
                                                                        if(caracteres % 3 != 0 && cifras % 2 == 0){
                                                                            tt++;
                                                                        }
                                                                        caracteres = 0; cifras = 0;
                                                                    }

<otro_recurso>[ \n]*                                                {caracteres = 0; cifras = 0;}

<recurso_bitly,recurso_tiny,otro_recurso>#                          {estadoAnterior = YY_START ;BEGIN(comentario);}

<comentario>.*\n                                                    {BEGIN(estadoAnterior);}
%%

int main(){
    int error = yylex();
    if(error == 0){
        printf("TB: %d\nTC: %d", tb, tt);
    }
    return error;
}
