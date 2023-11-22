#!/bin/bash

make

for i in {2..20}; do
    echo -e "TABLEROS DE TAMAÑO ${i}\n\n"
    for j in {1..10}; do
        time ./practica2 "${i}" "test/tam/${i}/tablero_${i}_${j}" | grep -E "Memory used|CPU time"
        echo
    done
done

for i in 0.1 0.2 0.3 0.4 0.5 0.6 0.7 0.8 0.9; do
    echo -e "CASILLAS EN BLANCO = ${i}\n\n"
    for j in {1..10}; do
        time ./practica2 9 "test/prob/${i}/tablero_${i}_${j}" | grep -E "Memory used|CPU time"
        echo
    done
done
