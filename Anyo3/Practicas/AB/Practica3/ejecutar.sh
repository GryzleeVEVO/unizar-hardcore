#!/bin/bash

ROJO='\033[0;31m'
VERDE='\033[0;32m'
NORMAL='\033[0m'

# Muestra un mensaje de éxito
bien() {
    echo -e "[${VERDE}Superado${NORMAL}]"
}

# Muestra un mensaje de fracaso
mal() {
    echo -e "[${ROJO}Fallido${NORMAL}]"
}

EXEC=partePalabras

# Ejecuta el programa y espera éxito
run_0() {
    ./${EXEC} $@ </dev/null &>/dev/null && bien || mal
}

# Ejecuta el programa y espera fracaso
run_1() {
    ./${EXEC} $@ </dev/null &>/dev/null && mal || bien
}

# Compara la salida del programa con un resultado esperado
run_cmp_1() {
    TEST=$1
    shift
    ./${EXEC} $@ |
        egrep -v "Se ha cargado el diccionario|Se ha solucionado el problema|^$" 2>/dev/null |
        cmp result/$TEST && bien || mal
}

# Prueba si la salida es vacia
run_cmp_0() {
    [ $(./${EXEC} $@ </dev/null &>/dev/null) ] && mal || bien
}

# Obtiene el tiempo de ejecución del programa
run_time() {
    ./${EXEC} $1 $(shuf -n $2 $1 | tr -d "\n\t ") |
        egrep "Se ha solucionado el problema" 2>/dev/null
}

# Compila, en caso de usar hendrix01 o 02 usa gmake
[ $(uname) = "SunOS" ] && gmake || make

echo
echo "Test 1: Precondiciones "
echo -n "Sin argumentos: "
run_1
echo -n "Solo diccionario: "
run_1 diccionarios/diccionario_ejemplo.txta megustasola

DIC="diccionarios/diccionario_ejemplo.txt"

echo
echo "Test 2: Diccionario ejemplo enunciado (con un par de términos extra)"

echo -n "meguste "
run_cmp_0 result0 $DIC meguste
echo -n "me "
run_cmp_1 result1 $DIC me
echo -n "megusta "
run_cmp_1 result2 $DIC megusta
echo -n "megustasol "
run_cmp_1 result3 $DIC megustasol
echo -n "megustadar "
run_cmp_1 result4 $DIC megustadar
echo -n "megustasoldar "
run_cmp_1 result5 $DIC megustasoldar
echo -n "megustasoldarlo "
run_cmp_1 result6 $DIC megustasoldarlo
echo -n "mesoldarlogusta "
run_cmp_1 result7 $DIC mesoldarlogusta
echo -n "soldarlomegusta "
run_cmp_1 result8 $DIC soldarlomegusta
echo -n "soldarlosoldarlosoldarmegustameme"
run_cmp_1 result9 $DIC soldarlosoldarlosoldarmegustameme

echo
echo "Test 3: Rendimiento, diccionarios grandes y frases grandes"

# Diccionarios 1000/5000/10000 términos más frecuentes obtenidos en
#https://corpus.rae.es/frec/1000_formas.TXT
#https://corpus.rae.es/frec/5000_formas.TXT
#https://corpus.rae.es/frec/10000_formas.TXT

echo
echo "Diccionario 100 palabras mas frecuentes"
run_time diccionarios/diccionario100 5
run_time diccionarios/diccionario100 10
run_time diccionarios/diccionario100 15
run_time diccionarios/diccionario100 30
run_time diccionarios/diccionario100 50

echo
echo "Diccionario 1000 palabras mas frecuentes"
run_time diccionarios/diccionario1000 5
run_time diccionarios/diccionario1000 10
run_time diccionarios/diccionario1000 15

echo
echo "Diccionario 5000 palabras mas frecuentes"
run_time diccionarios/diccionario5000 5
#run_time diccionarios/diccionario5000 10
#run_time diccionarios/diccionario5000 15

echo
echo "Diccionario 10000 palabras mas frecuentes"
run_time diccionarios/diccionario10000 5
#run_time diccionarios/diccionario10000 10
#run_time diccionarios/diccionario10000 15

#echo
#echo "Diccionario +600000 términos"
#run_time diccionarios/0_palabras_todas.txt 5
#run_time diccionarios/0_palabras_todas.txt 10
#run_time diccionarios/0_palabras_todas.txt 15
