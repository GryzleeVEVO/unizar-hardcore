#!/bin/sh

# Lanza un cliente interactivo y tres réplicas Raft no tolerantes a fallos

kubectl delete pod cliente
kubectl delete pod nodo-0
kubectl delete pod nodo-1
kubectl delete pod nodo-2
kubectl delete service raft-fallos
echo "--------- Esperar un poco para dar tiempo que terminen Pods previos"
sleep 1
kubectl create -f k8s/raft-cliente-fallos.yaml
echo "--------- Esperar un poco para dar tiempo a arrancar réplicas"
sleep 5
kubectl exec cliente -ti -- cliente \
nodo-0.raft-ss-service.default.svc.cluster.local:29311 \
nodo-1.raft-ss-service.default.svc.cluster.local:29311 \
nodo-2.raft-ss-service.default.svc.cluster.local:29311