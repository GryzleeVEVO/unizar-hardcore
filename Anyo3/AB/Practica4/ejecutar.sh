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

# Ejecuta el programa y espera éxito
run_0() {
    ./${EXEC} $@ &>/dev/null </dev/null && bien || mal
}

# Ejecuta el programa y espera fracaso
run_1() {
    ./${EXEC} $@ &>/dev/null </dev/null && mal || bien
}

OUT=out.txt

run_cmp() {
    ./${EXEC} $1 $OUT

    cut -d ' ' -f 1 $OUT | cmp $2 && bien || mal
}

run_perf() {
    ./${EXEC} $1 $OUT

    local SUM=$(cut -d ' ' -f 2 $OUT| paste -sd+ | bc -l)
    local AVG=$(echo "$SUM/1000" | bc -l)

    echo "TOTAL = $SUM, MEDIA = $AVG"
}

EXEC=transporte

# Compila
[ $(uname) = "SunOS" ] && gmake || make

echo
echo "Test 1: Precondiciones"

echo -n "Faltan argumentos "
run_1

echo -n "Solo 1 argumento "
run_1 pruebas/prueba-pre1.txt

echo -n "Tren sin asientos "
run_1 pruebas/prueba-pre1.txt out.txt

echo -n "Tren con destino 0 "
run_1 pruebas/prueba-pre2.txt out.txt

echo -n "Tren con destino >7 "
run_1 pruebas/prueba-pre3.txt out.txt

echo -n "Tren sin pedidos "
run_1 pruebas/prueba-pre4.txt out.txt

echo -n "Tren con mas de 22 pedidos "
run_1 pruebas/prueba-pre5.txt out.txt

echo -n "Pedido con una llegada anterior a la salida "
run_1 pruebas/prueba-pre6.txt out.txt

echo -n "Pedido con una llegada igual a la salida "
run_1 pruebas/prueba-pre7.txt out.txt

echo -n "Pedido con una llegada posterior al fin de trayecto "
run_1 pruebas/prueba-pre8.txt out.txt

echo -n "Pedido con mas pasajeros que capacidad "
run_1 pruebas/prueba-pre9.txt out.txt

echo -n "Pedido sin pasajeros "
run_1 pruebas/prueba-pre10.txt out.txt

echo
echo -n "Test 2: Prueba ejemplo enunciado "
run_cmp pruebas/prueba_ejemplo.txt resultado/resultado_ejemplo.txt

echo
echo "Test 3: Pruebas propias "

echo -n "Prueba 0: "
run_cmp pruebas/prueba_0 resultado/resultado_0

echo -n "Prueba 1: "
run_cmp pruebas/prueba_1 resultado/resultado_1

echo -n "Prueba 2: "
run_cmp pruebas/prueba_2 resultado/resultado_2

echo -n "Prueba 3: "
run_cmp pruebas/prueba_3 resultado/resultado_3

echo -n "Prueba 4: "
run_cmp pruebas/prueba_4 resultado/resultado_4

echo
echo "Test 4: Pruebas de rendimiento"

echo -n "5 pedidos: " 
run_perf pruebas/prueba_5_pedidos

echo -n "10 pedidos: "
run_perf pruebas/prueba_10_pedidos

echo -n "15 pedidos: "
run_perf pruebas/prueba_15_pedidos

echo -n "20 pedidos: "
run_perf pruebas/prueba_20_pedidos

echo -n "2 paradas: " 
run_perf pruebas/prueba_2_paradas

echo -n "4 paradas: " 
run_perf pruebas/prueba_4_paradas

echo -n "6 paradas: " 
run_perf pruebas/prueba_6_paradas

echo -n "7 paradas: " 
run_perf pruebas/prueba_7_paradas

echo -n "10 pasajeros: " 
run_perf pruebas/prueba_10_pasajeros

echo -n "20 pasajeros: " 
run_perf pruebas/prueba_20_pasajeros

echo -n "30 pasajeros: " 
run_perf pruebas/prueba_30_pasajeros

echo -n "50 pasajeros: " 
run_perf pruebas/prueba_50_pasajeros

rm out.txt