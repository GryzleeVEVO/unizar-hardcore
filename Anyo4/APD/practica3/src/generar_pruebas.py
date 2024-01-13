#!/usr/bin/env python3

import os
import random
import sys
from typing import List

# Constants
LOWER_BOUND_N = 3
UPPER_BOUND_N = 900

# Type hint definitions
Edge_Matrix = List[List[bool]]
Edge_Times = List[List[int]]
Edge_Probabilities = List[List[float]]


def has_path(edges: Edge_Matrix, origin: int, destination: int) -> bool:
    """
    Returns True if there is a path from origin to destination.
    """
    visited = [False] * len(edges)

    # Uses DFS to find a path
    stack = [origin]
    while stack:
        node = stack.pop()
        if node == destination:
            return True
        if not visited[node]:
            visited[node] = True
            stack.extend(i for i, has_edge in enumerate(edges[node]) if has_edge)

    return False


def generate_graph(filename: str, N: int, M: int) -> None:
    """
    Given the number of nodes and edges, generates a graph and writes it to a file.
    """
    with open(filename, "w") as file:
        # Genera vertices origen y destino, deben ser distintos
        C, A, B = random.sample(range(N), 3)

        # Escribe cabecera
        file.write(f"{N} {M} {C} {A} {B}\n")

        # Genera aristas
        edges: Edge_Matrix = generate_edges(N, M, C, A, B)
        times: Edge_Times = set_edge_times(edges, 1, 100)
        probs: Edge_Probabilities = set_probabilities(edges, C)

        # Print the edges. Do not repeat edges.
        for i in range(len(edges)):
            for j in range(i + 1, len(edges)):
                if edges[i][j]:
                    file.write(f"{i} {j} {times[i][j]} {probs[i][j]} {probs[j][i]}\n")


def generate_edges(N: int, M: int, C: int, A: int, B: int) -> Edge_Matrix:
    """
    Generates a random graph with N nodes and M edges. The graph must have a
    path from A to C and from B to C. All vertices in the graph must have at
    least one edge.
    """
    # Initialize the adjacency matrix with zeros
    matrix = [[False for _ in range(N)] for _ in range(N)]

    # Ensure all vertices have at least one edge by connecting them in pairs
    vertices = list(range(N))
    random.shuffle(vertices)
    for i in range(0, N, 2):
        j = (
            i + 1 if i + 1 < N else 0
        )  # Connect the last vertex to the first if N is odd
        matrix[vertices[i]][vertices[j]] = True
        matrix[vertices[j]][vertices[i]] = True

    # Count the number of edges we have added
    count = N // 2 + N % 2

    # Add the remaining edges
    while count < M:
        # Prioritize ensuring there is an indirect path from A to B
        if not any(matrix[A]) or not any(matrix[B]):
            if not any(matrix[A]):
                i = A
                j = random.choice([x for x in range(N) if x != A and x != B])
            else:  # not any(matrix[B])
                i = B
                j = random.choice([x for x in range(N) if x != A and x != B])
            matrix[i][j] = True
            matrix[j][i] = True
            count += 1
        else:
            i, j = random.sample(range(N), 2)  # Pick two distinct vertices

            # Ensure we don't add an edge where one already exists
            if not matrix[i][j]:
                matrix[i][j] = True
                matrix[j][i] = True
                count += 1

    return matrix


def set_edge_times(edges: Edge_Matrix, lower: int, upper: int) -> Edge_Times:
    """
    Given a edge 2D matrix, returns another array with the same edges but with
    set times for traversing the edge. The times are equal for both directions.
    """
    times: Edge_Times = [[0] * len(edges) for _ in range(len(edges))]

    for i in range(len(edges)):
        for j in range(len(edges)):
            if edges[i][j]:
                times[i][j] = random.randint(lower, upper)
                times[j][i] = times[i][j]

    return times


def set_probabilities(edges: Edge_Matrix, C: int) -> Edge_Probabilities:
    """
    Given a edge 2D matrix, returns another array with the same edges but with
    set probabilities for traversing the edge. The probabilities of leaving
    any node must sum 1. The probabilities may not be equal for both directions.
    The exception is the C node: the probabilities of leaving C must be zero.
    """
    probs: Edge_Probabilities = [[0] * len(edges) for _ in range(len(edges))]

    # Generates probabilities for each node. The probabilities for each row
    # must add up to 1.0
    for i in range(len(edges)):
        if i != C:
            # Get number of edges for this node
            num_edges = sum(edges[i])

            # There is only one exit
            if num_edges == 1:
                for j in range(len(edges)):
                    if edges[i][j]:
                        probs[i][j] = 1.0

            # Generate a list of random probabilities. The sum of the list must
            # be 1.0. The probabilities are rounded to 2 decimal places.

            probabilities = []

            while True:
                probabilities = []
                budget = 1.0

                for _ in range(num_edges - 1):
                    probabilities.append(round(random.uniform(0, budget * 0.35), 2))
                    budget -= probabilities[-1]

                probabilities.append(round(budget, 2))

                # Check if the sum is 1.0 and that no probabilities are  0
                if sum(probabilities) == 1.0 and 0.0 not in probabilities:
                    break

            # Shuffle the list
            random.shuffle(probabilities)

            # Assign the probabilities to the edges
            for j in range(len(edges)):
                if edges[i][j]:
                    probs[i][j] = probabilities.pop()

    return probs


if __name__ == "__main__":
    """
    Arguments:
        - N: Number of nodes. Must be between 3 and 900.
        - M: Number of edges. Must be between N / 2 and N * (N - 1) / 2.
    """

    if len(sys.argv) < 4:
        print(f"Usage: {sys.argv[0]} N M path/to/directory [samples]")
        exit(1)

    N = int(sys.argv[1])
    M = int(sys.argv[2])
    dir = sys.argv[3]
    samples = int(sys.argv[4]) if len(sys.argv) == 5 else 1

    # Check if directoy exists
    if not os.path.exists(dir):
        print(f"[ERROR]: El directorio {dir} no existe.")
        exit(1)

    if not (LOWER_BOUND_N <= N <= UPPER_BOUND_N):
        print(f"[ERROR]: N debe estar entre {LOWER_BOUND_N} y {UPPER_BOUND_N}.")
        exit(1)

    if not (N // 2 <= M <= N * (N - 1) // 2):
        print(f"[ERROR]: M debe estar entre {N // 2} y {N * (N - 1) // 2}.")
        exit(1)

    for i in range(samples):
        generate_graph(f"{dir}/test_{N}_vert_{M}_edge_{i+1}.txt", N, M)
        print(f"Generado test_{N}_vert_{M}_edge_{i+1}.txt")
