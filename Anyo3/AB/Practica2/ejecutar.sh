#!/bin/bash

ROJO='\033[0;31m'
VERDE='\033[0;32m'
NORMAL='\033[0m'

bien() {
    echo -e "[${VERDE}Superado${NORMAL}]"
}

mal() {
    echo -e "[${ROJO}Fallido${NORMAL}]"
}

# Compila, en caso de usar hendrix01 o 02 usa gmake
[ $(uname) = "SunOS" ] && gmake || make


echo -n "Test 1: Precondiciones "
./pinball test/test1 output
cmp result/result1 output < /dev/null &> /dev/null && bien || mal

rm output

echo -n "Test 2: Pruebas con árboles P=1..5 y pruebas indivuduales con árboles más grandes "
./pinball test/test2 output
sed '/^Tiempo/d' output > output2 # Los tiempos de ejecución varían y darian el test por inválido
cmp result/result2 output2 < /dev/null &> /dev/null && bien || mal

rm output output2

echo "Test 3: Números grandes y benchmark "
./pinball test/test3 output 
egrep "^Profundidad|^Tiempo|^$" output

rm output