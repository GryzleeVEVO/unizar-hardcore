#!/bin/sh

# Lanza un cliente interactivo y un Stateful Set de tres nodos Raft tolerantes
# a fallos

kubectl delete statefulset raft-ss
kubectl delete service raft-ss-service
kubectl delete pod cliente
echo "--------- Esperar un poco para dar tiempo que terminen Pods previos"
sleep 1
kubectl create -f k8s/raft-stateful.yaml
echo "--------- Esperar un poco para dar tiempo a arrancar réplicas"
sleep 5
kubectl exec cliente -ti -- cliente \
raft-ss-0.raft-ss-service.default.svc.cluster.local:29311 \
raft-ss-1.raft-ss-service.default.svc.cluster.local:29311 \
raft-ss-2.raft-ss-service.default.svc.cluster.local:29311