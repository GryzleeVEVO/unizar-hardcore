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
#	Descripción: Simple shell script que obtiene la salida (redirigida con &> 
#                   del cliente, la limpia y ordena para ser procesada por
#                   ./plot.sh

echo "$(sed -e '1i \# x y' -e 's/\(^.*: [ ]*\)\([0-9.]\+\)\(.*request[ ]*\)\([0-9]\+\)\(.*$\)/\4 \2/' $1 | sort -g)" > $1