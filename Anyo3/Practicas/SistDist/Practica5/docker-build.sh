#!/bin/sh

# Compila los binarios de cada ejecutable de Raft y genera imágenes Docker
# para cada ejecutable

cd raft

CGO_ENABLED=0 go build -o . ./...
mv srvraft ../dockerfiles/raft_nodo
mv client ../dockerfiles/raft_cliente

cd ../dockerfiles/raft_cliente
docker build . -t localhost:29310/cliente:latest
cd ../raft_nodo
docker build . -t localhost:29310/nodo:latest

docker push localhost:29310/cliente:latest
docker push localhost:29310/nodo:latest
