%{
    #include <stdio.h>
%}

%%
^[\t ][\t ]*[\t ]\n|\n          {}   
^[\t ][\t ]*                    {}
[\t ]*[\t ]$                    {}
[\t ][\t ]+                     {printf(" ");}
%%

int main(){
    int error = yylex();
    return error;
}