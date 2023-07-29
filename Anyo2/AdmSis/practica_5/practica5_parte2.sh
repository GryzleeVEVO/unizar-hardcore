#!/bin/bash
#817570, Wozniak, Dorian, M, 3, A
#820532, Pérez Guillén, Marcos, M, 3, A

if [ $# -eq 1 ]
then
	if ssh -n "as@$1" "exit" &> /dev/null
	then
		echo 
		# Discos duros disponibles y tamaño
		echo "Discos disponibles"
		ssh -n "as@$1" "sudo sfdisk -s | grep "^/dev""
		# Particiones y tamaño de estas
		echo ""; echo "Particiones definidas"
		ssh -n "as@$1" "sudo sfdisk -l | grep "^/dev" | tr -d '*' | tr -s ' ' | cut -d ' ' -f 1,5"
		# Volumenes montados, sistema, directorio, tamaño, espacio libre
		echo ""; echo "Puntos de montaje"
		ssh -n "as@$1" "sudo df -hT | grep "^/dev" | grep -v "tmpfs" | tr -s ' ' | cut -d ' ' -f 1,2,3,5"
	else
		echo "$1 inaccesible" >& 2
		exit 1
	fi
else
	echo "Parámetros insuficientes"
	exit 1
fi	
