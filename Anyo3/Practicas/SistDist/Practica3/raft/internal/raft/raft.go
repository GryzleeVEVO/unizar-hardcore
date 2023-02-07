/*
Sistemas distribuidos
Práctica 3

Fichero: *meter fichero aquí*

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

//
// API
// ===
// Este es el API que vuestra implementación debe exportar
//
// nodoRaft = NuevoNodo(...)
//   Crear un nuevo servidor del grupo de elección.
//
// nodoRaft.Para()
//   Solicitar la parado de un servidor
//
// nodo.ObtenerEstado() (yo, mandato, esLider)
//   Solicitar a un nodo de elección por "yo", su mandato en curso,
//   y si piensa que es el msmo el lider
//
// nodoRaft.SometerOperacion(operacion interface()) (indice, mandato, esLider)

// type AplicaOperacion

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
	kLogToStdout = false

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
	Operacion TipoOperacion
}

// Datos a almacenar en el registro de entradas del nodo
type ElementoLog struct {
	Operacion TipoOperacion
	Term      int
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
	Log         []ElementoLog

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
	LatidoCh  chan bool
	SClientch chan TipoOperacion
	RClientch chan bool

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
	OPinit := TipoOperacion{Operacion: "", Clave: "", Valor: ""}
	nr.Log = append([]ElementoLog{}, ElementoLog{Operacion: OPinit, Term: 0})
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
	nr.SClientch = make(chan TipoOperacion)
	nr.RClientch = make(chan bool)
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

// Creacion de un nuevo nodo de eleccion
//
// Tabla de <Direccion IP:puerto> de cada nodo incluido a si mismo.
//
// <Direccion IP:puerto> de este nodo esta en nodos[yo]
//
// Todos los arrays nodos[] de los nodos tienen el mismo orden

// canalAplicar es un canal donde, en la practica 5, se recogerán las
// operaciones a aplicar a la máquina de estados. Se puede asumir que
// este canal se consumira de forma continúa.
//
// NuevoNodo() debe devolver resultado rápido, por lo que se deberían
// poner en marcha Gorutinas para trabajos de larga duracion
func NuevoNodo(nodos []rpctimeout.HostPort, yo int,
	canalAplicarOperacion chan AplicaOperacion) *NodoRaft {
	nr := &NodoRaft{}
	nr.initNodo(nodos, yo, canalAplicarOperacion)
	nr.initLogger(nodos, yo)

	nr.Logger.Printf("NuevoNodo: Maquina creada\n")

	go nr.actEstado()

	return nr
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
func electionTimeout() time.Duration {
	rand.Seed(time.Now().UnixNano())
	return (time.Duration(rand.Intn(500-100)+100) * time.Millisecond)
}

// Función para definir comportamiento del estado SEGUIDOR
func (nr *NodoRaft) estadoSeguidor() {
	nr.Logger.Printf("SEGUIDOR: Pongo en marcha timeout de elecciones\n")
	temporizador := time.NewTicker(electionTimeout())
	defer temporizador.Stop()

	for {
		select {
		case <-nr.LatidoCh:
			// Si me llega appendentry, sigo siendo seguidor
			nr.Logger.Printf("SEGUIDOR: Latido recibido, reinicio timeout\n")
			temporizador.Reset(electionTimeout())
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
				// gorutina escribira
				MayoriaCh <- true
			}
			nr.Mux.Unlock()

		} else if replyPV.Term > nr.CurrentTerm {
			nr.Logger.Printf("CANDIDATO-GR: Voto rechazado, su mandato %d es mayor que el mio %d\n",
				replyPV.Term, nr.CurrentTerm)
			nr.CurrentTerm = replyPV.Term
			nr.CurrentRol = SEGUIDOR
			nr.VotedFor = -1
			nr.LatidoCh <- true
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

	peticionVoto := &ArgsPeticionVoto{Term: nr.CurrentTerm, CandidateId: nr.Yo,
		LastLogIndex: 0, LastLogTerm: 0}

	for i := range nr.Nodos {
		if i != nr.Yo {
			go nr.tratarReplyPeticionVoto(i, peticionVoto, MayoriaCh)
		}
	}
}

// Función para definir comportamiento del estado CANDIDATO
func (nr *NodoRaft) estadoCandidato() {
	nr.Logger.Printf("CANDIDATO: Pongo en marcha timeout de elecciones\n")
	temporizador := time.NewTicker(electionTimeout())
	defer temporizador.Stop()

	MayoriaCh := make(chan bool, 1)

	nr.Logger.Printf("CANDIDATO: Pido votos a los demas nodos\n")
	nr.broadcastPeticionVoto(MayoriaCh)

	select {
	case <-nr.LatidoCh:
		nr.Logger.Printf("CANDIDATO: Latido recibido, dejo candidatura\n")
		return
	case <-MayoriaCh:
		nr.Logger.Printf("CANDIDATO: Recibida mayoria de votos, comenzar liderazgo\n")
		return
	case <-temporizador.C:
		nr.Logger.Printf("CANDIDATO: Timeout de elecciones agotado, reiniciar elecciones\n")
		nr.CurrentRol = CANDIDATO
		return
	}
}

// Función para tratar respuesta de peticiones de replicación del LIDER
func (nr *NodoRaft) tratarReplyReplicateLog(nodo int, args *ArgAppendEntries) {
	var ReplyRL Results
	if nr.enviarReplicateLog(nodo, args, &ReplyRL) {
		nr.Logger.Printf("LIDER-GR: Recibido reply de AppendEntry\n")
		if ReplyRL.Term == nr.CurrentTerm && nr.CurrentRol == LIDER {
			// Como en esta practica se asume que no hay errores, no hace falta
			// verificar cuantos han coseguido replicar mi log, ni insistir en
			// caso de fallo
			if ReplyRL.Success {
				nr.Logger.Printf("LIDER-GR: Mi AppendEntry ha sido aceptado\n")
				return
			}
			nr.Logger.Printf("LIDER-GR: Mi AppendEntry ha sido rechazado\n")
			return
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

// Función para repartir latidos a seguidores indicando presencia de LIDER
func (nr *NodoRaft) broadcastLatido() {
	if nr.CurrentRol == LIDER {

		logRequest := &ArgAppendEntries{Term: nr.CurrentTerm,
			LeaderId: nr.IdLider, PrevLogIndex: nr.MatchIndex[nr.Yo],
			PrevLogTerm: nr.Log[nr.MatchIndex[nr.Yo]].Term,
			Entries:     []ElementoLog{}, LeaderCommit: nr.CommitIndex}

		for i := range nr.Nodos {
			if i != nr.Yo {
				go nr.tratarReplyReplicateLog(i, logRequest)
			}
		}
	}
}

// Función para repartir operaciones propuestas por el cliente
func (nr *NodoRaft) broadcastOperacion(operacion TipoOperacion) {
	if nr.CurrentRol == LIDER {
		// Append al log de la entrada solicitada
		nr.Logger.Printf("LIDER: Concateno operacion a mi registro\n")
		nr.Mux.Lock()
		entrada := ElementoLog{Operacion: operacion, Term: nr.CurrentTerm}
		nr.encolarOperacion(entrada)

		OpActual := ElementoLog{Operacion: operacion, Term: nr.CurrentTerm}
		entry := append([]ElementoLog{}, OpActual)

		logRequest := &ArgAppendEntries{Term: nr.CurrentTerm,
			LeaderId: nr.IdLider, PrevLogIndex: nr.MatchIndex[nr.Yo],
			PrevLogTerm: nr.Log[nr.MatchIndex[nr.Yo]].Term,
			Entries:     entry, LeaderCommit: nr.CommitIndex}
		nr.Mux.Unlock()

		for i := range nr.Nodos {
			if i != nr.Yo {
				go nr.tratarReplyReplicateLog(i, logRequest)
			}
		}
	}
	// else, debería hacer fowarding de petición a lider correcto,
	// no es necesario para esta práctica
}

// Función para definir comportamiento del estado LIDER
func (nr *NodoRaft) estadoLider() {
	nr.Logger.Printf("LIDER: Inicio timeout de latido a mandar\n")
	temporizador := time.NewTicker(50 * time.Millisecond)
	defer temporizador.Stop()

	nr.Logger.Printf("LIDER: Mando latido inicial para reconocimiento\n")
	nr.broadcastLatido()

	for {
		select {
		case <-temporizador.C:
			nr.Logger.Printf("LIDER: Mando latido periodico y lo reprogramo\n")
			nr.broadcastLatido()
			temporizador.Reset(50 * time.Millisecond)
			continue
		case <-nr.LatidoCh:
			nr.Logger.Printf("LIDER: Recibido AppendEntry mayor, dejo liderazgo\n")
			return
		case operacion := <-nr.SClientch:
			nr.Logger.Printf("LIDER: Tratando petición de cliente\n")
			nr.broadcastOperacion(operacion)
			// Como no hay errores nada más mandar, asumimos que todos replican
			// correctamente nuestro registro
			nr.Logger.Printf("LIDER: Petición de replica mandada, asumo comprometido\n")
			nr.CommitIndex = nr.MatchIndex[nr.Yo]
			temporizador.Reset(50 * time.Millisecond)
			nr.RClientch <- true
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
func (nr *NodoRaft) obtenerEstado() (int, int, bool, int) {
	nr.Mux.Lock()
	var yo int = nr.Yo
	var mandato int = nr.CurrentTerm
	var esLider bool = nr.CurrentRol == LIDER
	var idLider int = nr.IdLider
	nr.Mux.Unlock()

	return yo, mandato, esLider, idLider
}

// Función auxiliar para encolar operación a registro de entradas
func (nr *NodoRaft) encolarOperacion(entrada ElementoLog) {
	nr.Log = append(nr.Log, entrada)
	nr.NextIndex[nr.Yo]++
	nr.MatchIndex[nr.Yo]++
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
		indice = nr.NextIndex[nr.Yo]
		mandato = nr.CurrentTerm
		idLider = nr.IdLider
		nr.Mux.Unlock()
		nr.SClientch <- operacion
		<-nr.RClientch
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
	defer nr.para()
	return nil
}

type EstadoParcial struct {
	Mandato int
	EsLider bool
	IdLider int
}

type EstadoRemoto struct {
	IdNodo int
	EstadoParcial
}

func (nr *NodoRaft) ObtenerEstadoNodo(args Vacio, reply *EstadoRemoto) error {
	reply.IdNodo, reply.Mandato, reply.EsLider, reply.IdLider = nr.obtenerEstado()
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
//
// Structura de ejemplo de argumentos de RPC PedirVoto.
//
// Recordar
// -----------
// Nombres de campos deben comenzar con letra mayuscula !
type ArgsPeticionVoto struct {
	Term         int
	CandidateId  int
	LastLogIndex int
	LastLogTerm  int
}

// Structura de ejemplo de respuesta de RPC PedirVoto,
//
// Recordar
// -----------
// Nombres de campos deben comenzar con letra mayuscula !
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

	if peticion.Term > nr.CurrentTerm {
		nr.Logger.Printf("RPC - PedirVoto: Su mandato %d > mi mandato %d, paso a ser seguidor\n",
			peticion.Term, nr.CurrentTerm)
		nr.CurrentTerm = peticion.Term
		nr.CurrentRol = SEGUIDOR
		nr.VotedFor = -1
		nr.LatidoCh <- true
	}

	// Faltaría poner verificación de logs

	if peticion.Term == nr.CurrentTerm &&
		(nr.VotedFor == -1 || nr.VotedFor == peticion.CandidateId) {
		nr.Logger.Printf("RPC - PedirVoto: Concedo voto a %d\n",
			peticion.CandidateId)
		nr.VotedFor = peticion.CandidateId
		reply.Term = nr.CurrentTerm
		reply.VoteGranted = true
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
	Entries      []ElementoLog
	LeaderCommit int
}

type Results struct {
	Term    int
	Success bool
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
		nr.Logger.Printf("RPC - AppendEntries: Procedo a actualizar mi registro\n")
		nr.Log = append(nr.Log, args.Entries...)
		nr.CurrentRol = SEGUIDOR
		nr.IdLider = args.LeaderId
		results.Term = nr.CurrentTerm
		results.Success = true
		nr.LatidoCh <- true
	} else {
		nr.Logger.Printf("RPC - AppendEntries: Denegado, mandato es menor\n")
		results.Term = nr.CurrentTerm
		results.Success = false
	}

	// En esta práctica no hace falta verificar errores con las consistencias
	// de los logs, ya que se asume que no hay errores, por tanto valdría con
	// solo concatenar entry recibida siempre

	/*logOK := (len(nr.Log)-1 >= args.PrevLogIndex) &&
		(nr.Log[args.PrevLogIndex].Term == args.PrevLogTerm)

	if args.Term == nr.CurrentTerm && logOK {
		nr.Logger.Printf("RPC - AppendEntries: Replico entradas de lider válido\n")
		nr.Log = append(nr.Log, args.Entries...)
		results.Term = nr.CurrentTerm
		results.Success = true
		nr.LatidoCh <- true
	} else {
		results.Term = nr.CurrentTerm
		results.Success = false
	}*/

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
