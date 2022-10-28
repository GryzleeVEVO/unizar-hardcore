/*
Sistemas Distribuidos
Práctica 2

Autores:
Cristian Andrei Selivanov Dobrisan (816456@unizar.es)
Dorian Boleslaw Wozniak (817570@unizar.es)

Descripción:

Implementación de un programa que actúa como lector en un sistema
lector-escritor distribuido. El programa trata de obtener permiso del resto
para entrar en sección crítica. Una vez dentro, lee una copia local de un
fichero compartido por todos los nodos de la red. Una vez haya recibido
confirrmación del resto, devuelve el mutex y da responde a peticiones menos
prioritarias postergadas
*/

package main

import (
	"log"
	"os"
	"practica2/ms"
	"practica2/ra"
	"strconv"
	"time"
)

// Pre : True
// Post: En caso de no existir el fichero "fileName" se creara
func CrearFichero(fileName string) {
	file, err := os.OpenFile(fileName, os.O_CREATE|os.O_APPEND|os.O_RDWR, 0644)
	ms.CheckError(err)
	file.Close()
}

// Pre : El fichero "fileName" ha de existir
// Post: Se leen y devuelven contenidos de fichero "fileName"
func LeerFichero(fileName string) string {
	dat, err := os.ReadFile(fileName)
	ms.CheckError(err)
	return string(dat)
}

func main() {
	usersFile := os.Args[1]
	pid, _ := strconv.Atoi(os.Args[2])
	procFile := "fichero_" + strconv.Itoa(pid)

	CrearFichero(procFile)

	ra := ra.New(pid, ra.READER, usersFile, procFile)

	log.Printf("\t[%d] Esperamos 20 segundos a que todos inicializen\n", pid)
	time.Sleep(20 * time.Second)

	for i := 0; i < 10; i++ {
		time.Sleep(time.Duration(300) * time.Millisecond)
		ra.PreProtocol()
		// Entro a SC
		LeerFichero(procFile)
		// Salgo de SC
		ra.PostProtocol()
	}
	ra.Stop()
}
