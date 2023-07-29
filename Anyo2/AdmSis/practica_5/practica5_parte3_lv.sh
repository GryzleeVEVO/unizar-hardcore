#!/bin/bash
#817570, Wozniak, Dorian, M, 3, A
#820532, Pérez Guillén, Marcos, M, 3, A

# El script requiere privilegios de administrador
if [ $EUID -ne 0 ]
then
	echo "Se requieren privilegios de administración" >& 2 ; exit 1
else
	# Lee de entrada estandar
	while IFS=",", read VGname LVname Size FStype Mount
	do	
		# Primero comprueba que los parámetros esten correctamente escritos

		# Comprueba que el grupo volumen exista
		if [ -z "$(vgs | grep "$VGname" )" ]
		then
			echo "No se ha encontrado el grupo volumen $VGname"

		# Comprueba que el formato del tamaño sea correcto
		elif [ -z "$(echo "$Size" | grep '[0-9]\+\(B\|K\|M\|G\)')" ]
		then
			echo "Tamaño introducido incorrectamente. Formato: [0-9]+(B|K|M|G)"
		
		# Comprueba que el sistema de ficheros sea correcto
		elif [ -z "$(cat /proc/filesystems | grep "$FStype")" ]
		then
			echo "$FStype no es un sistema de ficheros soportado"

		# Comprueba que el punto de montaje exista
		elif [ ! -d "$Mount" ]
		then
			echo "El directorio $Mount no existe o no es accesible"
		
		# Los parámetros son correctos
		else
			# Si no encuentra el volumen lógico
			if [ -z "$(lvs | grep "$LVname")" ]
			then
				# Lo crea
				lvcreate -n $LVname -L $Size $VGname -y
				# Le da formato
				mkfs.$FStype /dev/$VGname/$LVname
				# Lo añade a fstab
				echo "UUID=$(blkid /dev/$VGname/$LVname | cut -d ' ' -f 2 | cut -d '"' -f 2) $Mount $FStype defaults 0 0" >> /etc/fstab
				# Lo monta
				mount -t $FStype /dev/$VGname/$LVname $Mount

			# Si lo encuentra, lo reescala
			else
				umount /dev/$VGname/$LVname
				lvextend -L +$Size /dev/$VGname/$LVname
				# Nota: resize2fs solo funciona con FS extX
				resize2fs /dev/$VGname/$LVname
				mount -t $FStype /dev/$VGname/$LVname $Mount
			fi
		fi
	done
fi


