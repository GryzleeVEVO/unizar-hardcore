#!/bin/bash
#817570, Wozniak, Dorian, M, 3, A
#820532, Pérez Guillén, Marcos, M, 3, A

if [ $# -eq 3 ]
then
    while read -r addr
    do
        if ssh -q -o ConnectTimeout=1 "as@$addr" `exit` < /dev/null > /dev/null
        then
            scp -q "./practica_3.sh" "$2" "as@$addr:~"
            ssh -q "as@$addr" "sudo ~/./practica_3.sh "$1" "$2" ; rm ~/practica_3.sh ~/$2" < /dev/null
            
        else
            echo "$addr no es accesible" >& 2
            exit 1
        fi
    done < "$3"
else
    echo "Numero incorrecto de parametros" >& 2
    exit 1
fi
