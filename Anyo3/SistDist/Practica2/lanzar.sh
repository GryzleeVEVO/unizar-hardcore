#!/bin/bash

# Sistemas Distribuidos
# Práctica 2
#
# Autores:
# Cristian Andrei Selivanov Dobrisan (816456@unizar.es)
# Dorian Boleslaw Wozniak (817570@unizar.es)
#
# Descripción:
#
# Shell script que lanza una serie de seis procesos: 4 lectores y 2 escritores
# El script lanza el script via ssh asumiendo que el binario se encuentra en
# la misma ruta desde la cual se ha lanzado el script

users="$PWD/ms/users.txt"
i=1

while read endpoint || [ -n "$endpoint" ]
do
    if [ $i -ne 2 -a $i -ne 4 -a $i -ne 6 -a $i -ne 8 -a $i -ne 10 ]
    then
        ssh -n $(echo $endpoint | cut -d ':' -f 1) "$PWD/bin/./lector $users $i" &
    else
        ssh -n $(echo $endpoint | cut -d ':' -f 1) "$PWD/bin/./escritor $users $i" &
    fi
    i=$(( $i+1 ))
done < $users
