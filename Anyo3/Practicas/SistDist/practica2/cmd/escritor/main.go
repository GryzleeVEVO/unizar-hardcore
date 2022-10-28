/*
Sistemas Distribuidos
Práctica 2

Autores:
Cristian Andrei Selivanov Dobrisan (816456@unizar.es)
Dorian Boleslaw Wozniak (817570@unizar.es)

Descripción:

Implementación de un programa que actúa como escritor en un sistema
lector-escritor distribuido. El programa trata de obtener permiso del resto
para entrar en sección crítica. Una vez dentro, añade un fragmento de texto
nuevo a su copia local del recurso compartido y notifica al resto de nodos
la modificación. Una vez haya recibido confirrmación del resto, devuelve el
mutex y da responde a peticiones menos prioritarias postergadas
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
// Post: Si el fichero "fileName" existe, escribira al final el string "frag",
// en caso contrario creara un fichero con el contenido de dicho string
func EscribirFichero(frag string, fileName string) {
	file, err := os.OpenFile(fileName, os.O_CREATE|os.O_APPEND|os.O_RDWR, 0644)
	ms.CheckError(err)
	_, err = file.WriteString(frag)
	ms.CheckError(err)
	file.Close()
}

func main() {
	usersFile := os.Args[1]
	pid, _ := strconv.Atoi(os.Args[2])
	procFile := "fichero_" + strconv.Itoa(pid)

	ra := ra.New(pid, ra.WRITER, usersFile, procFile)

	log.Printf("\t[%d] Esperamos 20 segundos a que todos inicializen\n", pid)
	time.Sleep(20 * time.Second)

	for i := 0; i < 7; i++ {
		frag := "El proceso " + strconv.Itoa(ra.Me) + " añade el mensaje " +
			strconv.Itoa(i) + "\n"

		ra.PreProtocol()
		// Entro a SC
		EscribirFichero(frag, procFile)
		ra.UpdateNodeFiles(frag) // Aviso a los demás nodos sobre cambios
		// Salgo de SC
		ra.PostProtocol()
	}
	ra.Stop()
}
