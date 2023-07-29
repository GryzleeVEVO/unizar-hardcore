package cltraft

import (
	"raft/internal/comun/rpctimeout"
	"raft/internal/raft"
	"time"
)

// Envía una RPC SometerEstadoRaft() al nodo dado
func SometerOperacionRaft(nodos []rpctimeout.HostPort, tipo, clave,
	valor string) (int, int, bool, int, string, error) {

	nodo := -1
	for i := range nodos {
		_, _, _, idLider, _, err := ObtenerEstadoNodo(nodos, i)
		if err == nil {
			nodo = idLider
			break
		}
	}

	args := raft.TipoOperacion{Operacion: tipo, Clave: clave, Valor: valor}
	reply := &raft.ResultadoRemoto{}

	err := nodos[nodo].CallTimeout("NodoRaft.SometerOperacionRaft", args, reply,
		2000*time.Millisecond)

	return reply.IndiceRegistro, reply.Mandato, reply.EsLider, reply.IdLider,
		reply.ValorADevolver, err
}

// Envía una RPC ObtenerEstado() al nodo dado
func ObtenerEstadoNodo(nodos []rpctimeout.HostPort, nodo int) (int, int, bool,
	int, []raft.AplicaOperacion, error) {
	args := raft.Vacio{}
	reply := raft.EstadoRemoto{}

	err := nodos[nodo].CallTimeout("NodoRaft.ObtenerEstadoNodo", args, &reply,
		2000*time.Millisecond)

	return reply.IdNodo, reply.Mandato, reply.EsLider, reply.IdLider, reply.Logs, err
}

// Envía una RPC ParaNodo() al nodo dado
func ParaNodo(nodos []rpctimeout.HostPort, nodo int) error {
	return nodos[nodo].CallTimeout("NodoRaft.ParaNodo",
		raft.Vacio{}, &raft.Vacio{}, 2000*time.Millisecond)
}
