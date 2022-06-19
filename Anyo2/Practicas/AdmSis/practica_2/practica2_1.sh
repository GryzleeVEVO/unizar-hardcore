#!/bin/bash

# 817570, Wozniak, Dorian Boleslaw, M, 3, A
# 820532, Pérez Guillén, Marcos, M, 3, A

# El script obtiene un nombre de fichero y, si es válido, muestra sus permisos
# de usuario

# Obtiene nombre del fichero
echo -n "Introduzca el nombre del fichero: "
read fich

# Si el fichero existe...
if [ -f "$fich" ]
then
    echo -n "Los permisos del archivo $fich son: "

    # Prueba si es legible
    if [ -r "$fich" ]
    then
        echo -n "r"
    else
        echo -n "-"
    fi

    # Prueba si es escribible
    if [ -w "$fich" ]
    then
        echo -n "w"
    else
        echo -n "-"
    fi

    # Prueba si es ejecutable
    if [ -x "$fich" ]
    then
        echo -n "x"
    else
        echo -n "-"
    fi

    echo
# Si no, avisa
else
    echo -n "$fich no existe"
fi