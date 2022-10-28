/*
Sistemas Distribuidos
Práctica 2

Autores:
Cristian Andrei Selivanov Dobrisan (816456@unizar.es)
Dorian Boleslaw Wozniak (817570@unizar.es)

Descripción:

Implementación del algorimo Ricart-Agrawala para un sistema distribuido de
lectores y escritores. El alogirmo es una versión generalizada que utiliza
vectores vectoriales y matrices de exclusión. Incluye además procedimientos
específicos para tratar el problema de lectores-escritores distribuido
*/

package ra

import (
	"bufio"
	"log"
	"os"
	"practica2/ms"
	"sync"
)

/* TIPOS DE PROCESOS QUE ACCEDERAN A SC */

const (
	READER = 0
	WRITER = 1
)

/* TIPOS DE MENSAJES COMPARTIDOS ENTRE NODOS */

// Mensaje para pedir acceso a SC
type Request struct {
	Clocks []int
	Pid    int
	Type   int
}

// Mensaje para ceder acceso a SC
type Reply struct {
	Pid int
}

// Mensaje para avisar actualización de fichero local
type Update struct {
	Pid      int
	Fragment string
}

// Mensaje para confirmar cambio de fichero local
type UpACK struct {
	Pid int
}

// Mensaje para indicar fin de peticiones de acceso a SC
type Finished struct {
	Pid int
}

/* VARIABLES COMPARTIDAS ENTRE PROCESOS LOCALES DE UN NODO */

type RASharedDB struct {
	Me int // Identificador único de nodo
	N  int // Nº de nodos en red

	HigSeqNum []int // Mayor reloj vectorial visto de cualquier REQUEST recibido
	OurSeqNum []int // Reloj vectorial de REQUEST originados desde este nodo
	OutRepCnt int   // Nº de mensajes REPLY esperados
	OutUpdCnt int   // Nº de mensajes UpACK esperados
	NodesLeft int   // Nº de nodos que siguen queriendo acceder a SC
	NodeType  int   // Tipo de proceso asociado al nodo

	ReqCS   bool       // TRUE, si solo si el nodo quiere acceder a SC
	RepDefd []bool     // RepDefd[J] es TRUE, si el nodo J ha sido pospuesto
	Exclude [2][2]bool // Vector de operaciones excluyentes

	File    string            // Nombre del fichero local del nodo
	Ms      *ms.MessageSystem // Mailbox del nodo
	Done    chan bool         // Canal para comunicar fin de vida de nodo
	Chrep   chan bool         // Canal para comunicar llegada de REPLY
	Chackup chan bool         // Canal para comunicar llegada de UpACK
	Chfin   chan bool         // Canal para comunicar llegada de
	Mutex   sync.Mutex        // Mutex para proteger concurrencia sobre vars
}

// Pre : "fileName" es un fichero existente, que contiene IP:Puertos de nodos
// Post: Devulve nº de líneas contenidas en el fichero, coinciden con nº nodos
func calculateNodes(fileName string) int {
	file, err := os.Open(fileName)
	ms.CheckError(err)

	FScan := bufio.NewScanner(file)
	FScan.Split(bufio.ScanLines)

	N := 0
	for FScan.Scan() {
		N++
	}

	file.Close()
	return N
}

// Pre : Ficheros "usersFile" y "procFile" existen
// Post: Pone en marcha procesos concurrentes a la escucha del Mailbox creado y
// devuelve la estructura de datos compartidos entre dichos procesos y el nodo
func New(me int, typeProc int, usersFile string, procFile string) *RASharedDB {

	// Indicamos tipos de mensajes que se compartiran entre nodos
	messageTypes := []ms.Message{Request{}, Reply{}, Update{},
		Finished{}, UpACK{}}

	msgs := ms.New(me, usersFile, messageTypes)
	exc := [2][2]bool{{false, true}, {true, true}}
	N := calculateNodes(usersFile)

	ra := RASharedDB{me, N, make([]int, N), make([]int, N), 0, 0, N - 1,
		typeProc, false, make([]bool, N), exc, procFile, &msgs, make(chan bool),
		make(chan bool), make(chan bool), make(chan bool, N), sync.Mutex{}}

	// Iniciamos proceso gestor de mensajes entrantes
	go ra.ManageMailbox()

	return &ra
}

// Pre : True
// Post: Actualiza reloj vectorial con el más avanzado y suma evento del nodo
func (ra *RASharedDB) updateClksPre() {
	for i, val := range ra.HigSeqNum {
		if (ra.Me - 1) == i {
			ra.OurSeqNum[i] = val + 1
		} else {
			ra.OurSeqNum[i] = val
		}
	}
}

// Pre : True
// Post: Realiza  el  PreProtocol  para el  algoritmo de
// Ricart-Agrawala Generalizado
func (ra *RASharedDB) PreProtocol() {
	// Petición de entrada a SC
	log.Printf("[%d] Mando preticiones para acceder a SC\n", ra.Me)

	ra.Mutex.Lock()
	ra.ReqCS = true
	ra.updateClksPre()
	ra.Mutex.Unlock()

	ra.OutRepCnt = ra.N - 1

	for j := 0; j < ra.N; j++ {
		if j+1 != ra.Me {
			// Enviamos REQUEST a cada uno de los nodos en red
			ra.Ms.Send(j+1, Request{Clocks: ra.OurSeqNum, Pid: ra.Me,
				Type: ra.NodeType})
		}
	}

	// Se espera a recibir los REPLY de cada uno de los nodos en red
	for ra.OutRepCnt != 0 {
		<-ra.Chrep
	}

	// A partir de este punto ya se ha accedido a SC
	log.Printf("[%d] Accedo a SC, siendo tipo %d\n", ra.Me, ra.NodeType)
}

// Pre : True
// Post: Realiza  el  PostProtocol  para el  algoritmo de
// Ricart-Agrawala Generalizado
func (ra *RASharedDB) PostProtocol() {
	// Salir de SC y avisar a procesos postergados
	ra.ReqCS = false

	for j := 0; j < ra.N; j++ {
		if ra.RepDefd[j] {
			ra.RepDefd[j] = false
			ra.Ms.Send(j+1, Reply{Pid: ra.Me})
		}
	}
}

// Pre : El nodo escritor ha accedido a SC y ha realizado una escritura
// Post: Se habran actualizado los ficheros locales todos los nodos de la red
func (ra *RASharedDB) UpdateNodeFiles(frag string) {
	ra.OutUpdCnt = ra.N - 1

	log.Printf("[%d] Aviso con UPDATE a todos los procesos\n", ra.Me)
	for j := 0; j < ra.N; j++ {
		if j+1 != ra.Me {
			// Se envia el fragmento a añadir en el documento
			ra.Ms.Send(j+1, Update{Pid: ra.Me, Fragment: frag})
		}
	}

	for ra.OutUpdCnt != 0 {
		<-ra.Chackup
	}

	log.Printf("[%d] Todos los procesos han actualizado sus ficheros\n", ra.Me)
}

// Función de proceso que se encarga de reexpedir mensajes a procesos oportunos
func (ra *RASharedDB) ManageMailbox() {
	// Por cada tipo de mensaje compartido, se crea un proceso que lo trata
	REQUESTch := make(chan Request)
	go ra.RequestProtocol(REQUESTch)

	REPLYch := make(chan Reply)
	go ra.ReplyProtocol(REPLYch)

	UPDATEch := make(chan Update)
	go ra.UpdateProtocol(UPDATEch)

	FINISHEDch := make(chan Finished)
	go ra.EndProtocol(FINISHEDch)

	UPDATEACKch := make(chan UpACK)
	go ra.ACKUpdateProtocol(UPDATEACKch)

	// Se procede a repartir mensajes entrantes, entre los distintos procesos
	// hasta que se haya concluido con la comunicación
	for {
		select {
		case <-ra.Done:
			return
		default:
			switch msg := ra.Ms.Receive().(type) {
			case Request:
				REQUESTch <- msg
			case Reply:
				REPLYch <- msg
			case Update:
				UPDATEch <- msg
			case UpACK:
				UPDATEACKch <- msg
			case Finished:
				FINISHEDch <- msg
			}
		}
	}
}

// Pre : "v" y "w" son vectores de la misma longitud
// Post: True, si solo si todos los elementos del reloj vectorial "v" son
// iguales que los del reloj vectorial "w"
func equalsCLKs(v []int, w []int) bool {
	aux := true
	for i, val := range v {
		if val != w[i] {
			aux = false
			break
		}
	}
	return aux
}

// Pre : "v" y "w" son vectores de la misma longitud
// Post: True, si solo si todos los elementos del reloj vectorial "v" son
// menores/iguales que los del reloj vectorial "w"
func lowerEqualCLKs(v []int, w []int) bool {
	aux := true
	for i, val := range v {
		if val > w[i] {
			aux = false
			break
		}
	}
	return aux
}

// Pre : "v" y "w" son vectores de la misma longitud
// Post: True, si solo si el reloj vectorial "v" es estricto menor que el reloj
// vectorial "w"
func lowerThanCLKs(v []int, w []int) bool {
	return lowerEqualCLKs(v, w) && !equalsCLKs(v, w)
}

// Pre : True
// Post: Actualiza reloj vectorial máximo "HigSeqNum" con los valores mayores
// del REQUEST recibido
func (ra *RASharedDB) updateClksReq(clks []int) {
	for i, val := range ra.HigSeqNum {
		if val < clks[i] {
			ra.HigSeqNum[i] = clks[i]
		}
	}
}

// Función de proceso que se encarga de tratar las peticiones REQUEST
func (ra *RASharedDB) RequestProtocol(Reqch chan Request) {
	for {
		req := <-Reqch
		log.Printf("[%d] REQUEST de %d recibido\n", ra.Me, req.Pid)

		ra.Mutex.Lock()
		// Actualizo mi reloj vectorial de máximos
		ra.updateClksReq(req.Clocks)

		// Verifico si petición REQUEST ha de ser postergada o no
		defer_it := ra.ReqCS && ra.Exclude[ra.NodeType][req.Type] &&
			(lowerThanCLKs(ra.OurSeqNum, req.Clocks) ||
				(!(lowerThanCLKs(req.Clocks, ra.OurSeqNum)) && req.Pid > ra.Me))

		ra.Mutex.Unlock()

		if defer_it {
			log.Printf("[%d] REQUEST de %d postergado\n", ra.Me, req.Pid)
			ra.RepDefd[req.Pid-1] = true
		} else {
			log.Printf("[%d] REQUEST de %d aceptado\n", ra.Me, req.Pid)
			ra.Ms.Send(req.Pid, Reply{Pid: ra.Me})
		}
	}
}

// Función de proceso que se encarga de tratar las peticiones REPLY
func (ra *RASharedDB) ReplyProtocol(Repch chan Reply) {
	for {
		rep := <-Repch
		log.Printf("[%d] REPLY de %d aceptado\n", ra.Me, rep.Pid)
		ra.OutRepCnt--
		ra.Chrep <- true
	}
}

// Pre : True
// Post: Actualiza contenido de "fileName" escribiendo al final el string "frag"
func updateFile(fileName string, frag string) {
	file, err := os.OpenFile(fileName, os.O_CREATE|os.O_APPEND|os.O_RDWR, 0644)
	ms.CheckError(err)
	_, err = file.WriteString(frag)
	ms.CheckError(err)
	file.Close()
}

// Función de proceso que se encarga de tratar las peticiones UPDATE
func (ra *RASharedDB) UpdateProtocol(Updch chan Update) {
	// TODO completar

	for {
		upd := <-Updch
		log.Printf("[%d] UPDATE de %d aceptado", ra.Me, upd.Pid)
		updateFile(ra.File, upd.Fragment)
		ra.Ms.Send(upd.Pid, UpACK{Pid: ra.Me})
	}
}

// Función de proceso que se encarga de tratar las peticiones UpACK
func (ra *RASharedDB) ACKUpdateProtocol(AUpdch chan UpACK) {
	for {
		ackupd := <-AUpdch
		log.Printf("[%d] ACK UPDATE de %d aceptado", ra.Me, ackupd.Pid)
		ra.OutUpdCnt--
		ra.Chackup <- true
	}
}

// Función de proceso que se encarga de tratar las peticiones FINISHED
func (ra *RASharedDB) EndProtocol(Finch chan Finished) {
	for {
		eoc := <-Finch
		ra.NodesLeft--
		log.Printf("[%d] Nodo %d concluye, quedan %d nodos que acceden a SC\n",
			ra.Me, eoc.Pid, ra.NodesLeft)

		ra.Chfin <- true
	}
}

// Pre : True
// Post: Una vez ejecutada, se cierran comunicaciones del nodo y se asegura que
// todos los demás nodos de la red han concluido con su trabajo
func (ra *RASharedDB) Stop() {
	log.Printf("[%d] He terminado mi trabajo, dejo de acceder a SC\n", ra.Me)

	// Se avisa del final a los demás nodos
	for j := 0; j < ra.N; j++ {
		if j+1 != ra.Me {
			ra.Ms.Send(j+1, Finished{Pid: ra.Me})
		}
	}

	// Bloqueo de nodo hasta que todos terminen, sigue atendiendo REQUEST para
	// dejarles acceder a SC
	for ra.NodesLeft != 0 {
		<-ra.Chfin
	}

	ra.Ms.Stop()
	ra.Done <- true
	log.Printf("[%d] TERMINADO", ra.Me)
}
