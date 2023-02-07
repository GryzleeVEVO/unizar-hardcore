/*
Sistemas distribuidos
Práctica 3

Fichero: raft.go

Autores:

Cristian Andrei Selivanov Dobrisan (816456@unizar.es)
Dorian Boleslaw Wozniak (817570@unizar.es)

Descripción:

Implementación del algoritmo de consenso Raft. El fichero implementa el estado
de un nodo raft junto al los algoritmos de elección de líder y replicación de
entradas. El sistema implementa las RPC PedirVoto y AppendEntries para la
comunicación entre nodos.

El fichero ofrece una interfaz para iniciar el sistema, obtener el estado del
nodo, añadir una operación a la máquina de estados del sistema y detener un
nodo. Las tres últimas tambien estan disponibles a través de sus respectivos RPC.
*/

package raft

import (
	"fmt"
	"io/ioutil"
	"log"
	"math/rand"
	"os"

	//"crypto/rand"
	"sync"
	"time"

	//"net/rpc"
	"raft/internal/comun/rpctimeout"
)

const (
	// Constante para fijar valor entero no inicializado
	IntNOINICIALIZADO = -1

	// Identificadores de estados
	SEGUIDOR  = 1
	LIDER     = 2
	CANDIDATO = 3

	//  false deshabilita por completo los logs de depuracion
	// Aseguraros de poner kEnableDebugLogs a false antes de la entrega
	kEnableDebugLogs = true

	// Poner a true para logear a stdout en lugar de a fichero
	kLogToStdout = true

	// Cambiar esto para salida de logs en un directorio diferente
	kLogOutputDir = "./logs_raft/"
)

type TipoOperacion struct {
	Operacion string // La operaciones posibles son "leer" y "escribir"
	Clave     string
	Valor     string // en el caso de la lectura Valor = ""
}

// A medida que el nodo Raft conoce las operaciones de las  entradas de registro
// comprometidas, envía un AplicaOperacion, con cada una de ellas, al canal
// "canalAplicar" (funcion NuevoNodo) de la maquina de estados
type AplicaOperacion struct {
	Indice    int // en la entrada de registro
	Term      int
	Operacion TipoOperacion
}

// Tipo de dato Go que representa un solo nodo (réplica) de raft
type NodoRaft struct {
	Mux sync.Mutex // Mutex para proteger acceso a estado compartido

	// Host:Port de todos los nodos (réplicas) Raft, en mismo orden
	Nodos    []rpctimeout.HostPort
	NumNodos int
	Yo       int // indice de este nodos en campo array "nodos"

	// Estado persistente en todos los servidores
	CurrentTerm int
	VotedFor    int
	Log         []AplicaOperacion

	// Estado volatil en todos los servidores
	IdLider       int
	CurrentRol    int
	VotesRecieved int
	CommitIndex   int
	LastApplied   int

	// Estado volatil en nodos LIDER
	NextIndex  []int
	MatchIndex []int

	// Canales para comunicar internamente nodos
	LatidoCh    chan bool
	VotoMayCh   chan bool
	SClientch   chan bool
	ApplyOPch   chan AplicaOperacion
	ReplyOPch   chan string
	ResClientch chan *chan string

	// Maquina de estados para entradas de registros aplicadas
	MaqEstados map[string]string

	// Utilización opcional de este logger para depuración
	// Cada nodo Raft tiene su propio registro de trazas (logs)
	Logger *log.Logger
}

// Función para inicializar nodo nuevo
func (nr *NodoRaft) initNodo(nodos []rpctimeout.HostPort, yo int,
	canalAplicarOperacion chan AplicaOperacion) {
	nr.Nodos = nodos
	nr.NumNodos = 0
	for range nr.Nodos {
		nr.NumNodos++
	}

	nr.Yo = yo
	nr.CurrentTerm = 0
	nr.VotedFor = -1
	nr.Log = []AplicaOperacion{
		{Indice: 0, Term: 0, Operacion: TipoOperacion{Operacion: "", Clave: "",
			Valor: ""}}}
	nr.IdLider = -1
	nr.CurrentRol = SEGUIDOR
	nr.VotesRecieved = 0
	nr.CommitIndex = 0
	nr.LastApplied = 0
	nr.NextIndex = make([]int, nr.NumNodos)
	for i := range nr.NextIndex {
		nr.NextIndex[i] = 1
	}

	nr.MatchIndex = make([]int, nr.NumNodos)
	nr.LatidoCh = make(chan bool)
	nr.VotoMayCh = make(chan bool)
	nr.SClientch = make(chan bool)
	nr.ApplyOPch = canalAplicarOperacion
	nr.ReplyOPch = make(chan string)
	nr.MaqEstados = make(map[string]string)
	nr.ResClientch = make(chan *chan string, 1000)
	nr.Mux = sync.Mutex{}
}

// Función para inicializar logger de nodo
func (nr *NodoRaft) initLogger(nodos []rpctimeout.HostPort, yo int) {
	if kEnableDebugLogs {
		nombreNodo := nodos[yo].Host() + "_" + nodos[yo].Port()
		logPrefix := fmt.Sprintf("%s", nombreNodo)

		fmt.Println("LogPrefix: ", logPrefix)

		if kLogToStdout {
			nr.Logger = log.New(os.Stdout, nombreNodo+" -->> ",
				log.Lmicroseconds|log.Lshortfile)
		} else {
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
	} else {
		nr.Logger = log.New(ioutil.Discard, "", 0)
	}
}

// Creacion de un nuevo nodo raft
//
// Tabla de <Direccion IP:puerto> de cada nodo incluido a si mismo.
//
// <Direccion IP:puerto> de este nodo esta en nodos[yo]
//
// Todos los arrays nodos[] de los nodos tienen el mismo orden

// canalAplicar es un canal donde se recogerán las
// operaciones a aplicar a la máquina de estados llevada por gorutina
//
// NuevoNodo() tambien pondra en marcha gorutina para la administración del nodo
func NuevoNodo(nodos []rpctimeout.HostPort, yo int,
	canalAplicarOperacion chan AplicaOperacion) *NodoRaft {
	nr := &NodoRaft{}
	nr.initNodo(nodos, yo, canalAplicarOperacion)
	nr.initLogger(nodos, yo)

	nr.Logger.Printf("NuevoNodo: Maquina creada\n")

	// Poner en marcha gorutina para administrar maquina de estados
	go nr.actMaquina()

	// Ejecutar nodo raft
	go nr.actEstado()

	return nr
}

// Función para mantener maquina de estados del nodo actualizada
func (nr *NodoRaft) actMaquina() {
	for {
		OPaplicar := <-nr.ApplyOPch

		datoLeido := OPaplicar.Operacion.Valor

		switch OPaplicar.Operacion.Operacion {
		case "escribir":
			nr.MaqEstados[OPaplicar.Operacion.Clave] = OPaplicar.Operacion.Valor
		case "leer":
			datoLeido = nr.MaqEstados[OPaplicar.Operacion.Clave]
		}
		nr.LastApplied++

		nr.ReplyOPch <- datoLeido
	}
}

// Función que verifica y actualiza estado del nodo al oportuno
func (nr *NodoRaft) actEstado() {
	nr.Logger.Printf("actEstado: Cambiando estado\n")
	for {
		switch nr.CurrentRol {
		case SEGUIDOR:
			nr.Logger.Printf("actEstado: Entrando a estado SEGUIDOR\n")
			nr.estadoSeguidor()
		case CANDIDATO:
			nr.Logger.Printf("actEstado: Entrando a estado CANDIDATO\n")
			nr.estadoCandidato()
		case LIDER:
			nr.Logger.Printf("actEstado: Entrando a estado LIDER\n")
			nr.estadoLider()
		}
	}
}

// Función para generar tiempo aleatorio de "election timeout"
func (nr *NodoRaft) electionTimeout() time.Duration {
	rand.Seed(time.Now().UnixNano() * (11 + int64(nr.Yo)))
	return (time.Duration(rand.Intn(500-100)+100) * time.Millisecond)
}

// Función para definir comportamiento del estado SEGUIDOR
func (nr *NodoRaft) estadoSeguidor() {
	nr.Logger.Printf("SEGUIDOR: Pongo en marcha timeout de elecciones\n")
	temporizador := time.NewTicker(nr.electionTimeout())
	defer temporizador.Stop()

	for {
		select {
		case <-nr.VotoMayCh:
			// Si me llega appendentry, sigo siendo seguidor
			nr.Logger.Printf("SEGUIDOR: Voto aceptado, reinicio timeout\n")
			temporizador.Reset(nr.electionTimeout())
			continue
		case <-nr.LatidoCh:
			// Si me llega appendentry, sigo siendo seguidor
			nr.Logger.Printf("SEGUIDOR: Latido/AppendEntry recibido, reinicio timeout\n")
			temporizador.Reset(nr.electionTimeout())
			continue
		case <-temporizador.C:
			// En caso contrario, me vuelvo candidato
			nr.Logger.Printf("SEGUIDOR: Timeout de elecciones agotado, comenzando elecciones\n")
			nr.CurrentRol = CANDIDATO
			return
		}
	}
}

// Función para enviar petición voto a nodo SEGUIDOR y tratar su respuesta
func (nr *NodoRaft) tratarReplyPeticionVoto(nodo int, args *ArgsPeticionVoto,
	MayoriaCh chan bool) {
	var replyPV RespuestaPeticionVoto
	if nr.enviarPeticionVoto(nodo, args, &replyPV) {
		nr.Logger.Printf("CANDIDATO-GR: Recibido reply de peticionVoto\n")
		if nr.CurrentRol == CANDIDATO && replyPV.Term == nr.CurrentTerm &&
			replyPV.VoteGranted {
			nr.Logger.Printf("CANDIDATO-GR: Me han concedido voto, actualizo\n")
			nr.Mux.Lock()
			nr.VotesRecieved++

			if nr.VotesRecieved == (nr.NumNodos/2)+1 {
				nr.CurrentRol = LIDER
				nr.IdLider = nr.Yo
				// El canal no es bloqueante, esta con buffer y solo una
				// gorutina escribira para indicar mayoría de votos
				MayoriaCh <- true
			}
			nr.Mux.Unlock()

		} else if replyPV.Term > nr.CurrentTerm {
			nr.Logger.Printf("CANDIDATO-GR: Voto rechazado, su mandato %d es mayor que el mio %d\n",
				replyPV.Term, nr.CurrentTerm)
			nr.CurrentTerm = replyPV.Term
			nr.CurrentRol = SEGUIDOR
			nr.VotedFor = -1
			nr.VotoMayCh <- true
		}
	}
}

// Función para iniciar elecciones por parte del CANDIDATO
func (nr *NodoRaft) broadcastPeticionVoto(MayoriaCh chan bool) {
	nr.Mux.Lock()
	nr.CurrentTerm++
	nr.VotedFor = nr.Yo
	nr.VotesRecieved = 1
	nr.Mux.Unlock()

	// Preparamos mensaje a enviar
	peticionVoto := &ArgsPeticionVoto{Term: nr.CurrentTerm, CandidateId: nr.Yo,
		LastLogIndex: len(nr.Log) - 1, LastLogTerm: nr.Log[len(nr.Log)-1].Term}

	for i := range nr.Nodos {
		if i != nr.Yo {
			go nr.tratarReplyPeticionVoto(i, peticionVoto, MayoriaCh)
		}
	}
}

// Función para definir comportamiento del estado CANDIDATO
func (nr *NodoRaft) estadoCandidato() {
	nr.Logger.Printf("CANDIDATO: Pongo en marcha timeout de elecciones\n")
	temporizador := time.NewTicker(nr.electionTimeout())
	defer temporizador.Stop()

	MayoriaCh := make(chan bool, 1)

	nr.Logger.Printf("CANDIDATO: Pido votos a los demas nodos\n")
	nr.broadcastPeticionVoto(MayoriaCh)

	select {
	case <-nr.VotoMayCh:
		// En caso de que se rechaze el voto por menor mandato
		nr.Logger.Printf("CANDIDATO: Voto rechazado, dejo candidatura\n")
		return
	case <-nr.LatidoCh:
		// Si se recibe presencia de lider valido se vuelve a ser seguidor
		nr.Logger.Printf("CANDIDATO: Latido recibido, dejo candidatura\n")
		return
	case <-MayoriaCh:
		// Si recibo mayoría de votos paso a ser lider
		nr.Logger.Printf("CANDIDATO: Recibida mayoria de votos, comenzar liderazgo\n")
		return
	case <-temporizador.C:
		// En caso de agotarse mi tiempo de elecciones, vuelvo a convocarlas
		nr.Logger.Printf("CANDIDATO: Timeout de elecciones agotado, reiniciar elecciones\n")
		nr.CurrentRol = CANDIDATO
		return
	}
}

// Función para comprometer entradas del registro una vez replicadas
// más de la mitad
func (nr *NodoRaft) commitLogs() {
	nr.Mux.Lock()
	for nr.CommitIndex < len(nr.Log)-1 {
		replicados := 0
		for i := range nr.Nodos {
			if nr.MatchIndex[i] > nr.CommitIndex {
				nr.Logger.Printf("LIDER-GR: MatchIndex de nodo %d replicado\n", i)
				replicados++
			}
		}
		if replicados >= (nr.NumNodos/2)+1 {
			nr.Logger.Printf("LIDER-GR: Entrada comprometida, procedo a aplicar\n")
			// Aplicar entrada a maquina de estados del lider
			nr.ApplyOPch <- nr.Log[nr.CommitIndex+1]
			// Asegurar que devuelvo en orden los resultados a los clientes
			datoLeido := <-nr.ReplyOPch
			canalCliente := <-nr.ResClientch
			*canalCliente <- datoLeido
			nr.CommitIndex++
		} else {
			nr.Logger.Printf("LIDER-GR: No se ha replicado en más de la mitad\n")
			break
		}
	}
	nr.Mux.Unlock()
}

// Función para tratar respuesta de peticiones de replicación del LIDER
func (nr *NodoRaft) tratarReplyReplicateLog(nodo int, args *ArgAppendEntries) {
	var ReplyRL Results
	if nr.enviarReplicateLog(nodo, args, &ReplyRL) {
		nr.Logger.Printf("LIDER-GR: Recibido reply de AppendEntry\n")

		// Verificamos si el seguidor tiene mayor mandato para volvernos su
		// seguidor
		if ReplyRL.Term == nr.CurrentTerm && nr.CurrentRol == LIDER {

			// Verificamos si seguidor a aceptado petición y ha replicado,
			// en caso contrario, insistimos en que nodo actualice su log
			// al del lider
			if ReplyRL.Success {
				nr.Logger.Printf("LIDER-GR: Mi AppendEntry ha sido aceptado\n")
				nr.Mux.Lock()
				nr.NextIndex[nodo] = nr.NextIndex[nodo] + len(args.Entries)
				nr.MatchIndex[nodo] = nr.NextIndex[nodo] - 1
				nr.Mux.Unlock()
				nr.Logger.Printf("LIDER-GR: Compruebo si comprometer entrada replicada\n")
				nr.commitLogs()
			} else if nr.NextIndex[nodo]-1 > 0 {
				nr.Logger.Printf("LIDER-GR: Mi AppendEntry ha sido rechazado por LOG inconsistente\n")
				nr.Logger.Printf("LIDER-GR: Reintentando entrega\n")
				nr.Mux.Lock()
				nr.NextIndex[nodo]--
				nr.Mux.Unlock()
				nr.replicateLog(nodo)
			}
		} else if ReplyRL.Term > nr.CurrentTerm {
			nr.Logger.Printf("LIDER-GR: AppendEntry rechazado, su mandato %d es mayor que el mio %d\n",
				ReplyRL.Term, nr.CurrentTerm)
			nr.CurrentTerm = ReplyRL.Term
			nr.CurrentRol = SEGUIDOR
			nr.VotedFor = -1
			nr.LatidoCh <- true
		}
	}
}

// Función para enivar llamadas appendEntry y replicar logs, aprovechada
// para hacer broadcast de latidos y de peticiones de cliente
func (nr *NodoRaft) replicateLog(nodoS int) {
	// Si no se han recibido entradas nuevas de cliente se mantendra vacio para
	// los latidos de corazón
	var entries []AplicaOperacion

	nr.Mux.Lock()
	for j := nr.NextIndex[nodoS]; j < len(nr.Log); j++ {
		entries = append(entries, nr.Log[j])
	}

	nr.Logger.Printf("LIDER-GR: Mi log antes de replicar= %v\n", nr.Log)
	nr.Logger.Printf("LIDER-GR: Entradas a replicar por seguidores= %v\n", entries)

	logRequest := &ArgAppendEntries{Term: nr.CurrentTerm,
		LeaderId: nr.IdLider, PrevLogIndex: nr.NextIndex[nodoS] - 1,
		PrevLogTerm: nr.Log[nr.NextIndex[nodoS]-1].Term,
		Entries:     entries, LeaderCommit: nr.CommitIndex}

	nr.Mux.Unlock()

	nr.tratarReplyReplicateLog(nodoS, logRequest)
}

// Función para repartir appendEntries a seguidores indicando presencia de LIDER
// También usado para replicar entradas del cliente
func (nr *NodoRaft) broadcastAppendEntry() {
	if nr.CurrentRol == LIDER {

		for i := range nr.Nodos {
			if i != nr.Yo {
				go nr.replicateLog(i)
			}
		}
	}
}

// Función para inicialiar indices de nodos tras elección
func (nr *NodoRaft) initNodeIndex() {
	nr.Mux.Lock()
	for i := range nr.Nodos {
		if i != nr.Yo {
			nr.NextIndex[i] = len(nr.Log)
			nr.MatchIndex[i] = 0
		}
	}
	nr.MatchIndex[nr.Yo] = len(nr.Log) - 1
	nr.Mux.Unlock()
}

// Función para definir comportamiento del estado LIDER
func (nr *NodoRaft) estadoLider() {
	nr.Logger.Printf("LIDER: Inicio timeout de latido a mandar\n")
	temporizador := time.NewTicker(50 * time.Millisecond)
	defer temporizador.Stop()

	// Actualizamos indices de entradas replicadas
	nr.initNodeIndex()

	// Mandamos latido para indicar nuestra existencia como lider
	nr.Logger.Printf("LIDER: Mando latido inicial para reconocimiento\n")
	nr.broadcastAppendEntry()

	for {
		select {
		case <-temporizador.C:
			// Mandar periodicamente latido recordando a seguidore que hay lider
			nr.Logger.Printf("LIDER: Mando latido periodico y lo reprogramo\n")
			nr.broadcastAppendEntry()
			temporizador.Reset(50 * time.Millisecond)
			continue
		case <-nr.LatidoCh:
			// Si me llega un appendEntry de mayor mandato me vuelvo seguidor
			nr.Logger.Printf("LIDER: Recibido AppendEntry mayor, dejo liderazgo\n")
			return
		case <-nr.VotoMayCh:
			// Si me llega petición de candidato asequible, me vuelvo seguidor
			nr.Logger.Printf("LIDER: Peticion Voto con mayor mandato, dejo liderazgo\n")
			return
		case <-nr.SClientch:
			// En caso de petición de cliente, dejo de estar en idle
			nr.Logger.Printf("LIDER: Tratando petición de cliente\n")
			temporizador.Reset(50 * time.Millisecond)
			continue
		}
	}
}

// Metodo Para() utilizado cuando no se necesita mas al nodo
//
// Quizas interesante desactivar la salida de depuracion
// de este nodo
func (nr *NodoRaft) para() {
	go func() { time.Sleep(5 * time.Millisecond); os.Exit(0) }()
}

// Devuelve "yo", mandato en curso y si este nodo cree ser lider
//
// Primer valor devuelto es el indice de este  nodo Raft el el conjunto de nodos
// la operacion si consigue comprometerse.
// El segundo valor es el mandato en curso
// El tercer valor es true si el nodo cree ser el lider
// Cuarto valor es el lider, es el indice del líder si no es él
func (nr *NodoRaft) obtenerEstado() (int, int, bool, int, []AplicaOperacion) {
	nr.Mux.Lock()
	var yo int = nr.Yo
	var mandato int = nr.CurrentTerm
	var esLider bool = nr.CurrentRol == LIDER
	var idLider int = nr.IdLider
	var logs []AplicaOperacion = nr.Log
	nr.Mux.Unlock()

	return yo, mandato, esLider, idLider, logs
}

// El servicio que utilice Raft (base de datos clave/valor, por ejemplo)
// Quiere buscar un acuerdo de posicion en registro para siguiente operacion
// solicitada por cliente.

// Si el nodo no es el lider, devolver falso
// Sino, comenzar la operacion de consenso sobre la operacion y devolver en
// cuanto se consiga
//
// No hay garantia que esta operacion consiga comprometerse en una entrada de
// de registro, dado que el lider puede fallar y la entrada ser reemplazada
// en el futuro.
// Primer valor devuelto es el indice del registro donde se va a colocar
// la operacion si consigue comprometerse.
// El segundo valor es el mandato en curso
// El tercer valor es true si el nodo cree ser el lider
// Cuarto valor es el lider, es el indice del líder si no es él
func (nr *NodoRaft) someterOperacion(operacion TipoOperacion) (int, int,
	bool, int, string) {
	nr.Logger.Printf("RPC - SOperacion: Petición de cliente recibida\n")
	indice := -1
	mandato := -1
	EsLider := false
	idLider := -1
	valorADevolver := ""

	if nr.IdLider == nr.Yo {
		nr.Logger.Printf("RPC - SOperacion: Soy lider, acepto petición\n")
		EsLider = true

		nr.Mux.Lock()
		indice = nr.MatchIndex[nr.Yo] + 1
		mandato = nr.CurrentTerm
		idLider = nr.IdLider
		miCanal := make(chan string)
		nr.ResClientch <- &miCanal

		// Append al log de la entrada solicitada
		nr.Logger.Printf("LIDER: Mi registro antes de cliente= %v\n", nr.Log)
		nr.Logger.Printf("LIDER: Concateno operacion a mi registro\n")
		nr.Log = append(nr.Log,
			AplicaOperacion{Indice: nr.MatchIndex[nr.Yo] + 1,
				Term: nr.CurrentTerm, Operacion: operacion})
		nr.MatchIndex[nr.Yo]++
		nr.Logger.Printf("LIDER: Mi registro tras cliente= %v\n", nr.Log)
		nr.Mux.Unlock()

		// Replicar en todos los nodos la entrada que acabo de recibir
		nr.broadcastAppendEntry()

		// Avisar a lider para reiniciar timer ya que se hace broadcast
		nr.SClientch <- true

		// Cliente espera a que su operación sea aplicada
		valorADevolver = <-miCanal
	} else {
		nr.Logger.Printf("RPC - SOperacion: No soy lider, rechazo petición\n")
		nr.Mux.Lock()
		idLider = nr.IdLider
		nr.Mux.Unlock()
	}

	nr.Logger.Printf("RPC - SOperacion: Fin de petición\n")
	return indice, mandato, EsLider, idLider, valorADevolver
}

// -----------------------------------------------------------------------
// LLAMADAS RPC al API
//
// Si no tenemos argumentos o respuesta estructura vacia (tamaño cero)
type Vacio struct{}

func (nr *NodoRaft) ParaNodo(args Vacio, reply *Vacio) error {
	nr.Logger.Printf("RPC - ParaNodo: Me piden parar mi servicio, FIN\n")
	defer nr.para()
	return nil
}

type EstadoParcial struct {
	Mandato int
	EsLider bool
	IdLider int
	Logs    []AplicaOperacion
}

type EstadoRemoto struct {
	IdNodo int
	EstadoParcial
}

func (nr *NodoRaft) ObtenerEstadoNodo(args Vacio, reply *EstadoRemoto) error {
	reply.IdNodo, reply.Mandato, reply.EsLider, reply.IdLider, reply.Logs = nr.obtenerEstado()
	return nil
}

type ResultadoRemoto struct {
	ValorADevolver string
	IndiceRegistro int
	EstadoParcial
}

func (nr *NodoRaft) SometerOperacionRaft(operacion TipoOperacion,
	reply *ResultadoRemoto) error {
	reply.IndiceRegistro, reply.Mandato, reply.EsLider,
		reply.IdLider, reply.ValorADevolver = nr.someterOperacion(operacion)
	return nil
}

// -----------------------------------------------------------------------
// LLAMADAS RPC protocolo RAFT
type ArgsPeticionVoto struct {
	Term         int
	CandidateId  int
	LastLogIndex int
	LastLogTerm  int
}

// Structura de ejemplo de respuesta de RPC PedirVoto,
type RespuestaPeticionVoto struct {
	Term        int
	VoteGranted bool
}

// Metodo para RPC PedirVoto
func (nr *NodoRaft) PedirVoto(peticion *ArgsPeticionVoto,
	reply *RespuestaPeticionVoto) error {
	nr.Logger.Printf("RPC - PedirVoto: PeticionVoto de %d recibido\n",
		peticion.CandidateId)
	nr.Logger.Printf("RPC - PedirVoto: He votado a %d\n", nr.VotedFor)
	nr.Logger.Printf("RPC - PedirVoto: Su mandato %d, Mi mandato %d\n",
		peticion.Term, nr.CurrentTerm)

	// Verificamos si su último mandato es mayor que el nuestro,
	// en caso de ser iguales, solo concedemos voto si su logitud es mayor
	validLog := (peticion.LastLogTerm > nr.Log[len(nr.Log)-1].Term) ||
		((peticion.LastLogTerm == nr.Log[len(nr.Log)-1].Term) &&
			(peticion.LastLogIndex >= len(nr.Log)-1))

	// Compruebo si el mandato del candidato me supera
	if validLog && peticion.Term > nr.CurrentTerm {
		nr.Logger.Printf("RPC - PedirVoto: Su mandato %d > mi mandato %d, paso a ser seguidor\n",
			peticion.Term, nr.CurrentTerm)
		nr.CurrentTerm = peticion.Term
		nr.CurrentRol = SEGUIDOR
		nr.VotedFor = -1
	}

	// Comprobamos si se debe conceder voto
	if peticion.Term == nr.CurrentTerm && validLog &&
		(nr.VotedFor == -1 || nr.VotedFor == peticion.CandidateId) {
		nr.Logger.Printf("RPC - PedirVoto: Concedo voto a %d\n",
			peticion.CandidateId)
		nr.VotedFor = peticion.CandidateId
		reply.Term = nr.CurrentTerm
		reply.VoteGranted = true
		nr.VotoMayCh <- true
	} else {
		nr.Logger.Printf("RPC - PedirVoto: No concedo voto a %d\n",
			peticion.CandidateId)
		reply.Term = nr.CurrentTerm
		reply.VoteGranted = false
	}

	return nil
}

type ArgAppendEntries struct {
	Term         int
	LeaderId     int
	PrevLogIndex int
	PrevLogTerm  int
	Entries      []AplicaOperacion
	LeaderCommit int
}

type Results struct {
	Term    int
	Success bool
}

// Función para obtener el menor valor numérico dado a y b
func elemMinimo(a, b int) int {
	if a < b {
		return a
	} else {
		return b
	}
}

// Función para concatenar / truncar entradas del registro provenientes de un
// un lider al registro del nodo que acepte su petición
func (nr *NodoRaft) appendLogs(prevLogLen, LeaderCommit int,
	entries []AplicaOperacion) {
	// Verificar si hay inconsistencias en el log para truncar entradas erroneas
	if len(entries) > 0 && len(nr.Log) > prevLogLen {
		nr.Logger.Printf("RPC - AppendEntries: Entry con length > 0 recibido\n")
		indice := elemMinimo(len(nr.Log), prevLogLen+len(entries)) - 1

		if nr.Log[indice].Term != entries[indice-prevLogLen].Term {
			nr.Logger.Printf("RPC - AppendEntries: Hay que truncar entradas\n")
			nr.Log = nr.Log[:prevLogLen]
		}
	}
	// Verificar si hace falta hacer append de entradas restantes al registro
	if prevLogLen+len(entries) > len(nr.Log) {
		nr.Logger.Printf("RPC - AppendEntries: Añadiendo entradas restantes\n")
		for j := len(nr.Log) - prevLogLen; j < len(entries); j++ {
			nr.Log = append(nr.Log, entries[j])
		}
	}
	nr.Logger.Printf("RPC - AppendEntries: Mi registro tras append de lider= %v\n",
		nr.Log)

	// Verificamos si quedan entradas comprometidas por aplicar
	if LeaderCommit > nr.CommitIndex {
		nr.Logger.Printf("RPC - AppendEntries: Aplicando entradas comprometidas\n")
		for j := nr.CommitIndex + 1; j <= LeaderCommit; j++ {
			// Enviar mensaje a canal de aplicación
			nr.ApplyOPch <- nr.Log[j]
			<-nr.ReplyOPch
		}
		nr.CommitIndex = elemMinimo(LeaderCommit, len(nr.Log)-1)
	}
}

// Metodo de tratamiento de llamadas RPC AppendEntries
func (nr *NodoRaft) AppendEntries(args *ArgAppendEntries,
	results *Results) error {
	nr.Logger.Printf("RPC - AppendEntries: Recibido AppendEntry de %d\n",
		args.LeaderId)
	nr.Logger.Printf("RPC - AppendEntries: Su mandato %d, Mi mandato %d\n",
		args.Term, nr.CurrentTerm)

	if args.Term > nr.CurrentTerm {
		nr.Logger.Printf("RPC - AppendEntries: Actualizado mandato al mayor\n")
		nr.CurrentTerm = args.Term
		nr.VotedFor = -1
	}
	if args.Term == nr.CurrentTerm {
		nr.Logger.Printf("RPC - AppendEntries: Aceptado, me vuelvo seguidor\n")
		nr.CurrentRol = SEGUIDOR
		nr.IdLider = args.LeaderId
	}

	nr.Logger.Printf("RPC - AppendEntries: PrevLogIndex= %d, Entries= %v\n",
		args.PrevLogIndex, args.Entries)
	nr.Logger.Printf("RPC - AppendEntries: MiUltIndex= %d, MiRegistro= %v\n",
		len(nr.Log)-1, nr.Log)

	// Verificamos si la última entrada conocida por el lider es la misma que
	// la nuestra última, o esta si quiera dentro de nuestro log
	validLog := (len(nr.Log)-1 >= args.PrevLogIndex) &&
		(nr.Log[args.PrevLogIndex].Term == args.PrevLogTerm)

	/* LOGS PARA DEPURACIÓN */
	if len(nr.Log)-1 >= args.PrevLogIndex {
		nr.Logger.Printf("RPC - AppendEntries: Su indice previo esta en rango\n")
		if nr.Log[args.PrevLogIndex].Term == args.PrevLogTerm {
			nr.Logger.Printf("RPC - AppendEntries: Su último mandato coincide\n")
		} else {
			nr.Logger.Printf("RPC - AppendEntries: Su último mandato no coincide\n")
		}
	} else {
		nr.Logger.Printf("RPC - AppendEntries: Su indice previo no esta en rango\n")
	}

	// Si el mandato de la petición es mayor y nuestro log consistente respecto
	// del ultimo log conocido por el solicitante de la petición, aceptamos
	// y procedemos a actualizar nuestro registro con sus entradas.
	// En caso contrario, rechazo y espero que solicitante mande petición de
	// vuelta para poder replicar su registro de forma correcta.
	if args.Term == nr.CurrentTerm && validLog {
		nr.Logger.Printf("RPC - AppendEntries: Replico entradas de lider válido\n")
		nr.appendLogs(args.PrevLogIndex+1, args.LeaderCommit, args.Entries)
		results.Term = nr.CurrentTerm
		results.Success = true
		nr.LatidoCh <- true
	} else {
		nr.Logger.Printf("RPC - AppendEntries: Denegado, mandato o registro\n")
		results.Term = nr.CurrentTerm
		results.Success = false
	}

	return nil
}

// ----- Metodos/Funciones a utilizar como clientes
//
//

// Ejemplo de código enviarPeticionVoto
//
// nodo int -- indice del servidor destino en nr.nodos[]
//
// args *RequestVoteArgs -- argumentos par la llamada RPC
//
// reply *RequestVoteReply -- respuesta RPC
//
// Los tipos de argumentos y respuesta pasados a CallTimeout deben ser
// los mismos que los argumentos declarados en el metodo de tratamiento
// de la llamada (incluido si son punteros
//
// Si en la llamada RPC, la respuesta llega en un intervalo de tiempo,
// la funcion devuelve true, sino devuelve false
//
// la llamada RPC deberia tener un timout adecuado.
//
// Un resultado falso podria ser causado por una replica caida,
// un servidor vivo que no es alcanzable (por problemas de red ?),
// una petición perdida, o una respuesta perdida
//
// Para problemas con funcionamiento de RPC, comprobar que la primera letra
// del nombre  todo los campos de la estructura (y sus subestructuras)
// pasadas como parametros en las llamadas RPC es una mayuscula,
// Y que la estructura de recuperacion de resultado sea un puntero a estructura
// y no la estructura misma.
func (nr *NodoRaft) enviarPeticionVoto(nodo int, args *ArgsPeticionVoto,
	reply *RespuestaPeticionVoto) bool {

	err := nr.Nodos[nodo].CallTimeout("NodoRaft.PedirVoto", args, reply,
		20*time.Millisecond)

	return err == nil
}

func (nr *NodoRaft) enviarReplicateLog(nodo int, args *ArgAppendEntries,
	reply *Results) bool {

	err := nr.Nodos[nodo].CallTimeout("NodoRaft.AppendEntries", args, reply,
		20*time.Millisecond)

	return err == nil
}
