%{
#include "y.tab.h"
%}

%%

"x"     {return(X);}
"y"     {return(Y);}
"z"     {return(Z);}

.       {return(yytext[0]);}

%%
