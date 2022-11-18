package testintegracionraft1

import (
	"bufio"
	"os"
	"path/filepath"
	"raft/internal/comun/check"
	"raft/internal/comun/rpctimeout"
	"testing"
)

const (
	HOSTS_FILE     = "hosts"               // Nombre del fichero con nodos
	SRV_FILE       = "cmd/srvraft/main.go" // Nombre del fichero servidor
	PATH_FROM_HOME = "practica3/raft"      // Ruta a módulo desde $HOME

	PRIVKEYFILE = "rsa"
)

// Ruta absoluta a módulo
var PATH string = filepath.Join(os.Getenv("HOME"), PATH_FROM_HOME)

// Comando a ejecutar
var RUN_CMD string = "cd" + PATH + "; go run" + SRV_FILE

// Realiza el juego de pruebas para los nodos dados
func TestPrimerasPruebas(t *testing.T) {

	nodos := leerNodos(filepath.Join(PATH, HOSTS_FILE))

	cfg := makeCfgDespliegue(t, len(nodos))
}

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

// ---------------------------------------------------------------------
// Operativa en configuracion de despliegue y pruebas asociadas
type configDespliegue struct {
	t           *testing.T
	conectados  []bool
	numReplicas int
	nodosRaft   []rpctimeout.HostPort
	cr          canalResultados
}

func makeCfgDespliegue(t*testing.T, n int, nodosRaft rpctimeout.HostPort,
	 conectados [bool]) *configDespliegue {

} 