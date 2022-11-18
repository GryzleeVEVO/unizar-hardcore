/*
Sistemas Distribuidos
Práctica 3

Fichero: raft/internal/raft/raft.go

Autores:

Cristian Andrei Selivanov Dobrisan (816456@unizar.es)
Dorian Boleslaw Wozniak (817570@unizar.es)

Descripción:
*/

package raftoide

import (
	"fmt"
	"io/ioutil"
	"log"
	"math/rand"
	"net"
	"net/rpc"
	"os"
	"raft/internal/comun/check"
	"raft/internal/comun/rpctimeout"
	"sync"
	"time"
)

const (
	// Habilitar traza de depuración
	kEnableDebugLogs = true

	// Mostrar traza en salida estandar (solo si kEnableDebugLogs = true)
	kLogToStdout = true

	// Ruta de creación de ficheros de depuración
	kLogOutputDir = "./log_raft/"

	SEGUIDOR  = 1
	LIDER     = 2
	CANDIDATO = 3
)

/* --- ALGORITMO RAFT --- */

// Representa el estado de un nodo individual dentro del sistema Raft
type NodoRaft struct {
	Mux sync.Mutex

	// Identidad

	nodos    []rpctimeout.HostPort // dirección:puerto de todos los nodos
	yo       int                   // Índice de este nodo en Nodos
	numNodos int                   // Número de nodos conocidos
	estado   int                   // Estado actual
	mandato  int                   // Mandato actual

	// Liderazgo

	idLider int // Índice del líder
	idVoto  int // Índice del último nodo votado

	// Estado de la réplica

	//Registro     []EntradaRaft // Entradas de la réplica local
	//Comprometido int           // Índice de última entrada con consenso

	// Atributos del líder (sólo cuando este nodo lidera)

	//sigEntrada       []int // Índice de siguiente entrada a enviar a cada nodo
	//entradaReplicada []int // Índice de última entrada con consenso de cada nodo

	// Canal de recepción de operaciones a aplicar
	//CanalAplicarOperacion chan AplicaOperacion

	// Traza de depuración (opcional)
	Logger *log.Logger

	appendReciente chan bool    // Marca si no es necesario enviar latido
	appendRecibido chan Results // Encola respuestas si no soy seguidor
}

// Una entrada dentro del registro de estados replicados del sistema Raft
type EntradaRaft struct {
	Operación TipoOperacion // Tipo de operacion
	Mandato   int           // Mandato en el que se ha añadido
}

// Una operación de un sistema de almacenamiento clave-valor
type TipoOperacion struct {
	Operacion string // Tipo de operación. Puede ser "Leer" o "Escribir"
	Clave     string // Clave sobre la que realizar la operación
	Valor     string // Valor a escribir (Si operación = "Leer", Valor = -1)
}

/* --- INTERFAZ --- */

// Almacena una orden para someter una operación en el registro del nodo
type AplicaOperacion struct {
	Indice    int           // Índice donde insertar
	Operacion TipoOperacion // Operación a añaidr
}

// Dado un conjunto de nodos Raft, inicializa el nodo con el índice dado.
func NuevoNodo(nodos []rpctimeout.HostPort, yo int,
	canalAplicarOperacion chan AplicaOperacion) *NodoRaft {

	nr := &NodoRaft{}

	nr.inicializarNodo(nodos, yo, canalAplicarOperacion)
	nr.inicializarLogger()

	go nr.recepcion()
	go nr.estadoActual()

	return nr
}

func (nr *NodoRaft) SometerOperacion(operacion TipoOperacion) (int, int, bool,
	int) {
	return 0, 0, false, 0
}

// Devuelve el estado del nodo: su índice, su mandato actual, si cree ser lider
// y el índice del líder actual.
func (nr *NodoRaft) ObtenerEstado() (int, int, bool, int) {
	return nr.yo, nr.mandato, nr.yo == nr.idLider, nr.idLider
}

// Detiene este nodo del conjunto.
func (nr *NodoRaft) Para() {
	go func() { time.Sleep(5 * time.Millisecond); os.Exit(0) }()
}

/* --- RPC PedirVoto() --- */

// Argumentos para una RPC PedirVoto
type ArgsPeticionVoto struct {
	Mandato     int // Mandato actual del candidato
	IdCandidato int // Índice del candidato emisor
}

// Respuesta de una RPC PedirVoto
type RespuestaPeticionVoto struct {
	Mandato      int  // Mandato nuevo para el candidato
	VotoObtenido bool // Verdad si el candidato ha obtenido un voto
}

// Función RPC que devuelve al llamante el resultado de la votación de este
// nodo durante una elección junto al mandato actualizado.
func (nr *NodoRaft) PedirVoto(args *ArgsPeticionVoto,
	reply *RespuestaPeticionVoto) error {

	reply.Mandato, reply.VotoObtenido =
		nr.tratarPedirVoto(args.Mandato, args.IdCandidato)

	return nil
}

// Si este nodo no tiene un mandato mayor y aún no ha votado o ya ha votado
// al candidato que pide el voto, devuelve verdad. En caso contrario devuelve
// falso. Tambien devuelve el máximo entre en mandato del nodo y del candidato.
func (nr *NodoRaft) tratarPedirVoto(mandato int, idCandidato int) (int, bool) {

	// Rechazo si el mandato del candidato es estrictamente menor que el mío
	mandatoValido := mandato >= nr.mandato

	// Otorgo voto si no he votado a nadie o ya he votado a este candidato
	otorgarVoto := nr.idVoto == -1 || nr.idVoto == idCandidato

	if mandatoValido && otorgarVoto { /* Voto otorgado */
		return max(nr.mandato, mandato), true
	} else { /* Voto rechazado */
		return max(nr.mandato, mandato), false
	}
}

// Dado un nodo i que no sea el mismo, le envía una RPC PedirVoto(). Devuelve
// si el nodo remoto le ha votado, y el mandato actualizado.
func (nr *NodoRaft) enviarPedirVoto(ch chan RespuestaPeticionVoto) {
	for i := 0; i < nr.numNodos; i++ { /* Para cada nodo */
		if i != nr.yo { /* Salvo él mismo */
			go func() { /* Lanza el mensaje de forma concurrente */
				args := ArgsPeticionVoto{Mandato: nr.mandato,
					IdCandidato: nr.yo}
				var reply RespuestaPeticionVoto
				err := nr.nodos[i].CallTimeout("NodoRaft.PedirVoto", args,
					reply, time.Duration(50)*time.Millisecond)
				nr.checkError(err, "enviarPedirVoto()",
					"rpctimeout.HostPort.CallTimeout()")
				ch <- reply // Devuelve el resultado en un canal
			}()
		}
	}
}

/* --- RPC AppendEntries() --- */

// Argumentos para una RPC AppendEntries
type ArgAppendEntries struct {
	Mandato int // Mandato actual del líder
	IdLider int // Índice del lider

	//IndUltEntrada  int           // Índice de última entrada
	//MandUltEntrada int           // Mandato de última entrada
	//Entradas       []EntradaRaft // Entradas a insertar tras la última entrada
	//Comprometido   int           // Índice de la última entrada con consenso
}

// Respuesta de una RPC AppendEntries
type Results struct {
	Mandato int  // Mandato nuevo para el líder
	Exito   bool // Verdad si se ha realizado la inserción
}

func (nr *NodoRaft) AppendEntries(args *ArgAppendEntries,
	reply *Results) error {

	reply.Mandato, reply.Exito =
		nr.tratarAppendEntries(args.Mandato, args.IdLider)

	nr.appendRecibido <- *reply

	return nil
}

func (nr *NodoRaft) enviarAppendEntries(ch chan Results) {
	for i := 0; i < nr.numNodos; i++ {
		if i != nr.yo {
			go func() {
				args := ArgAppendEntries{Mandato: nr.mandato,
					IdLider: nr.idLider}
				var reply Results
				err := nr.nodos[i].CallTimeout("NodoRaft.AppendEntries", args,
					reply, time.Duration(50)*time.Millisecond)
				check.CheckError(err, "")
				ch <- reply
			}()
		}
	}

	nr.appendReciente <- true // Avisa que no es necesario un latido
}

func (nr *NodoRaft) tratarAppendEntries(mandato int, idLider int) (int, bool) {
	// Rechazo si el mandato del lider es estrictamente menor que el mío
	mandatoValido := mandato >= nr.mandato

	/* Gestiona registro */

	if mandatoValido { /* Ha añadido correctamente */
		return max(nr.mandato, mandato), true
	} else { /* No ha añadido correctamente */
		return max(nr.mandato, mandato), false
	}
}

/* --- RPC EXTERNAS (SometerOperacion() / ObtenerEstado() / Para()) --- */

/*
// Argumento/respuesta vacía
type Vacio struct{}

// Datos del estado de un nodo
type EstadoParcial struct {
	Mandato int  // Mandato actual
	EsLider bool // El nodo se considera líder
	IdLider int  // Índice del líder actual
}

// Respuesta de una RPC ObtenerEstado()
type EstadoRemoto struct {
	IdNodo int // Índice del nodo consultado
	EstadoParcial
}

// Resupesta de una RPC SometerOperacion()
type ResultadoRemoto struct {
	// ValorADevolver string
	IndiceRegistro int
	EstadoParcial
}

func (nr *NodoRaft) SometerOperacionRaft(operacion TipoOperacion,
	reply *ResultadoRemoto) error {

}

func (nr *NodoRaft) ObtenerEstadoNodo(args Vacio, reply *EstadoRemoto) error {

}

func (nr *NodoRaft) ParaNodo(args Vacio, reply *Vacio) error {

}
*/

/* --- FUNCIONES INTERNAS --- */

// Devuelve número pseudoaleatorio dado un intervalo [A, B]
func randNum(A int, B int) int {
	rand.Seed(time.Now().UnixNano())
	return rand.Intn(B-A) + A
}

// Devuelve el mayor de entre un par de enteros
func max(x int, y int) int {
	if x > y {
		return x
	} else {
		return y
	}
}

// Inicializa los miembros del struct que representa el estado del nodo
func (nr *NodoRaft) inicializarNodo(nodos []rpctimeout.HostPort, yo int,
	canalAplicarOperacion chan AplicaOperacion) {

	nr.nodos = nodos
	nr.yo = yo

	nr.numNodos = 0
	for range nr.nodos {
		nr.numNodos++
	}

	nr.mandato = 0
	nr.idLider = -1
	nr.idVoto = -1

	//nr.canalAplicarOperacion = canalAplicarOperacion
}

// Si se ha habilitado en las constantes de este módulo, inicializa la traza de
// depuración del nodo
func (nr *NodoRaft) inicializarLogger() {
	if kEnableDebugLogs { /* logger habilitado */
		nombreNodo := nr.nodos[nr.yo].Host() + "_" + nr.nodos[nr.yo].Port()
		logPrefix := fmt.Sprintf("%s", nombreNodo)
		fmt.Println("LogPrefix: ", logPrefix)
		if kLogToStdout { /* salida estandar */
			nr.Logger = log.New(os.Stdout, nombreNodo+" -->> ",
				log.Lmicroseconds|log.Lshortfile)
		} else { /* en un archivo */
			err := os.MkdirAll(kLogOutputDir, os.ModePerm)
			if err != nil {
				panic(err.Error())
			}
			logOutputFile, err := os.OpenFile(fmt.Sprintf("%s/%s.txt",
				kLogOutputDir, logPrefix), os.O_RDWR|os.O_CREATE|os.O_TRUNC, 0755)
			if err != nil {
				panic(err.Error())
			}
			nr.Logger = log.New(logOutputFile,
				logPrefix+" -> ", log.Lmicroseconds|log.Lshortfile)
		}
		nr.Logger.Println("logger initialized")
	} else { /* logger no habilitado */
		nr.Logger = log.New(ioutil.Discard, "", 0)
	}
}

// En caso de error del nodo, muestra información sobre este en stderr
func (nr *NodoRaft) checkError(err error, where string, what string) {
	if err != nil {
		fmt.Fprintf(os.Stderr, "[ERROR] [Nodo %d] [%s] %s: %s",
			nr.yo, where, what, err.Error())
		os.Exit(1)
	}
}

// Registra las funciones exportadas como RPC y comienza a aceptarlas
func (nr *NodoRaft) recepcion() {
	// Registra las funciones exportadas como RPC
	err := rpc.Register(nr)
	nr.checkError(err, "recepcion()", "rpc.Register()")

	// Comienza a escuchar y aceptar peticiones
	l, err := net.Listen("tcp", string(nr.nodos[nr.yo]))
	nr.checkError(err, "recepcion()", "net.listen()")

	for {
		rpc.Accept(l)
	}
}

/* --- ESTADOS DEL NODO --- */

// Selecciona el algoritmo adecuado según el estado actual del nodo
func (nr *NodoRaft) estadoActual() {
	for {
		switch nr.estado {
		case SEGUIDOR:
			nr.seguidor()
		case LIDER:
			nr.lider()
		case CANDIDATO:
			nr.candidato()
		}
	}
}

// Acciones a realizar como seguidor
func (nr *NodoRaft) seguidor() {
	for {
		select {
		case <-nr.appendReciente: // Recibe latido
			continue

		case <-time.After(time.Duration(randNum(150, 300)) * time.Millisecond): // Tiempo se agota, se vuelve candidato
			nr.estado = CANDIDATO
			return
		}
	}
}

// Acciones a realizar como lider
func (nr *NodoRaft) lider() {
	nr.idLider = nr.yo

	for {
		select {
		case <-nr.appendReciente: // Se ha mandado un append de forma reciente
			continue

		case <-time.After(time.Duration(100) * time.Millisecond): // Enviar un latido si no se ha mandado nada
			nr.enviarAppendEntries(nil)

		case reply := <-nr.appendRecibido: // Ha recibido AppendEntries()
			if reply.Exito { /* Hay un líder con mayor mandato */
				nr.estado = SEGUIDOR
				return
			}
		}
	}
}

// Acciones a realizar durante unas elecciones
func (nr *NodoRaft) candidato() {
	nr.idLider = -1
	nr.idVoto = -1
	nr.mandato++
	votos := 1
	var respuestaVoto chan RespuestaPeticionVoto
	nr.enviarPedirVoto(respuestaVoto)
	timer := time.After(time.Duration(randNum(150, 300)) * time.Millisecond)

	for {
		select {
		case reply := <-respuestaVoto: // Alguien responde al voto del candidato
			if reply.VotoObtenido {
				votos++
				if votos >= nr.numNodos/2+1 {
					nr.estado = LIDER
					nr.enviarAppendEntries(nil)
					return
				}
			}

		case reply := <-nr.appendRecibido: // Ha recibido un AppendEntries()
			if reply.Exito { /* Hay un nuevo líder legitimo */
				nr.estado = SEGUIDOR
				return
			}

		case <-timer: // Vence el timer
			nr.estado = CANDIDATO
			return
		}
	}
}
