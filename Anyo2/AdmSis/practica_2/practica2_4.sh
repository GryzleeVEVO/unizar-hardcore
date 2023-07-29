#!/bin/bash

# 817570, Wozniak, Dorian Boleslaw, M, 3, A
# 820532, Pérez Guillén, Marcos, M, 3, A

# El script dice el tipo del primer carácter introducido por el usuario

# Obtiene una línea
echo -n "Introduzca una tecla: "
read line
# Obtiene de la línea el primer carácter
char=$(echo "$line" | cut -c 1)

# Según el tipo de carácter...
case "$char" in
    # Es letra
    [a-zA-Z] )
        echo "$char es una letra"
        ;;
    # Es número
    [0-9] )
        echo "$char es un numero"
        ;;
    # Es otro carácter (especial)
    * )
        echo "$char es un caracter especial"
        ;;
esac
