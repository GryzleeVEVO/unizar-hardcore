#!/bin/bash

# 817570, Wozniak, Dorian Boleslaw, M, 3, A
# 820532, Pérez Guillén, Marcos, M, 3, A

# El script da permisos de usuario y grupo, y muestra los permisos de un fichero
# introducido como parámetro 

# Solo funciona si hay un solo parámetro
if [ $# -eq 1 ]
then
    # Prueba si es un fichero común
    if [ -f "$1" ]
    then
        # Otorga primero permisos de ejecución al usuario y grupo, luego muestra sus permisos 
        chmod ug+x "$1"
        stat --format=%A "$1"
    # Si no existe, avisa
    else
        echo "$1 no existe"
    fi
# En caso contrario, muestra la sintáxis correcta
else
    echo "Sintaxis: practica2_3.sh <nombre_archivo>"
fi