package raft

import (
	"fmt"
	"io"
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
	kLogToStdout = false

	// Ruta de creación de ficheros de depuración
	kLogOutputDir = "./log_raft/"

	// Identificadores de estados
	SEGUIDOR  = 1
	LIDER     = 2
	CANDIDATO = 3

	// Atributo no inicializado
	RAFT_NIL = -1

	// Tiempos de espera
	RAFT_RPC_TIMEOUT          = 10
	RAFT_HEARTBEAT_TIMEOUT    = 150
	RAFT_MIN_ELECTION_TIMEOUT = 100
	RAFT_MAX_ELECTION_TIMEOUT = 500
)

/* ---------------------------------------------------------------------------*/
/* --- ESTADO --- */

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

	idLider        int // Índice del líder
	idVoto         int // Índice del último nodo votado
	votosRecibidos int // Número de votos recibidos en última votación

	// Canales de comunicación

	hayLider          chan bool //
	operacionSometida chan bool //

	// Traza de depuración (opcional)
	Logger *log.Logger
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

/* ---------------------------------------------------------------------------*/
/* --- ALGORITMO --- */

// Registra las funciones exportadas como RPC y comienza a aceptarlas
func (nr *NodoRaft) recepcion() {
	// Registra las funciones exportadas como RPC
	err := rpc.Register(nr)
	nr.checkError(err, "recepcion()", true)

	// Comienza a escuchar y aceptar peticiones
	l, err := net.Listen("tcp", string(nr.nodos[nr.yo-1]))
	nr.checkError(err, "recepcion()", true)

	nr.log("RPC registradas, se inicia la escucha")

	for {
		rpc.Accept(l)
		nr.log("Conexión aceptada")
	}
}

// Selecciona el algoritmo adecuado según el estado actual del nodo
func (nr *NodoRaft) estadoActual() {
	for {
		nr.log("Seleccionando estado...")
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
	nr.log("El nodo ahora es SEGUIDOR")

	for {
		select {
		case <-nr.hayLider:
			nr.log("El nodo ha recibido un latido del líder")
			continue

		case <-nuevoTimer(RAFT_HEARTBEAT_TIMEOUT):
			nr.log("El tiempo para recibir un latido se ha acabado")
			nr.estado = CANDIDATO
			return
		}
	}
}

// Acciones a realizar como lider
func (nr *NodoRaft) lider() {
	//nr.enviarAppendEntries()
	nr.log("El nodo ahora es LÍDER")

	for {
		select {
		case <-nr.operacionSometida:
			nr.log("Ha sometido una operación, cancelo latido")
			continue

		case <-time.After(time.Duration(100) * time.Millisecond):
			nr.log("Envío latido")
			//nr.enviarAppendEntries()

		case <-nr.hayLider:
			nr.log("Hay un líder con mayor mandato que yo")
			nr.estado = SEGUIDOR
			return
		}
	}
}

// Acciones a realizar durante unas elecciones
func (nr *NodoRaft) candidato() {
	nr.log("El nodo ahora es CANDIDATO")

	// Inicia la votación y lanza las RPC de forma asíncrona
	var soyLider, cancelarCandidatura chan bool
	go nr.iniciarVotacion(soyLider, cancelarCandidatura)
	timer := nuevoTimerAleatorio(
		RAFT_MIN_ELECTION_TIMEOUT, RAFT_MAX_ELECTION_TIMEOUT)
	nr.log("El nodo ha comenzado una elección")

	// Espera a un evento que cambie su estado
	select {
	case <-soyLider:
		nr.log("Fin de votación: El nodo ha salido elegido líder")
		nr.estado = LIDER
	case <-cancelarCandidatura:
		nr.log("Fin de votación: Mi mandato es demasiado bajo para competir")
		nr.estado = SEGUIDOR
	case <-nr.hayLider:
		nr.log("Fin de votación: Hay un nuevo líder")
		nr.estado = SEGUIDOR
	case <-timer:
		nr.log("Fin de votación: No se ha encontrado un líder")
		nr.estado = CANDIDATO
	}
}

// Inicia votación y manda PedirVoto al resto de los candidatos. Si responden
// en un tiempo dado, trata el resultado. Si recibe mayoría, indica que
// se convierte en líder
func (nr *NodoRaft) iniciarVotacion(soyLider, cancelarCandidatura chan bool) {
	// Inicio parámetros
	nr.Mux.Lock()
	nr.mandato++          // Aumento mi mandato
	nr.votosRecibidos = 1 // Me voto a mi mismo
	nr.idLider = -1       // No conozco líder
	nr.idVoto = -1        // No he votado a nadie
	nr.Mux.Unlock()

	// Envío PedirVoto a todos los nodos salvo a mi mismo
	for i, nodo := range nr.nodos {
		if i != nr.yo-1 {
			go nr.enviarPedirVoto(nodo, soyLider, cancelarCandidatura)
		}
	}
}

/* ---------------------------------------------------------------------------*/
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

	list := ""
	for i, nodo := range nr.nodos {
		list += fmt.Sprintf("\n\t%d\t%s", i+1, nodo)
	}
	nr.log("Se encuentran disponibles los siguientes nodos:" + list)

	go nr.recepcion()
	go nr.estadoActual()

	return nr
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

/* ---------------------------------------------------------------------------*/
/* --- RCP PedirVoto() --- */

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

// Recibe una petición de otro nodo para que le vote. Si su mandato es igual
// o superior, y el nodo no ha votado a nadie, le otorga el voto. Actualiza
// su estado interno de forma acorde
func (nr *NodoRaft) PedirVoto(args *ArgsPeticionVoto,
	reply *RespuestaPeticionVoto) error {

	// Rechazo si el mandato del candidato es estrictamente menor que el mío
	mandatoValido := args.Mandato >= nr.mandato
	reply.Mandato = max(args.Mandato, nr.mandato)

	// Otorgo voto si no he votado a nadie o ya he votado a este candidato
	otorgarVoto := nr.idVoto == -1 || nr.idVoto == args.IdCandidato
	reply.VotoObtenido = mandatoValido && otorgarVoto

	nr.log(fmt.Sprintf("RPC PedirVoto: He recibido mandato %d, índice %d",
		args.Mandato, args.IdCandidato))

	nr.log(fmt.Sprintf("RPC PedirVoto: Mi mandato es %d, he votado a %d",
		nr.mandato, nr.idVoto))

	nr.log(fmt.Sprintf("RPC PedirVoto: He enviado mandato %d, voto %t",
		reply.Mandato, reply.VotoObtenido))

	// Actualiza sus valores según lo recibido
	nr.mandato = reply.Mandato
	if otorgarVoto {
		nr.idVoto = args.IdCandidato
	}
	nr.Mux.Unlock()

	return nil
}

// Envía un RPC PedirVoto() al nodo dado. El nodo debe ser diferente al emisor.
// Si recibe una respuesta, actualiza su estado interno y actúa si debe
// convertirse en líder o seguidor
func (nr *NodoRaft) enviarPedirVoto(nodo rpctimeout.HostPort,
	soyLider, cancelarCandidatura chan bool) {

	args := ArgsPeticionVoto{Mandato: nr.mandato, IdCandidato: nr.yo}
	reply := RespuestaPeticionVoto{}

	err := nodo.CallTimeout("NodoRaft.PedirVoto", args,
		&reply, time.Duration(RAFT_RPC_TIMEOUT)*time.Millisecond)

	nr.checkError(err, "enviarPedirVoto", false)

	if err == nil {
		nr.log(fmt.Sprintf("enviarPedirVoto: He recibido mandato %d, voto %t",
			reply.Mandato, reply.VotoObtenido))

		nr.log(fmt.Sprintf("enviarPedirVoto: Mi mandato es %d, he votado a %d",
			nr.mandato, nr.idVoto))

		nr.Mux.Lock()
		// Si mi mandato es menor que el del receptor, no puedo ser candidato
		if nr.mandato < reply.Mandato {
			nr.mandato = reply.Mandato
			cancelarCandidatura <- true
		}

		// Si me ha votado, me sumo un voto
		if reply.VotoObtenido {
			nr.votosRecibidos++

			// Si tengo mayoría, me vuelvo líder
			if nr.votosRecibidos >= nr.numNodos/2+1 {
				nr.idLider = nr.yo
				soyLider <- true
			}
		}
		nr.Mux.Unlock()
	}
}

/* ---------------------------------------------------------------------------*/
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

	return nil
}

func (nr *NodoRaft) enviarAppendEntries() {
	for i := 0; i < nr.numNodos; i++ {
		if i != nr.yo {
			go func() {
				args := ArgAppendEntries{Mandato: nr.mandato,
					IdLider: nr.idLider}
				reply := &Results{}
				err := nr.nodos[i].CallTimeout("NodoRaft.AppendEntries", args,
					reply, time.Duration(20)*time.Millisecond)
				check.CheckError(err, "")
			}()
		}
	}

	//nr.appendReciente <- true // Avisa que no es necesario un latido
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

/* ---------------------------------------------------------------------------*/
/* --- FUNCIONES AUXILIARES --- */

// Inicializa los miembros del struct que representa el estado del nodo
func (nr *NodoRaft) inicializarNodo(nodos []rpctimeout.HostPort, yo int,
	canalAplicarOperacion chan AplicaOperacion) {

	nr.nodos = nodos
	nr.yo = yo

	nr.numNodos = 0
	for range nr.nodos {
		nr.numNodos++
	}

	nr.estado = SEGUIDOR
	nr.mandato = 0
	nr.idLider = RAFT_NIL
	nr.idVoto = RAFT_NIL
}

// Si se ha habilitado en las constantes de este módulo, inicializa la traza de
// depuración del nodo
func (nr *NodoRaft) inicializarLogger() {
	if kEnableDebugLogs { /* logger habilitado */
		fprefix := nr.nodos[nr.yo-1].Host() + "_" + nr.nodos[nr.yo-1].Port()
		prefix := "[" + fprefix + "] "
		lflags := log.Lmicroseconds | log.Lshortfile
		oflags := os.O_RDWR | os.O_CREATE | os.O_TRUNC
		file := fmt.Sprintf("%s/%s.txt", kLogOutputDir, fprefix)

		if kLogToStdout { /* salida estandar */
			nr.Logger = log.New(os.Stdout, prefix, lflags)
		} else { /* en un archivo */
			err := os.MkdirAll(kLogOutputDir, os.ModePerm)
			nr.checkError(err, "inicializarLogger()", true)

			logOutputFile, err := os.OpenFile(file, oflags, 0755)
			nr.checkError(err, "inicializarLogger()", true)

			nr.Logger = log.New(logOutputFile, prefix, lflags)
		}
		nr.log("Traza iniciada")
	} else { /* logger no habilitado */
		nr.Logger = log.New(io.Discard, "", 0)
	}
}

// Añade una nueva entrada a la traza de depuración
func (nr *NodoRaft) log(what string) {
	nr.Logger.Println(what)
}

// En caso de error del nodo, muestra información sobre este en stderr
func (nr *NodoRaft) checkError(err error, where string, panic bool) {
	if err != nil {
		fmt.Fprintf(os.Stderr, "[ERROR] [Nodo %d] [%s] %s\n",
			nr.yo, where, err.Error())
		if panic {
			os.Exit(1)
		}
	}
}

// Devuelve un entero pseudoaleatorio dado un intervalo [A, B]
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

// Devuelve un nuevo timer con un tiempo dado en milisegundos
func nuevoTimer(ms int) <-chan time.Time {
	return time.NewTimer(time.Duration(ms) * time.Millisecond).C
}

// Devuelve un nuevo timer con un tiempo aleatorio dentro de un rango em
// milisegundos
func nuevoTimerAleatorio(min, max int) <-chan time.Time {
	return nuevoTimer(randNum(min, max))
}
