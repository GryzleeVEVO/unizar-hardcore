#!/bin/bash

# BASES DE DATOS
# Práctica 1: Liga de fútbol
# Fichero: obtenerNombresCortos.bash
# Autores:  Álvaro Seral Gracia                 816456  816456@unizar.es
#           Cristian Andrei Selivanov Dobrisan  819425  819425@unizar.es
#           Dorian Boleslaw Wozniak             817570  817570@unizar.es
# Resumen: Script que obtiene en un archivo "nombresCortos" las columnas
#       local y visitante seguidas una de otra a partir de "LigaHost.txt"
#
# Entregado el 4 de abril de 2022

cat LigaHost.txt | cut -c 30-46 >> nombresCortos
cat LigaHost.txt | cut -c 47-63 >> nombresCortos
./obtenerNombresCortos
