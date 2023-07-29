#!/bin/bash

# 817570, Wozniak, Dorian Boleslaw, M, 3, A
# 820532, Pérez Guillén, Marcos, M, 3, A

# El script obtiene el nombre de un directorio y muestra el número de ficheros
# y directorios contenidos

# Obtiene el nombre del directorio
echo -n "Introduzca el nombre de un directorio: "
read dir

# Si es un directorio válido
if [ -d "$dir" ]
then
    # ls -p diferencia ficheros de directorios concatenando una / al final de los segundos
    n_files=$(ls "$dir" -p | grep -v / | wc -l) # Obtiene nombres sin /
    n_dirs=$(ls "$dir" -p | grep / | wc -l)     # Obtiene nombres con /
    echo "El numero de ficheros y directorios en $dir es de $n_files y $n_dirs, respectivamente"
else
    echo "$dir no es un directorio"
fi