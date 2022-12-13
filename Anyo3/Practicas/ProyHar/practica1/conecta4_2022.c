/*
    Proyecto Hardware
    Práctica 1

    Fichero: conecta4_2022.c

    Autores:
        Dorian Boleslaw Wozniak   (817570@unizar.es)
        Pablo Latre Villacampa    (778543@unizar.es)

    Descripción: Implementación de un juego conecta 4. Especificamente, el
					fichero contiene:
					
		- conecta4_jugar(): Bucle principal del juego
		- conecta4_hay_linea() y conecta4_buscar_alineamiento(): Obtienen
			el número de celdas del mismo color consecutivas máximas y, si
			hay una línea con 4 celdas o más, gana la partida
		- C4_calcular_fila(): Obtiene la celda donde introducir una ficha a
			partir de una columna
		- C4_actualizar_tablero: Actualiza la posición dada con una ficha nueva
		- C4_comprobar_empate(): Acaba el juego si el tablero está lleno y
			nadie ha ganado
*/

#include "conecta4_2022.h"
#include "entrada.h"

/* --- FUNCIONES INTERNAS --- */

// Devuelve la primera fila disponible para introducir una ficha en la columna
// dada, o ERROR si la columna está fuera del tablero o si se intenta introducir
// la ficha en una fila fuera del tablero
uint8_t 
C4_calcular_fila(CELDA cuadricula[TAM_FILS][TAM_COLS], uint8_t columna) {
	uint8_t fila = 1;

	if ((columna < 1) || (columna > NUM_COLUMNAS)) return ERROR; 

	while ((fila <= NUM_FILAS) 
			&& (celda_vacia(cuadricula[fila][columna]) == FALSE))
		fila++;
	
	return fila <= NUM_FILAS ? fila : ERROR;
}

// Introduce una ficha de color val en @cuadricula[fila][columna]
void C4_actualizar_tablero(CELDA cuadricula[TAM_FILS][TAM_COLS], 
							uint8_t fila, uint8_t columna, uint8_t val)
	{ celda_poner_valor(&cuadricula[(fila)][(columna)], val); }



// Devuelve verdad sólo si el tablero está lleno sin que ningún jugador
// haya ganado
int C4_comprobar_empate(CELDA cuadricula[TAM_FILS][TAM_COLS]) {

	for (uint8_t i = 1; i <= NUM_COLUMNAS; i++)
		if (celda_vacia(cuadricula[NUM_FILAS][i]))
			return FALSE;
	return TRUE;
}
/* -------------------------------------------------------------------------- */

/* --- FUNCIÓN BUSCAR_ALINEAMIENTO --- */
/*
	Obtiene el número de celdas sucesivas desde cuadricula[fila][columna] 
	del mismo color dada una dirección delta_fila, delta_columna
*/
uint8_t conecta4_buscar_alineamiento_c
	(CELDA cuadricula[TAM_FILS][TAM_COLS],
	uint8_t fila, uint8_t columna, uint8_t color, 
	int8_t delta_fila, int8_t delta_columna) {

	// Acaba si la posición está fuera del tablero
	if (!C4_fila_valida(fila) || !C4_columna_valida(columna)) return 0;

	// Acaba si la celda es vacía o de otro color
	if (celda_vacia(cuadricula[fila][columna]) 
		|| (celda_color(cuadricula[fila][columna]) != color))
		return 0;

	// Avanza índices
	uint8_t nueva_fila = fila + delta_fila;
	uint8_t nueva_columna = columna + delta_columna;

	// Añade 1 y llamada recursiva
	return 1 + conecta4_buscar_alineamiento_c(cuadricula, nueva_fila, nueva_columna, color, delta_fila, delta_columna);
}
/* -------------------------------------------------------------------------- */

/* --- FUNCIONES CONECTA4_HAY_LINEA --- */
/*
	Comprueba que con la ficha introducida hay una línea de 4 fichas del mismo
	color o más.

	Devuelve TRUE solo si ha encontrado dicha línea, 
	en caso contrario devuelve FALSE

	Se definen dos versiones de la función en este fichero:

	- conecta4_hay_linea_c_c: Versíon en C con llamadas a versión de
								buscar_alineamiento en C
	- conecta4_hay_linea_c_arm: Versíon en C con llamadas a versión de
								buscar_alineamiento en ensamblador
							
	Se comprueba línea en las siguientes líneas, en este orden:

		- Izquierda + derecha
		- Arriba + abajo
		- Diag. sup. izda. + Diag. inf. dcha.
		- Diag. inf. izda. + Diag. sup. dcha
*/

// C-C
uint8_t conecta4_hay_linea_c_c(CELDA cuadricula[TAM_FILS][TAM_COLS], 
								uint8_t fila, uint8_t columna, uint8_t color) {

	// Deltas 							
	int8_t deltas_fila[4] = {0, -1, -1, 1};
	int8_t deltas_columna[4] = {-1, 0, -1, -1};

	// Variables
	unsigned int i = 0;		// Índice
	uint8_t linea = FALSE;	// Se ha encontrado línea
	uint8_t long_linea = 0;	// Tamaño de línea

	for (i = 0; (i < 4) && (linea == FALSE); ++i) {
		// Comprueba en dirección de las deltas
		long_linea = conecta4_buscar_alineamiento_c
						(cuadricula, 
						fila, columna, color, 
						deltas_fila[i], deltas_columna[i]);

		linea = long_linea >= 4;
		if (linea) continue;

		// Comprueba en dirección contraria, suma a la línea encontrada antes
		long_linea += 
			conecta4_buscar_alineamiento_c
				(cuadricula, 
				fila - deltas_fila[i], columna - deltas_columna[i], color, 
				-deltas_fila[i], -deltas_columna[i]);

		linea = long_linea >= 4;
	}

	return linea;
}

// C-ARM
uint8_t conecta4_hay_linea_c_arm(CELDA cuadricula[TAM_FILS][TAM_COLS], 
								uint8_t fila, uint8_t columna, uint8_t color) {

	// Deltas 							
	int8_t deltas_fila[4] = {0, -1, -1, 1};
	int8_t deltas_columna[4] = {-1, 0, -1, -1};

	// Variables
	unsigned int i = 0;		  // Índice
	uint8_t linea = FALSE;	// Se ha encontrado línea
	uint8_t long_linea = 0;	// Tamaño de línea

	for (i = 0; (i < 4) && (linea == FALSE); ++i) {
		// Comprueba en dirección de las deltas
		long_linea = conecta4_buscar_alineamiento_arm
						(cuadricula, 
						fila, columna, color, 
						deltas_fila[i], deltas_columna[i]);

		linea = long_linea >= 4;
		if (linea) continue;

		// Comprueba en dirección contraria, suma a la línea encontrada antes
		long_linea += 
			conecta4_buscar_alineamiento_arm
				(cuadricula, 
				fila - deltas_fila[i], columna - deltas_columna[i], color, 
				-deltas_fila[i], -deltas_columna[i]);

		linea = long_linea >= 4;
	}

	return linea;
}
/* -------------------------------------------------------------------------- */
				
/* --- FUNCIÓN VERIFICAR_4_EN_LÍNEA --- */
/*
	Invoca la función o funciones hay_linea() y devuelve su resultado.
	
	Modificar según la versión a probar
*/
int C4_verificar_4_en_linea(CELDA cuadricula[TAM_FILS][TAM_COLS], 
							uint8_t fila, uint8_t columna, uint8_t color) {
	
	//uint8_t resultado_c_c 		= conecta4_hay_linea_c_c(cuadricula, fila, columna, color);
	uint8_t resultado_c_arm 	= conecta4_hay_linea_c_arm(cuadricula, fila, columna, color);
	//uint8_t resultado_arm_c 	= conecta4_hay_linea_arm_c(cuadricula, fila, columna, color);
	//uint8_t resultado_arm_arm = conecta4_hay_linea_arm_arm(cuadricula, fila, columna, color);
	//uint8_t resultado_arm_arm_opt1 = conecta4_hay_linea_arm_arm_opt1(cuadricula, fila, columna, color);
	//uint8_t resultado_arm_arm_opt2 = conecta4_hay_linea_arm_arm_opt2(cuadricula, fila, columna, color);					
	//uint8_t resultado_arm_arm_opt3 = conecta4_hay_linea_arm_arm_opt3(cuadricula, fila, columna, color);					
	//uint8_t resultado_arm_arm_opt4 = conecta4_hay_linea_arm_arm_opt4(cuadricula, fila, columna, color);					

							
	/*
	if ( resultado_c_c != resultado_c_arm
			&& resultado_c_c != resultado_arm_c
			&& resultado_c_c != resultado_arm_arm
			&& resultado_c_c != resultado_arm_arm_opt1
			&& resultado_c_c != resultado_arm_arm_opt2
			&& resultado_c_c != resultado_arm_arm_opt3
			&& resultado_c_c != resultado_arm_arm_opt4

		)
		while(1);
	*/
								
	return resultado_c_arm;
}
/* -------------------------------------------------------------------------- */

/* --- FUNCIÓN DE JUEGO PRINCIPAL --- */
void conecta4_jugar(void) {

	/* --- MEMORIA Y DEFINICIONES --- */
	// La memoria estática comienza en @0x40000000

	// Vector de entrada
	static volatile uint8_t entrada[8] = {0, 0, 0, 0, 0, 0, 0, 0};

	// Tablero a usar.
	#include "tableros.h" 
	// Vector de movimientos. Descomentar para jugar automaticamente
	#include "jugadas.h" 
	// La tabla y vector de movimientos a utilizar se definen en los
	// ficheros incluidos con los símbolos TABLERO y JUGADA

	/* --- JUEGO --- */
	uint8_t column, row, colour;
	colour = 1; //Empiezan blancas 
	
	// Índice para vector de movimientos
	#ifdef JUGADAS_H 
		uint8_t i = 0; 
	#endif // JUGADAS_H
	

	while (1) {

		// Obtiene siguiente movimiento de JUGADA. Si no hay mas movimientos,
		// se debe introducir un movimiento manualmente
		#ifdef JUGADAS_H
			if (JUGADA[i] != 0) 
				{ entrada[0] = 1; entrada[1] = JUGADA[i]; i++; }
		#endif // JUGADAS_H
			
		// Espera a siguiente movimiento
		while (entrada_nueva(entrada) == 0);

		// Obtiene columna introducida, y la fila donde introducir 
		// siguiente ficha
		column = entrada_leer(entrada);
		row = C4_calcular_fila(TABLERO, column);

		if (C4_fila_valida(row) && C4_columna_valida(column)) {
			
			// Si es válido, introduce el movimiento
			C4_actualizar_tablero(TABLERO, row, column, colour);

			// Comprueba si se ha ganado. Según lo definido se para o deshace
			// la jugada
			if (C4_verificar_4_en_linea(TABLERO, row, column, colour))
				#ifndef UNDO_IF_WIN
					while(1);
				#else
					celda_borrar_valor(&TABLERO[(row)][(column)]);
				#endif
					
			// Comprueba si hay empate. En caso afirmativo para
			if (C4_comprobar_empate(TABLERO))
				while (1);

			// Pasa turno
			colour = C4_alternar_color(colour);
		}

		// Limpia vector entrada
		entrada_inicializar(entrada);
	}
}
