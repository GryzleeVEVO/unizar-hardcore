#!/bin/bash

./bin/lector ./ms/users.txt 1 &
./bin/lector ./ms/users.txt 2 &
./bin/escritor ./ms/users.txt 3 &
./bin/lector ./ms/users.txt 4 &
./bin/lector ./ms/users.txt 5 &
./bin/escritor ./ms/users.txt 6