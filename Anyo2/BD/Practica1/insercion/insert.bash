#!/bin/bash

# BASES DE DATOS
# Práctica 1: Liga de fútbol
# Fichero: insert.bash
# Autores:  Álvaro Seral Gracia                 816456  816456@unizar.es
#           Cristian Andrei Selivanov Dobrisan  819425  819425@unizar.es
#           Dorian Boleslaw Wozniak             817570  817570@unizar.es
# Resumen: Script de inserción de tuplas en la base de datos "Liga de fútbol"
#
# Entregado el 4 de abril de 2022

sqlldr2 parfile="./ligas/insert_liga.par"
sqlldr2 parfile="./temporadas/insert_temporada.par"
sqlldr2 parfile="./jornadas/insert_jornada.par"
sqlldr2 parfile="./estadios/insert_estadio.par"
sqlldr2 parfile="./equipos/insert_equipo.par"
sqlldr2 parfile="./apodos/insert_apodo.par"
sqlldr2 parfile="./partidos/insert_partido.par"
sqlldr2 parfile="./compiten/insert_compiten.par"
