package raft

import (
	"log"
	"math/rand"
	"net"
	"net/rpc"
	"raft/internal/comun/check"
	"raft/internal/comun/rpctimeout"
	"sync"
	"time"
)

const (
	kEnableDebugLogs = true

	kLogToStdout = false

	kLogOutputDir = "./log_raft/"

	SEGUIDOR  = 1
	LIDER     = 2
	CANDIDATO = 3

	RAFT_NIL = -1

	RAFT_RPC_TIMEOUT = 20
	RAFT_HB_TIMEOUT  = 100
	RAFT_MIN_TIMEOUT = 150
	RAFT_MAX_TIMEOUT = 300
)

/* ---------------------------------------------------------------------------*/
/* --- ESTADO --- */

type NodoRaft struct {
	nodos    []rpctimeout.HostPort
	numNodos int

	yo      int
	estado  int
	mandato int

	idLider int
	idVoto  int
	votos   int

	reiniciarTimeout  chan bool
	operacionSometida chan bool

	mux    sync.Mutex
	logger *log.Logger
}

/* ---------------------------------------------------------------------------*/
/* --- ALGORTIMO --- */

func (nr *NodoRaft) recepcion() {

	err := rpc.Register(nr)
	check.CheckError(err, "recepcion()")

	l, err := net.Listen("tcp", string(nr.nodos[nr.yo-1]))
	check.CheckError(err, "recepcion()")

	rpc.Accept(l)
}

func (nr *NodoRaft) start() {
	for {
		nr.mux.Lock()
		switch nr.estado {
		case LIDER:
			nr.mux.Unlock()
			timer := nuevoTimer(RAFT_HB_TIMEOUT)
			select {
			case <-nr.operacionSometida:
				timer.Stop()
			case <-timer.C:
				for i, nodo := range nr.nodos {
					if i != nr.yo-1 {
						nr.enviarAppendEntries(nodo)
					}
				}
			}
		default:
			nr.mux.Unlock()
			timer := nuevoTimerAleatorio(RAFT_MIN_TIMEOUT, RAFT_MAX_TIMEOUT)
			select {
			case <-nr.reiniciarTimeout:
				timer.Stop()
			case <-timer.C:
				nr.iniciarElecciones()
			}
		}
	}
}

func (nr *NodoRaft) iniciarElecciones() {
	nr.mux.Lock()
	nr.mandato++
	nr.estado = CANDIDATO
	nr.idVoto = nr.yo
	nr.votos = 1
	nr.mux.Unlock()

	for i, nodo := range nr.nodos {
		if i != nr.yo-1 {
			nr.enviarPedirVoto(nodo)
		}
	}
}

/* ---------------------------------------------------------------------------*/
/* --- RPC PeticionVoto() --- */

type ArgsPeticionVoto struct {
	Mandato     int
	IdCandidato int
}

type RespuestaPeticionVoto struct {
	Mandato      int
	VotoObtenido bool
}

func (nr *NodoRaft) PedirVoto(args *ArgsPeticionVoto,
	reply *RespuestaPeticionVoto) error {
	nr.mux.Lock()
	if args.Mandato > nr.mandato {
		nr.mandato = args.Mandato
		nr.estado = SEGUIDOR
		nr.idVoto = -1
	}
	reply.Mandato = nr.mandato
	if (args.Mandato == nr.mandato) &&
		(nr.idVoto == -1 || nr.idVoto == args.IdCandidato) {
		nr.idVoto = args.IdCandidato
		reply.VotoObtenido = true
	} else {
		reply.VotoObtenido = false
	}
	nr.mux.Unlock()
	return nil
}

func (nr *NodoRaft) enviarPedirVoto(nodo rpctimeout.HostPort) {

	nr.mux.Lock()
	args := ArgsPeticionVoto{Mandato: nr.mandato, IdCandidato: nr.yo}
	reply := RespuestaPeticionVoto{}
	nr.mux.Unlock()

	err := nodo.CallTimeout("NodoRaft.PedirVoto", args, &reply, 0)

	// Trata respuesta si recibe una
	if err == nil {
		nr.mux.Lock()

		if (nr.estado == CANDIDATO) && (reply.Mandato == nr.mandato) &&
			(reply.VotoObtenido) {
			nr.votos++
			if nr.votos > nr.numNodos/2+1 {
				nr.estado = LIDER
				nr.idLider = nr.yo
				nr.mux.Unlock()
				nr.reiniciarTimeout <- true
				for i, nodo := range nr.nodos {
					if i != nr.yo-1 {
						nr.enviarAppendEntries(nodo)
					}
				}
			}

		} else if reply.Mandato > nr.mandato {
			nr.estado = SEGUIDOR
			nr.idVoto = -1
			nr.mux.Unlock()
			nr.reiniciarTimeout <- true
		} else {
			nr.mux.Unlock()
		}
	}
}

/* ---------------------------------------------------------------------------*/
/* --- RPC AppendEntries() --- */

type ArgAppendEntries struct {
	Mandato int
	IdLider int
}

type Results struct {
	Mandato int
	Exito   bool
}

func (nr *NodoRaft) AppendEntries(args *ArgAppendEntries,
	reply *Results) error {

	nr.mux.Lock()
	if args.Mandato > nr.mandato {
		nr.mandato = args.Mandato
		nr.idVoto = -1
		nr.mux.Unlock()
		nr.reiniciarTimeout <- true
		nr.mux.Lock()
	}

	if args.Mandato == nr.mandato {

	}

	return nil
}

func (nr *NodoRaft) enviarAppendEntries(nodo rpctimeout.HostPort) {

	nr.mux.Lock()
	args := ArgAppendEntries{Mandato: nr.mandato, IdLider: nr.yo}
	reply := Results{}
	nr.mux.Unlock()

	err := nodo.CallTimeout("NodoRaft.AppendEntries", args, &reply, 0)

	// Trata respuesta si recibe una
	if err == nil {
		nr.mux.Lock()
		if reply.Mandato == nr.mandato && nr.estado == LIDER {
			nr.mux.Unlock()
		} else if reply.Mandato > nr.mandato {
			nr.mandato = reply.Mandato
			for i, nodo := range nr.nodos {
				if i != nr.yo-1 {
					nr.enviarAppendEntries(nodo)
				}
			}
		} else {
			nr.mux.Unlock()
		}
	}
}

/* ---------------------------------------------------------------------------*/
/* --- FUNCIONES AUXILIARES --- */

func nuevoTimer(x int) *time.Timer {
	return time.NewTimer(tiempo(x))
}

func nuevoTimerAleatorio(min, max int) *time.Timer {
	return nuevoTimer(randNum(min, max))

}

func tiempo(x int) time.Duration {
	return time.Duration(x) * time.Millisecond
}

func randNum(min, max int) int {
	rand.Seed(time.Now().UnixNano())
	return rand.Intn(max-min) + min
}

func max(x, y int) int {
	if x > y {
		return x
	} else {
		return y
	}
}
