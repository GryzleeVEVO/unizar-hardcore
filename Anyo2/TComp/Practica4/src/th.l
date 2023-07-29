/*
	Teoría de la computación
	Práctica 4

	Autores:	Dorian Boleslaw Wozniak		817570
				David Nicolás				715753

	Descripción: Analizador léxico que tokeniza textos que describen grafos
    de la forma:

    graph <etiqueta> {
        p0 -> p1_(ent1), p2(), ... , pn(entn);
        ...
        ...
    }

    El programa no tiene en cuenta los espacios en blanco, tabulaciones y
    saltos de línea al analizar su sintáxis (en th.y)
*/

%{
#include <string.h>
#include "y.tab.h"
%}

%s graphID nodeID edgeID

%%

 /* Reconoce el nombre del grafo (obtiene valor textual pero no es necesario) */
<graphID>[a-zA-Z0-9_]+                      {  
                                                yylval.nombre = strdup(yytext);
                                                BEGIN(INITIAL);
                                                return(ID);
                                            }

 /* Reconoce nombre del nodo */
<nodeID>[a-zA-Z0-9_]+                       {
                                                yylval.nombre = strdup(yytext);
                                                BEGIN(INITIAL);
                                                return(NODE);
                                            }

 /* Reconoce nombre de la entrada para que se produzca la transición */
<edgeID>[a-zA-Z0-9_]+                       {
                                                yylval.nombre = strdup(yytext);
                                                BEGIN(INITIAL);
                                                return(INPUT);
                                            }

 /* Reconoce paréntesis de apertura */
"("                                         {   
                                                BEGIN(edgeID);
                                                return(OP);
                                            }

 /* Reconoce paréntesis de cierre */
")"                                         {
                                                return(CP);
                                            }


 /* Reconoce la palabra clave "graph" */
"graph"                                     {
                                                BEGIN(graphID); 
                                                return(GRAPH);
                                            }

 /* Reconoce llave de apertura (inicio de lista) */
"{"                                         {
                                                BEGIN(nodeID);
                                                return(OB);
                                            }
    
 /* Reconoce llave de cierre (fin de lista) */
"}"                                         {
                                                return(CB);
                                            }

 /* Reconoce operador de arista */
"->"                                        {
                                                BEGIN(nodeID);
                                                return(EDGEOP);
                                            }


 /* Reconoce coma, que separa nodos finales y sus transiciones */
","                                         {
                                                BEGIN(nodeID);
                                                return(COMMA);
                                            }

 /* Reconoce punto y coma, que separa nodos iniciales */
";"                                         {
                                                BEGIN(nodeID);
                                                return(SEMICOLON);
                                            }
 /* Ignora espacios en blanco */
[ \t\n]+                                    {}

%%