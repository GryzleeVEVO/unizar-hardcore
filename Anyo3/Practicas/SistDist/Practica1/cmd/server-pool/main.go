/*
	Sistemas Distribuidos
	Practica 1

	Fichero: cmd/server-pool/main.go

	Autores:
		Dorian Boleslaw Wozniak (817570@unizar.es)
		Cristian Andrei Selivanov Dobrisan (816456@unizar.es)

	Descripción: Versión del servidor de la práctica 1 que lanza un número
				determinado de Gorutinas handleRequest(), que reciben conexiones
				nuevas a través de un canal
*/

package main

import (
	"encoding/gob"
	"log"
	"net"
	"os"
	"practica1/com"
)

const (
	CONN_TYPE = "tcp"
)

/******************************************************************************/
//							CÁLCULO DE PRIMOS

// PRE: verdad = !foundDivisor
// POST: IsPrime devuelve verdad si n es primo y falso en caso contrario
func isPrime(n int) (foundDivisor bool) {
	foundDivisor = false
	for i := 2; (i < n) && !foundDivisor; i++ {
		foundDivisor = (n%i == 0)
	}
	return !foundDivisor
}

// PRE: interval.A < interval.B
// POST: FindPrimes devuelve todos los números primos comprendidos en el
// intervalo [interval.A, interval.B]
func findPrimes(interval com.TPInterval) (primes []int) {
	for i := interval.Min; i <= interval.Max; i++ {
		if isPrime(i) {
			primes = append(primes, i)
		}
	}
	return primes
}

/******************************************************************************/

// Obtiene la petición de un cliente, la realiza y devuelve su resultado
func handleRequest(i int, ch chan net.Conn) {
	for {
		// Obtiene siguiente tarea
		conn := <-ch
		log.Println("[INFO] [P", i, "] Trata siguiente petición")

		// Codificadores/decodificadores
		encoder := gob.NewEncoder(conn)
		decoder := gob.NewDecoder(conn)

		// Lee petición
		var request com.Request
		err := decoder.Decode(&request)
		com.CheckError(err)
		log.Println("[INFO] [P", i, "] [ID ", request.Id, "] Recibida petición")

		// Lanza tarea
		primes := findPrimes(request.Interval)

		// Envía respuesta
		reply := com.Reply{Id: request.Id, Primes: primes}
		encoder.Encode(reply)
		com.CheckError(err)
		log.Println("[INFO] [P", i, "] [ID ", reply.Id, "] Enviada respuesta")

		conn.Close()
	}
}

func main() {

	endpoint := os.Args[1]

	// Escucha nuevas peticiones
	listener, err := net.Listen(CONN_TYPE, endpoint)
	com.CheckError(err)

	log.SetFlags(log.Lshortfile | log.Lmicroseconds)
	log.Println("[INFO] Escuchando en ", endpoint)

	// Lanza pool de gorutinas
	ch := make(chan net.Conn)
	for i := 0; i < 3; i++ {
		go handleRequest(i, ch)
	}

	for {
		// Recibe petición de cliente
		conn, err := listener.Accept()
		com.CheckError(err)

		log.Println("[INFO] Conexión de cliente aceptada")

		// Añade tarea a cola
		ch <- conn
	}
}
