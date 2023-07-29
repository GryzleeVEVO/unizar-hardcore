#!/bin/bash
#817570, Wozniak, Dorian, M, 3, A
#820532, Pérez Guillén, Marcos, M, 3, A

# El script requiere permisos de administrador
if [ $EUID -ne 0 ]
then
	echo "Se requieren permisos de administrador" >& 2 ; exit 1 
# El script requiere al menos dos parámetros: el VG a extender y las particiones a usar
elif [ $# -le 1 ]
then 
	echo "Parametros insuficientes" >& 2 ; exit 1
# Comprueba que exista el grupo de volumenes especificado
elif [ -z "$(vgs | grep "$1")" ]  
then
	echo "No existe el grupo de volumenes" >& 2 ; exit 1
else
	# Almacena el VG para que no se pierda
	volume="$1"
	shift
	# Itera sobre el resto de parámetros
	while [ $# -gt 0 ]
	do	
		# Trata de encontrar la partición
		if [ -z "$(sfdisk -l | grep "$1")" ]
		then
			echo "No existe la particion $1" >& 2 ; exit 1
		else
			# Comprueba si esta montada. Si es así, la desmonta
			if [ -n "$(df | grep "$1")" ] 
			then
				umount $1
			fi
			# Extiende
			vgextend "$volume" $1
		fi
		# Siguiente parámetro
		shift
	done
fi


