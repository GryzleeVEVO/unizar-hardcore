/*



 */

package main

import (
	"encoding/gob"
	"fmt"
	"net"
	"os"
	"practica1/com"
)

const (
	CONN_TYPE = "tcp"
	CONN_HOST = "127.0.0.1"
	CONN_PORT = "30000"
)

/*
	CÁLCULO DE PRIMOS
*/

/*
	PRE: ---
	POST: Verdad solo si n es primo
*/

func IsPrime(n int) (foundDivisor bool) {

	foundDivisor = false

	for i := 2; (i < n) && !foundDivisor; i++ {
		foundDivisor = (n%i == 0)
	}

	return !foundDivisor
}

/*
	PRE: interval.A < interval.B
 	POST: Números primos comprendidos en [interval.A, interval.B]
*/

func FindPrimes(interval com.TPInterval) (primes []int) {
	for i := interval.A; i <= interval.B; i++ {
		if IsPrime(i) {
			primes = append(primes, i)
		}
	}
	return primes
}

/*
	Comprueba errores
*/

func checkError(err error) {
	if err != nil {
		fmt.Fprintf(os.Stderr, "Fatal error: %s", err.Error())
		os.Exit(1)
	} else {
		fmt.Fprintf(os.Stderr, "Ok\n")
	}
}

func handleRequest(conn net.Conn) {

	defer conn.Close()

	/* Crea codificadores/decodificadores para paso de mensajes*/
	encoder := gob.NewEncoder(conn)
	decoder := gob.NewDecoder(conn)

	/* Lee petición */
	var request com.Request
	err := decoder.Decode(&request)

	fmt.Fprintf(os.Stderr, "[Decode] Petición: ID %d\n", request.Id)

	/* Lanza tarea */
	primes := FindPrimes(request.Interval)

	/* Envía respuesta */
	reply := com.Reply{request.Id, primes}
	encoder.Encode(reply)
	checkError(err)

	fmt.Fprintf(os.Stderr, "[Encode] Respuesta: ID %d\n", reply.Id)
}

func main() {

	/* Abre escucha en puerto */
	listener, err := net.Listen(CONN_TYPE, CONN_HOST+":"+CONN_PORT)
	checkError(err)

	fmt.Fprintf(os.Stderr, "[Listen] Escuchando en %s\n", CONN_HOST+":"+CONN_PORT)

	for {
		/* Recibe petición de cliente */
		conn, err := listener.Accept()
		checkError(err)

		handleRequest(conn)
	}

}
