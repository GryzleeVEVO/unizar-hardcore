#!/usr/bin/env python3

import sys
from typing import List

vert_files = [f"results_test_vert_{i}" for i in range(25, 225, 25)]
edge_files = [f"results_test_edge_{i}" for i in range(50, 300, 25)]


def generar_csv(archivos: List[str], res: str) -> None:
    with open(res, "w") as f:
        f.write("vertices;tiempo;errores\n")

        for archivo in archivos:
            with open(f"{ruta}/{archivo}") as f2:
                tiempo = 0.0
                error = 20
                for linea in f2:
                    tiempo += float(linea)
                    error -= 1

                f.write(f"{archivo.split('_')[-1]};{tiempo};{error}\n")


if __name__ == "__main__":
    if len(sys.argv) != 2:
        print(f"Uso: python3 {sys.argv[0]} <ruta>")
        sys.exit(1)

    ruta = sys.argv[1]

    generar_csv(vert_files, "results_vert.csv")
    generar_csv(edge_files, "results_edge.csv")
