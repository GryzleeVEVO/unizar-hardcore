#!/bin/bash
#817570, Wozniak, Dorian, M, 3, A
#820532, Pérez Guillén, Marcos, M, 3, A

# Comprueba que el ejecutor tiene permisos de administrador
if [ "$EUID" -ne 0 ]
then
    echo "Este script necesita privilegios de administracion" >& 2
    exit 1

# Comprueba que tiene el número correcto de parámetros
elif [ $# -eq 2 ]
then
    case "$1" in

        # Creación
        -a )
            # Abre y procesa el fichero indicado en el 2o parametro
            while IFS=",", read -r id pw fullname
            do
                # Alguno de los campos es inválido (vacío o más de 3 parámetros)
                if [ -z "$id" -o -z "$pw" -o -z "$fullname" \
                        -o $(echo "$fullname" | tr -cd "," | wc -c) -ne 0 ]
                then
                    echo "Campo invalido" >& 2
                    exit 1
                
                # Encuentra en el sistema un usuario con el mismo nombre
                elif getent passwd "$id" > /dev/null
                then
                    echo "El usuario $id ya existe"

                # El usuario se puede añadir
                else
                    # El usuario creado:
                    #   - Tiene UID > 1815  
                    #   - Contraseña caduca en 30 dias
                    #   - Grupo con su mismo nombre por defecto
                    #   - Se crea un directorio home copiando de /etc/skel

                    useradd -K UID_MIN=1815 -K PASS_MAX_DAYS=30 -m -k /etc/skel -c "$fullname" -U "$id"
                    echo "$id:$pw" | chpasswd
                    usermod -U "$id"
                    echo "$fullname ha sido creado"
                fi
            done < "$2"  
        ;;

        # Borrado
        -s )
	    # Crea /extra/backup si no existe 
	    if [ ! -d "/extra" ]
	    then
            mkdir /extra/
	    fi

	    if [ ! -d "/extra/backup" ]
	    then
	        mkdir /extra/backup/
    	    fi
            # Lee solo el primer campo del fichero obtenido como parámetro
            while IFS=",", read -r id elresto
            do 

                # Prueba que el usuario no exista o sea root
                if [ -n "$(getent passwd "$id")" -a "$id" != "root" ]
                then

                    # Trata de hacer una copia del directorio
                    suhome=$(getent passwd "$id" | cut -d: -f6)
                    if tar cfP "/extra/backup/$id.tar" "$suhome"
                    then

                        # Borra el usuario y su directorio home  
                        userdel "$id"
                        rm -r "$suhome"
                    fi
                fi
            done < "$2"
        ;;

        # La opción es incorrecta
        *)  echo "Opcion invalida" >& 2
            exit 1
        ;;
    esac

else
    echo "Numero incorrecto de parametros" >& 2
    exit 1
fi
