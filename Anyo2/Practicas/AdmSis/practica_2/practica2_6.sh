#!/bin/bash

# 817570, Wozniak, Dorian Boleslaw, M, 3, A
# 820532, Pérez Guillén, Marcos, M, 3, A

# El script copia los ficheros del directorio actual en un directorio temporal
# en el directorio HOME del usuario. Si no hay directorios con el formato binXXX
# crea uno; en caso contrario, elige la directorio con la fecha de actualización
# más antigua

# Si hay algún directorio con el formato binXXX ...
if [ $(ls -p "$HOME" | grep "bin[a-zA-Z0-9][a-zA-Z0-9][a-zA-Z0-9]/" | wc -l) -ne 0 ]
then
    # Obtiene el directorio binXXX más antiguo según su actualización
    # -t lista directorios según lo recien modificados que son, -r revierte el orden
    # head deja sólo el primer resultado, cut elimina el / final
    bin="$HOME/$(ls -rtp $HOME | grep "bin[a-zA-Z0-9][a-zA-Z0-9][a-zA-Z0-9]/" | head -1 | cut -c -6)"
# Si no, crea un nuevo directorio temporal
else
    bin=$(mktemp -d "$HOME/binXXX")
    echo "Se ha creado el directorio $bin"
fi

echo "Directorio destino de copia: $bin"

# Copia los arcivos del directorio actual
count=0
for i in *
do
    # Si el archivo existe y no es un directorio...
    if [ -f "$i" -a ! -d "$i" ]
    then
        cp "$i" "$bin"
        echo "./$i ha sido copiado a $bin"
        count=$((count+1))
    fi
done

# Mensaje según número de archivos copiados
if [ $count -eq 0 ]
then
    echo "No se ha copiado ningun archivo"
else
    echo "Se han copiado $count archivos"
fi