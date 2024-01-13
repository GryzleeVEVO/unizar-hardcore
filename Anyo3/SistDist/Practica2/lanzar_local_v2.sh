#!/bin/bash

./bin/escritor ./ms/users.txt 1 &
./bin/escritor ./ms/users.txt 2 &
./bin/escritor ./ms/users.txt 3 &
./bin/escritor ./ms/users.txt 4 &
./bin/lector ./ms/users.txt 5 &
./bin/lector ./ms/users.txt 6 &
./bin/lector ./ms/users.txt 7 &
./bin/lector ./ms/users.txt 8 &
./bin/lector ./ms/users.txt 9 &
./bin/lector ./ms/users.txt 10