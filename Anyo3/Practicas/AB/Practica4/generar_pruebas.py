#!/bin/python
#
# Dorian Boleslaw Wozniak (817570@unizar.es)
# Jesus Mendoza Aranda (777524@unizar.es)
#
# Script que genera una serie de ficheros de pruebas de problemas para
# la práctica 4 

import random

random.seed()

MIN_PASAJEROS = 1
MAX_PASAJEROS = 50

PARADA_INICIAL = 0
PARADA_FINAL = 7

MIN_PEDIDOS = 1
MAX_PEDIDOS = 22

MIN_VIAJES = 1
MAX_VIAJES = 100

NUM_FICHEROS = 1

# Devuelve un nuevo tren
# El tren es representado por una 3-tupla
def crearTren():
    capacidad = random.randint(MIN_PASAJEROS, MAX_PASAJEROS)
    #capacidad = MAX_PASAJEROS
    final = random.randint(PARADA_INICIAL + 1, PARADA_FINAL)
    #final = PARADA_FINAL
    pedidos = random.randint(MIN_PEDIDOS, MAX_PEDIDOS)
    #pedidos = MAX_PEDIDOS
    return (capacidad, final, pedidos)


# Devuelve una lista de pedidos
# Un pedido es una lista de 3-tuplas de tantos pedidos como especifique el tren
def crearPedidos(tren):
    pedidos = []

    # Genera tantos pedidos como diga el tren
    for i in range(tren[2]):
        # La salida va de 0 a una estacion anterior a la final
        salida = random.randint(PARADA_INICIAL, tren[1] - 1)

        # La llegada debe ser posterior a la salida y no posterior al final
        llegada = random.randint(salida + 1, tren[1])

        pasajeros = random.randint(MIN_PASAJEROS, tren[0])

        # Añade los pedidos a la lista
        pedidos.append((salida, llegada, pasajeros))

    return pedidos


# Genera múltiples ficheros ficheros
for i in range(NUM_FICHEROS):
    file = open("prueba_" + str(i), "w")

    # Crea un fichero con hasta 10 pruebas
    for j in range(random.randint(MIN_VIAJES, MAX_VIAJES)):
    #for j in range(MAX_VIAJES):
        # Genera un tren y una serie de pedidos
        tren = crearTren()
        pedidos = crearPedidos(tren)
    
        #print(" ".join(map(str, tren)))

        # Los escribe en el fichero
        file.write(" ".join(map(str, tren)) + "\n")

        for k in pedidos:
            file.write(" ".join(map(str, k)) + "\n")

    # Termina el fichero y lo cierra
    file.write("0 0 0")

    file.close()
