/*
	Teoría de la computación
	Práctica 4

	Autores:	Dorian Boleslaw Wozniak		817570
				David Nicolás				715753

	Descripción: Analizador sintáctico de grafos que permite obtener el camino
	más corto de un nodo inicial a uno final a partir de potencias de una matriz
	de adyacencia. El programa, en concreto, está diseñado para resolver el
	problema de las torres de Hanoi

	Para la ejecución del programa:

		(En caso de compilarlo manualmente)

		flex th.l
		bison -yd th.y
		gcc lex.yy.c y.tab.c -lfl -o th

		./th < thP3D3.txt 				(resultado por salida estandar)

		./th < thP3D3.txt > salida.txt	(resultado redirigido a salida.txt)
*/


%{
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int yyerror();
int yylex();

/* Constantes globales */

#define PALOS 3
#define DIM 27
#define BUFF 4000

/* 
    Almacena temporalmente los pares nodos origen-fin y transiciones 
    antes de introducirlas en la tabla de adyacencia
*/

struct ListaTransiciones{
    char* nodoOrig;
    char* nodosFin[DIM];
    char* etiquetas[DIM];

    int total;
} listaTr;

struct NodosConocidos{
	char* nodo[DIM];

	int total;
} listaNodo;

/* Tabla de adyacencia */
char* tablaTr[DIM][DIM];

/* Inicializa tabla de adyacencia */
void iniTabla(char* tabla[DIM][DIM]) {
	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {
			tabla[i][j] = "";
		}
	}
}

/* Multiplica tabla */
void multiplicar(char* t1[DIM][DIM], char* t2[DIM][DIM], char* res[DIM][DIM]) {
	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {
			res[i][j] = (char*) calloc(BUFF, sizeof(char));
			for (int k = 0; k < DIM; k++) {
				if (strcmp(t1[i][k],"")!=0 && strcmp(t2[k][j],"") != 0) {
					strcat(strcat(res[i][j],t1[i][k]),"-");
					strcat(res[i][j],t2[k][j]);
				}
			}
		}
	}
}

/* Copia la tabla de adyacencia */
void copiar(char* orig[DIM][DIM], char* copia[DIM][DIM]) {
	for (int i = 0; i < DIM; i++) {
		for (int j = 0; j < DIM; j++) {
			copia[i][j] = strdup(orig[i][j]);
		}
	}
}


 /* Imprime una tabla de tamaño DIM * DIM*/
void imprimirTabla(char* tabla[DIM][DIM]){
	printf("\t\t");

	for(int i = 0; i < DIM; i++){
		printf("%s\t", listaNodo.nodo[i]);
	}

	printf("\n\n");

	for(int i = 0; i < DIM; i++){
		printf("%s|\t", listaNodo.nodo[i]);
		for(int j = 0; j < DIM; j++){
			printf("%s\t", tablaTr[i][j]);
		}

		printf("\n");
	}

	printf("\n\n");
}

/* Obtiene la posición de un nodo conocido en tablaTr; si no es conocido, lo añade */
int posNodo(struct NodosConocidos* listaNodo, char* nodo){

	for(int i = 0; i < listaNodo -> total; i++){
		if(strcmp(listaNodo -> nodo[i], nodo) == 0){
			return i;
		}
	}
	
	listaNodo -> nodo[listaNodo -> total] = strdup(nodo);
	listaNodo -> total++;

	return listaNodo -> total - 1;
}

/* Introduce la regla obtenida por el analizador a la tabla de adyacencia */
void introducirEnTabla(struct ListaTransiciones listaTr, struct NodosConocidos* listaNodo, char* tablaTr[DIM][DIM]){
	int nodoIzda = posNodo(listaNodo, listaTr.nodoOrig);

	for(int i = 0; i < listaTr.total; i++){
		int nodoDcha = posNodo(listaNodo, listaTr.nodosFin[i]);

		tablaTr[nodoIzda][nodoDcha] = strdup(listaTr.etiquetas[i]);
	}
}

/* Devuelve una cadena con el camino mas corto de un origen a su destino
	calculando potencias de una tabla de adyacencia */

char* calcularCaminoMasCorto(char* orig, char* dest, char* tablaTr[DIM][DIM], struct NodosConocidos* listaNodo){

	/* Obtiene indices de los estados inicial y final */
	int posIni = posNodo(listaNodo, orig);
	int posFin = posNodo(listaNodo, dest);

	/* Crea una matriz potencia, que ahora representa tablaTr ^ 1*/
	char* pot[DIM][DIM];
	char* aux[DIM][DIM];

	copiar(tablaTr, pot);

	do{
		copiar(pot, aux);
		multiplicar(tablaTr, aux, pot);

	}while(!(strcmp(aux[posIni][posFin], "") == 0 && strcmp(pot[posIni][posFin], "") != 0));

	return pot[posIni][posFin];
}

%}

/* Nuevo tipo para yylval */
%union{
    char* nombre;
}

/* No terminal inicial */
%start grafo

/* Tokens utilizados */
%token GRAPH OB CB OP CP COMMA SEMICOLON EDGEOP
%token<nombre> ID NODE INPUT						/* yylval de tipo char* */

/* No terminales de tipo char* */
%type<nombre> edge									

%%

/* Inicial: graph <etiq> { ... } */
grafo : GRAPH ID OB listaGrafo CB

/* { ... ; ... ;} */
listaGrafo : /* Vacío */                    
            | regla SEMICOLON listaGrafo    

/* p_0 -> ... */
/* Añade nodo original a listaTr, y añade transiciones a tabla de adyacencia */
regla : NODE EDGEOP derecha                 { 
												listaTr.nodoOrig = strdup($1);
												introducirEnTabla(listaTr, &listaNodo, tablaTr);
												listaTr.total = 0;
											}

/* p_1... , p2... ,..., pn... */
/* Añade cada nueva parte derecha a listaTr*/
derecha : NODE edge COMMA derecha            {
												listaTr.nodosFin[listaTr.total] = strdup($1);
												listaTr.etiquetas[listaTr.total] = strdup($2);
												listaTr.total++;	
											}

        | NODE edge                         {
												listaTr.nodosFin[listaTr.total] = strdup($1);
												listaTr.etiquetas[listaTr.total] = strdup($2);
												listaTr.total++;	 
											}

/* (<etiq>) */
/* 
	Obtiene el nombre de la entrada para la transición.
	Si existe, es una cadena alfanumérica más "_" sin espacios
	Si no existe (transición vacía), es un espacio en blanco
*/
edge: OP CP                                 { $$ = " "; }
    | OP INPUT CP                           { $$ = $2; }

%%

int yyerror(char* s) {
	printf("%s\n", s);
	return -1;
}

int main() {

    /* Inicializa variables */
	listaTr.total = 0;
	listaNodo.total = 0;
	iniTabla(tablaTr);

	for(int i = 0; i < DIM; i++){
		listaNodo.nodo[i] = "";
	}

    /* Nodos inicial y final */
	char* estadoIni = "000";
	char* estadoFin = "111";
	
    /* Comienza el parsing */
	int error = yyparse();

	if(error == 0){
		char* camino = calcularCaminoMasCorto(estadoIni, estadoFin, tablaTr, &listaNodo);
	

		printf("Nodo inicial  : %s\n", estadoIni);
		printf("Movimientos   : %s\n", camino);
		printf("Nodo final    : %s\n", estadoFin);
	}

	return error;
}