#!/bin/bash

#	Sistemas Distribuidos
#	Practica 1
#
#	Fichero: cmd/server-pool/main.go
#
#	Autores:
#		Dorian Boleslaw Wozniak (817570@unizar.es)
#		Cristian Andrei Selivanov Dobrisan (816456@unizar.es)
#
#	Descripción: Shell script que lanza una serie de procesos workers en las
#                   máquinas remotas definidas en el fichero "workers" mediante
#                   ssh. Asume que las máquinas son alcanzables.

while read endpoint || [ -n "$endpoint" ]
do
    ssh -n $(echo $endpoint | cut -d ':' -f 1) "$HOME/practica1/bin/./server-worker $endpoint" &
done < "$HOME/practica1/workers"
