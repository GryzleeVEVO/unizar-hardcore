/*
	Sistemas Distribuidos
	Practica 1

	Fichero: cmd/server-master/main.go

	Autores:
		Dorian Boleslaw Wozniak (817570@unizar.es)
		Cristian Andrei Selivanov Dobrisan (816456@unizar.es)

	Descripción: Versión del servidor de la práctica 1 que lanza mediante una
					shell script una serie de procesos worker, y lanza tantas
					Gorutinas handleRequest() como workers. Cada una se conecta
					a su worker asociado, y le envía una petición al recibirla
					de un cliente
*/

package main

import (
	"bufio"
	"encoding/gob"
	"log"
	"net"
	"os"
	"os/exec"
	"practica1/com"
	"time"
)

const (
	CONN_TYPE = "tcp"
)

/******************************************************************************/

// Establece conexión con un worker. A coontinucación, obtiene la petición de un
// cliente, la reenvía al master, y devuelve su resultado
func handleRequest(i int, endpoint string, ch chan net.Conn) {
	// Inicia conexion con worker
	connW, err := net.Dial(CONN_TYPE, endpoint)
	com.CheckError(err)

	log.Println("[INFO] [P", i, "] Conexión con worker establecida")

	encoderW := gob.NewEncoder(connW)
	decoderW := gob.NewDecoder(connW)

	for {
		// Obtiene siguiente tarea
		conn := <-ch
		log.Println("[INFO] [P", i, "] Trata siguiente petición")

		// Codificadores/decodificadores
		encoder := gob.NewEncoder(conn)
		decoder := gob.NewDecoder(conn)

		// Lee petición
		var request com.Request
		err = decoder.Decode(&request)
		com.CheckError(err)
		log.Println("[INFO] [P", i, "] [ID ", request.Id, "] Recibida petición")

		// Reenvía petición a worker
		encoderW.Encode(request)
		log.Println("[INFO] [P", i, "] [ID ", request.Id, "] Enviando a Worker")

		// Espera respuesta del worker
		var reply com.Reply
		err = decoderW.Decode(&reply)
		com.CheckError(err)
		log.Println("[INFO] [P", i, "] [ID ", request.Id, "] Recibido Worker")

		//connW.Close()

		// Reenvía respuesta
		encoder.Encode(reply)
		com.CheckError(err)
		log.Println("[INFO] [P", i, "] [ID ", reply.Id, "] Enviada respuesta")

		conn.Close()
	}
}

func main() {

	// Obtiene IP y puerto donde escuchar en sus argumentos
	endpoint := os.Args[1]

	// Escucha nuevas peticiones
	listener, err := net.Listen(CONN_TYPE, endpoint)
	com.CheckError(err)

	log.SetFlags(log.Lshortfile | log.Lmicroseconds)
	log.Println("[INFO] Escuchando en ", endpoint)

	// Ejecuta script para lanzar procesos remotos
	err = exec.Command(".././lanzar_master_worker.sh").Start()

	time.Sleep(time.Duration(1000) * time.Millisecond)

	com.CheckError(err)

	// Obtiene lista de workers con los que conectar
	f, err := os.Open("../workers")
	com.CheckError(err)
	FScan := bufio.NewScanner(f)
	FScan.Split(bufio.ScanLines)
	var lines []string
	for FScan.Scan() {
		lines = append(lines, FScan.Text())
	}
	f.Close()

	// Canal donde se encolaran las peticiones
	ch := make(chan net.Conn)

	// Lanza gorutinas con un worker asociado por gorutina
	i := 1
	for _, endpoint := range lines {
		go handleRequest(i, endpoint, ch)
		i++
	}

	log.Println("[INFO] Workers conectados, esperando cliente...")

	for {
		// Recibe petición de cliente
		conn, err := listener.Accept()
		com.CheckError(err)

		log.Println("[INFO] Conexión de cliente aceptada")

		// Añade tarea a cola
		ch <- conn
	}
}
