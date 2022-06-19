%{
    #include <stdio.h>
    int tl = 0, tb = 0, tc = 0, td = 0, tv = 0;
%}

%%
^[\t ][\t ]*[\t ]\n                 {tl++; tb++; tc = tc + yyleng - 1;}
\n                                  {}
^[aeiouAEIOU].*[0-9].*(.$)?         {tl++; td++; tv++; tc = tc + yyleng;}
^[aeiouAEIOU].*(.$)?                {tl++; tv++; tc = tc + yyleng;}
(^..*[0-9].*(.$)?)|(^[0-9].*(.$)?)  {tl++; td++; tc = tc + yyleng;}
^..*(.$)?                           {tl++; tc = tc + yyleng;}

%%

int main(){
    int error = yylex();

    if(error == 0){
        printf("TL: %d\n", tl);
        printf("TB: %d\n", tb);
        printf("TC: %d\n", tc);
        printf("TD: %d\n", td);
        printf("TV: %d\n", tv);
    }

    return error;
}