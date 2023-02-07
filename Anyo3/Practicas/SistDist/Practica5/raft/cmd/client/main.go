/*
	Cliente interactivo para enviar solicitudes a un grupo de nodos Raft
*/

package main

import (
	"bufio"
	"fmt"
	"os"
	"raft/internal/comun/rpctimeout"
	"raft/pkg/cltraft"
	"strconv"
	"strings"
)

func main() {

	// Obtiene nodos Raft disponibles
	var nodos []rpctimeout.HostPort
	for _, endPoint := range os.Args[1:] {
		nodos = append(nodos, rpctimeout.HostPort(endPoint))
	}

	// Prompt
	scanner := bufio.NewScanner(os.Stdin)

	for {
		fmt.Println(
			"Indique una acción {leer | escribir | estado | parar}: ")
		scanner.Scan()
		read := scanner.Text()
		line := strings.Fields(read)

		switch line[0] {
		case "leer":
			if len(line) != 2 {
				fmt.Println("Sintáxis: leer clave")
			} else {
				fmt.Println(cltraft.SometerOperacionRaft(nodos, "leer",
					line[1], ""))
			}
		case "escribir":
			if len(line) < 3 {
				fmt.Println("Sintáxis: escribir clave valor")
			} else {
				fmt.Println(cltraft.SometerOperacionRaft(nodos, "escribir",
					line[1], strings.Join(line[2:], " ")))
			}
		case "estado":
			if (len(line)) != 2 {
				fmt.Println("Sintáxis: estado nodo")
			} else {
				nodo, err := strconv.Atoi(line[1])
				if err != nil || nodo >= len(nodos) {
					fmt.Println("Nodo no válido")
				} else {
					fmt.Println(cltraft.ObtenerEstadoNodo(nodos, nodo))
				}
			}
		case "parar":
			if len(line) != 2 {
				fmt.Println("Sintáxis: parar nodo")
			} else {
				nodo, err := strconv.Atoi(line[1])
				if err != nil || nodo >= len(nodos) {
					fmt.Println("Nodo no válido")
				} else {
					cltraft.ParaNodo(nodos, nodo)
				}
			}
		default:
			fmt.Println("Acción incorrecta")
		}
		fmt.Println()
	}
}
