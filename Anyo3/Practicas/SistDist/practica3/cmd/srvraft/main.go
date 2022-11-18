package main

import (
	//"errors"
	"bufio"
	"fmt"
	"time"

	//"log"

	"os"
	"raft/internal/comun/check"
	"raft/internal/comun/rpctimeout"
	"raft/internal/raft"
	"strconv"
	//"time"
)

// Devuelve los pares direccion:puerto de un fichero dado en forma de nodos
// para un sistema Raft. No se asegura que el formato sea correcto.
func leerNodos(file string) []rpctimeout.HostPort {

	//Abre el archivo
	fd, err := os.Open(file)
	check.CheckError2(err, "leerNodos()", "os.Open()")

	// Lo divide por línea
	scanner := bufio.NewScanner(fd)
	scanner.Split(bufio.ScanLines)

	// Añade nodos al array
	var nodos []rpctimeout.HostPort
	for scanner.Scan() {
		nodos = append(nodos, rpctimeout.HostPort(scanner.Text()))
	}

	return nodos
}

func main() {

	// Lee los nodos de un fichero [arg 1]
	file := os.Args[1]
	nodos := leerNodos(file)

	// Obtiene el índice del nodo [arg 2]
	yo, err := strconv.Atoi(os.Args[2])
	check.CheckError2(err, "main()", "strconv.Atoi()")

	//Inicializa el servidor Raft
	nr := raft.NuevoNodo(nodos, yo, make(chan raft.AplicaOperacion))

	for i := 0; i < 10; i++ {
		select {
		case <-time.After(1 * time.Second):
			fmt.Println(nr.ObtenerEstado())
		}
	}

	nr.Para()
}
