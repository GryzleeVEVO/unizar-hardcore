#!/bin/bash

for i in test/vert/*; do
    : >"results_test_vert_$(basename "${i}")"

    for j in "${i}"/*; do
        output=$(./practica3 "${j}")
        echo "${output}" | grep -v "\[AVISO\]" | grep "Tiempo medio de ejecución" | cut -d " " -f 7
        echo "${output}" | grep -v "\[AVISO\]" | grep "Tiempo medio de ejecución" | cut -d " " -f 7 >>"results_test_vert_$(basename "${i}")"
    done
done

for i in test/edge/*; do
    : >"results_test_edge_$(basename "${i}")"

    for j in "${i}"/*; do
        output=$(./practica3 "${j}")
        echo "${output}" | grep -v "\[AVISO\]" | grep "Tiempo medio de ejecución" | cut -d " " -f 7
        echo "${output}" | grep -v "\[AVISO\]" | grep "Tiempo medio de ejecución" | cut -d " " -f 7 >>"results_test_edge_$(basename "${i}")"
    done
done
