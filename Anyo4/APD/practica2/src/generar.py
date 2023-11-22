#!/bin/python3

"""
Fichero: generar.py
Autores:
    Seral Gracia, Álvaro (819425@unizar.es)
    Wozniak, Dorian Boleslaw (817570@unizar.es)

Descripción:
    El programa genera un cuadrado latino aleatorio de orden n válido, con una
    serie casillas en blanco.

    Uso: python generar.py <tamaño> <prob_casilla_blanco> <fichero_salida>
"""

import sys
import random


def is_valid_latin_square(square):
    """_summary_ : Comprueba si un cuadrado es latino válido.
    _param_ : square: Cuadrado a comprobar.
    """

    n = len(square)

    # Check rows
    for row in square:
        if sorted(row) != list(range(1, n + 1)):
            return False

    # Transpose the square to check columns
    square = list(map(list, zip(*square)))

    # Check columns
    for row in square:
        if sorted(row) != list(range(1, n + 1)):
            return False

    return True


def generate_random_latin_square(n):
    """_summary_ : Genera un cuadrado latino aleatorio de orden n válido.
    _param_ : n: Orden del cuadrado.
    """

    # Generate an initial Latin square
    square = [[(i + j) % n + 1 for j in range(n)] for i in range(n)]

    # Generate a random permutation
    permutation = list(range(1, n + 1))
    random.shuffle(permutation)

    # Use the permutation to rearrange the rows and columns
    square = [
        [square[permutation[i] - 1][permutation[j] - 1] for j in range(n)]
        for i in range(n)
    ]

    return square


def mask_latin_square(square, p):
    """_summary_: Enmascara casillas de un cuadrado latino.
    _param_: square: Cuadrado a enmascarar.
    _param_: p: Probabilidad de que una casilla se vuelva vacía.
    """
    # Mask each number with probability p
    for i in range(len(square)):
        for j in range(len(square[i])):
            if random.random() < p:
                square[i][j] = "*"
    return square


def mask_latin_square_percentage(square, percentage):
    """_summary_: Enmascara un porcentaje de casillas de un cuadrado latino.
    _param_: square: Cuadrado a enmascarar.
    _param_: percentage: Porcentaje de casillas a enmascarar.
    """
    # Calculate the number of squares to mask
    num_squares = round(len(square) * len(square[0]) * percentage)

    # Generate a list of all square positions
    positions = [(i, j) for i in range(len(square)) for j in range(len(square[i]))]

    # Randomly select and mask the squares
    masked_positions = random.sample(positions, num_squares)
    for i, j in masked_positions:
        square[i][j] = "*"

    return square


################################################################################

if len(sys.argv) != 4:
    print("Uso: python generar.py <tamaño> <prob_casilla_blanco> <fichero_salida>")
    sys.exit(1)

size = int(sys.argv[1])
probability = float(sys.argv[2])
output_file = sys.argv[3]

# Genera un cuadrado
square = generate_random_latin_square(size)

if not is_valid_latin_square(square):
    print("Error: el cuadrado generado no es latino válido")
    sys.exit(1)

masked_square = mask_latin_square_percentage(square, probability)

# Escribe el cuadrado en un fichero
with open(output_file, "w") as f:
    for row in masked_square:
        f.write(" ".join(str(num) for num in row))
        f.write("\n")
