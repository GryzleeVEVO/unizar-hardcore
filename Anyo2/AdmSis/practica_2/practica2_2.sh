#!/bin/bash

# 817570, Wozniak, Dorian Boleslaw, M, 3, A
# 820532, Pérez Guillén, Marcos, M, 3, A

# El script obtiene una secuencia de ficheros de la línea de parámetros y los
# que son válidos muestra sus contenidos y los que no avisa de que no lo es

# Para todos los parámetros, separados por espacios (salvo si estan entrecomillados)
for i in "$@"
do
    # Si es un fichero común, lo pagina con more
    if [ -f "$i" ]
    then
        more "$i"
    # Si no, avisa que no es un fichero
    else
        echo "$i no es un fichero"
    fi
done