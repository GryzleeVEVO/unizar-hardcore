package testintegracionraft1

import (
	"fmt"
	"raft/internal/comun/check"

	//"log"
	//"crypto/rand"
	"os"
	"path/filepath"
	"strconv"
	"testing"
	"time"

	"raft/internal/comun/rpctimeout"
	"raft/internal/despliegue"
	"raft/internal/raft"
)

const (
	//hosts
	MAQUINA1 = "127.0.0.1"
	MAQUINA2 = "127.0.0.1"
	MAQUINA3 = "127.0.0.1"

	//puertos
	PUERTOREPLICA1 = "29350"
	PUERTOREPLICA2 = "29351"
	PUERTOREPLICA3 = "29352"

	//nodos replicas
	REPLICA1 = MAQUINA1 + ":" + PUERTOREPLICA1
	REPLICA2 = MAQUINA2 + ":" + PUERTOREPLICA2
	REPLICA3 = MAQUINA3 + ":" + PUERTOREPLICA3

	// paquete main de ejecutables relativos a PATH previo
	EXECREPLICA = "cmd/srvraft/main.go"

	// comandos completo a ejecutar en máquinas remota con ssh. Ejemplo :
	// 				cd $HOME/raft; go run cmd/srvraft/main.go 127.0.0.1:29001

	// Ubicar, en esta constante, nombre de fichero de vuestra clave privada local
	// emparejada con la clave pública en authorized_keys de máquinas remotas

	PRIVKEYFILE = "id_rsa"
)

// PATH de los ejecutables de modulo golang de servicio Raft
var PATH string = filepath.Join(os.Getenv("HOME"), "Downloads", "CodigoEsqueleto", "raft")

// go run cmd/srvraft/main.go 0 127.0.0.1:29001 127.0.0.1:29002 127.0.0.1:29003
var EXECREPLICACMD string = "cd " + PATH + "; go run " + EXECREPLICA

// TEST primer rango
func TestPrimerasPruebas(t *testing.T) { // (m *testing.M) {
	// <setup code>
	// Crear canal de resultados de ejecuciones ssh en maquinas remotas
	cfg := makeCfgDespliegue(t,
		3,
		[]string{REPLICA1, REPLICA2, REPLICA3},
		[]bool{true, true, true})

	// tear down code
	// eliminar procesos en máquinas remotas
	defer cfg.stop()

	// Run test sequence

	// Test1 : No debería haber ningun primario, si SV no ha recibido aún latidos
	t.Run("T1:soloArranqueYparada",
		func(t *testing.T) { cfg.soloArranqueYparadaTest1(t) })

	// Test2 : No debería haber ningun primario, si SV no ha recibido aún latidos
	t.Run("T2:ElegirPrimerLider",
		func(t *testing.T) { cfg.elegirPrimerLiderTest2(t) })

	// Test3: tenemos el primer primario correcto
	t.Run("T3:FalloAnteriorElegirNuevoLider",
		func(t *testing.T) { cfg.falloAnteriorElegirNuevoLiderTest3(t) })

	// Test4: Tres operaciones comprometidas en configuración estable
	t.Run("T4:tresOperacionesComprometidasEstable",
		func(t *testing.T) { cfg.tresOperacionesComprometidasEstable(t) })
}

// TEST primer rango
func TestAcuerdosConFallos(t *testing.T) { // (m *testing.M) {
	// <setup code>
	// Crear canal de resultados de ejecuciones ssh en maquinas remotas
	cfg := makeCfgDespliegue(t,
		3,
		[]string{REPLICA1, REPLICA2, REPLICA3},
		[]bool{true, true, true})

	// tear down code
	// eliminar procesos en máquinas remotas
	defer cfg.stop()

	// Test5: Se consigue acuerdo a pesar de desconexiones de seguidor
	t.Run("T5:AcuerdoAPesarDeDesconexionesDeSeguidor ",
		func(t *testing.T) { cfg.AcuerdoApesarDeSeguidor(t) })

	t.Run("T5:SinAcuerdoPorFallos ",
		func(t *testing.T) { cfg.SinAcuerdoPorFallos(t) })

	t.Run("T5:SometerConcurrentementeOperaciones ",
		func(t *testing.T) { cfg.SometerConcurrentementeOperaciones(t) })

}

// ---------------------------------------------------------------------
//
// Canal de resultados de ejecución de comandos ssh remotos
type canalResultados chan string

func (cr canalResultados) stop() {
	close(cr)

	// Leer las salidas obtenidos de los comandos ssh ejecutados
	for s := range cr {
		fmt.Println(s)
	}
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

// Crear una configuracion de despliegue
func makeCfgDespliegue(t *testing.T, n int, nodosraft []string,
	conectados []bool) *configDespliegue {
	cfg := &configDespliegue{}
	cfg.t = t
	cfg.conectados = conectados
	cfg.numReplicas = n
	cfg.nodosRaft = rpctimeout.StringArrayToHostPortArray(nodosraft)
	cfg.cr = make(canalResultados, 2000)

	return cfg
}

func (cfg *configDespliegue) stop() {
	//cfg.stopDistributedProcesses()

	time.Sleep(50 * time.Millisecond)

	cfg.cr.stop()
}

// --------------------------------------------------------------------------
// FUNCIONES DE SUBTESTS

// Se pone en marcha una replica ?? - 3 NODOS RAFT
func (cfg *configDespliegue) soloArranqueYparadaTest1(t *testing.T) {
	//t.Skip("SKIPPED soloArranqueYparadaTest1")

	fmt.Println(t.Name(), ".....................")

	cfg.t = t // Actualizar la estructura de datos de tests para errores

	// Poner en marcha replicas en remoto con un tiempo de espera incluido
	cfg.startDistributedProcesses()

	// Parar réplicas almacenamiento en remoto
	cfg.stopDistributedProcesses()

	fmt.Println(".............", t.Name(), "Superado")
}

// Primer lider en marcha - 3 NODOS RAFT
func (cfg *configDespliegue) elegirPrimerLiderTest2(t *testing.T) {
	//t.Skip("SKIPPED ElegirPrimerLiderTest2")

	fmt.Println(t.Name(), ".....................")

	cfg.startDistributedProcesses()

	// Se ha elegido lider ?
	fmt.Printf("Probando lider en curso\n")
	cfg.pruebaUnLider(3)

	// Parar réplicas alamcenamiento en remoto
	cfg.stopDistributedProcesses() // Parametros

	fmt.Println(".............", t.Name(), "Superado")
}

// Fallo de un primer lider y reeleccion de uno nuevo - 3 NODOS RAFT
func (cfg *configDespliegue) falloAnteriorElegirNuevoLiderTest3(t *testing.T) {
	//t.Skip("SKIPPED FalloAnteriorElegirNuevoLiderTest3")

	fmt.Println(t.Name(), ".....................")

	cfg.startDistributedProcesses()

	fmt.Printf("Lider inicial\n")
	cfg.pruebaUnLider(3)

	// Desconectar lider
	liderActual := cfg.pruebaUnLider(3) // Verificamos quien es el lider actual
	cfg.detenerNodoRemoto(liderActual)  // Paramos nodo de lider
	time.Sleep(10 * time.Millisecond)

	fmt.Printf("Comprobar nuevo lider\n")
	cfg.pruebaUnLider(3)

	// Parar réplicas almacenamiento en remoto
	cfg.stopDistributedProcesses() //parametros

	fmt.Println(".............", t.Name(), "Superado")
}

// 3 operaciones comprometidas con situacion estable y sin fallos - 3 NODOS RAFT
func (cfg *configDespliegue) tresOperacionesComprometidasEstable(t *testing.T) {
	//t.Skip("SKIPPED tresOperacionesComprometidasEstable")

	fmt.Println(t.Name(), ".....................")

	cfg.startDistributedProcesses()

	fmt.Printf("Lider inicial\n")
	liderActual := cfg.pruebaUnLider(3)
	fmt.Println("Lider actual es ", liderActual)

	indices := make([]raft.ResultadoRemoto, 3)

	// Verifico si hace append correctamente de 3 operaciones
	for i := 0; i < 3; i++ {
		fmt.Println("Comprometiendo operacion ", i, " al lider ", liderActual)
		indices[i] = cfg.someterOperacionLider(liderActual)
		fmt.Println("Operación comprometida en ", indices[i].IndiceRegistro)

		fmt.Println("Se ha escrito en maquina de estados, " +
			indices[i].ValorADevolver)
	}

	// Parar réplicas alamcenamiento en remoto
	cfg.stopDistributedProcesses() // Parametros

	if indices[0].IndiceRegistro < indices[1].IndiceRegistro &&
		indices[1].IndiceRegistro < indices[2].IndiceRegistro {
		fmt.Println("Entradas comprometidas correctamente")
		fmt.Println(".............", t.Name(), "Superado")
	} else {
		cfg.t.Fatalf("No se han comprometido bien entradas en %s", t.Name())
	}
}

// Se consigue acuerdo a pesar de desconexiones de seguidor -- 3 NODOS RAFT
func (cfg *configDespliegue) AcuerdoApesarDeSeguidor(t *testing.T) {
	//t.Skip("SKIPPED AcuerdoApesarDeSeguidor")

	fmt.Println(t.Name(), ".....................")

	cfg.startDistributedProcesses()

	fmt.Printf("Lider inicial\n")
	liderActual := cfg.pruebaUnLider(3)
	fmt.Println("Lider actual es ", liderActual)

	// Comprometer una entrada
	resComprometido := cfg.someterOperacionPersonalizadaLider(liderActual,
		raft.TipoOperacion{Operacion: "escribir", Clave: "123", Valor: "Hola mundo!"})
	fmt.Println("Acabo de escribir en clave 123 " +
		resComprometido.ValorADevolver)

	nodoDetener := 0

	//  Obtener un lider y, a continuación desconectar uno de los nodos Raft
	if liderActual == 0 {
		nodoDetener = 2
		cfg.detenerNodoRemoto(nodoDetener)
		time.Sleep(10 * time.Millisecond)
	} else {
		nodoDetener = liderActual - 1
		cfg.detenerNodoRemoto(nodoDetener)
		time.Sleep(10 * time.Millisecond)
	}

	// Comprobar varios acuerdos con una réplica desconectada y además
	// verificamos si anteriormente comprometida la lee bien
	resComprometido2 := cfg.someterOperacionPersonalizadaLider(liderActual,
		raft.TipoOperacion{Operacion: "leer", Clave: "123", Valor: ""})

	if resComprometido.ValorADevolver != resComprometido2.ValorADevolver {
		cfg.t.Fatalf("Se ha leido de ME una entrada distinta en clave 123 %s",
			t.Name())
	}
	resComprometido3 := cfg.someterOperacionPersonalizadaLider(liderActual,
		raft.TipoOperacion{Operacion: "escribir", Clave: "123", Valor: "Nuevo"})
	fmt.Println("Acabo de escribir en clave 123 " +
		resComprometido3.ValorADevolver)

	// reconectar nodo Raft previamente desconectado y comprobar varios acuerdos
	cfg.startDistributedProcess(nodoDetener)
	resComprometido4 := cfg.someterOperacionPersonalizadaLider(liderActual,
		raft.TipoOperacion{Operacion: "leer", Clave: "123", Valor: ""})

	if resComprometido3.ValorADevolver != resComprometido4.ValorADevolver {
		cfg.t.Fatalf("Se ha leido de ME una entrada distinta en clave 123 %s",
			t.Name())
	}
	resComprometido5 := cfg.someterOperacionPersonalizadaLider(liderActual,
		raft.TipoOperacion{Operacion: "escribir", Clave: "123", Valor: "Viejo"})
	fmt.Println("Acabo de escribir en clave 123 " +
		resComprometido5.ValorADevolver)

	// Parar réplicas alamcenamiento en remoto
	cfg.stopDistributedProcesses() // Parametros

	if resComprometido.IdLider == liderActual &&
		resComprometido2.IdLider == liderActual &&
		resComprometido3.IdLider == liderActual &&
		resComprometido4.IdLider == liderActual &&
		resComprometido5.IdLider == liderActual {
		fmt.Println("Entradas comprometidas correctamente en todo momento")
		fmt.Println("Además de lecturas correctas de las escrituras a ME")
		fmt.Println(".............", t.Name(), "Superado")
	} else {
		cfg.t.Fatalf("No se ha mantenido mismo lider en todo momento %s", t.Name())
	}
}

// NO se consigue acuerdo al desconectarse mayoría de seguidores -- 3 NODOS RAFT
func (cfg *configDespliegue) SinAcuerdoPorFallos(t *testing.T) {
	//t.Skip("SKIPPED SinAcuerdoPorFallos")

	fmt.Println(t.Name(), ".....................")

	cfg.startDistributedProcesses()

	respuestas := make([]raft.ResultadoRemoto, 10)

	fmt.Printf("Lider inicial\n")
	liderActual := cfg.pruebaUnLider(3)
	fmt.Println("Lider actual es ", liderActual)

	// Comprometer una entrada
	respuestas[0] = cfg.someterOperacionPersonalizadaLider(liderActual,
		raft.TipoOperacion{Operacion: "escribir", Clave: "123", Valor: "Hola mundo!"})
	fmt.Println("Acabo de escribir en clave 123 " +
		respuestas[0].ValorADevolver)

	//  Obtener un lider y, a continuación desconectar 2 de los nodos Raft
	for j := 0; j < 3; j++ {
		if j != liderActual {
			cfg.detenerNodoRemoto(j)
			time.Sleep(10 * time.Millisecond)
		}
	}

	// Comprobar varios acuerdos con 2 réplicas desconectada
	cfg.someterOperacionPersTimeoutLider(liderActual,
		raft.TipoOperacion{Operacion: "escribir", Clave: "345", Valor: "Valor1"})
	cfg.someterOperacionPersTimeoutLider(liderActual,
		raft.TipoOperacion{Operacion: "escribir", Clave: "456", Valor: "Valor2"})

	// reconectar los 2 nodos Raft desconectados y probar varios acuerdos
	for j := 0; j < 3; j++ {
		if j != liderActual {
			cfg.startDistributedProcess(j)
		}
	}
	respuestas[1] = cfg.someterOperacionPersonalizadaLider(liderActual,
		raft.TipoOperacion{Operacion: "escribir", Clave: "789", Valor: "Valor3"})
	fmt.Println("Acabo de escribir en clave 789 " +
		respuestas[1].ValorADevolver)

	respuestas[2] = cfg.someterOperacionPersonalizadaLider(liderActual,
		raft.TipoOperacion{Operacion: "leer", Clave: "123", Valor: ""})

	if respuestas[0].ValorADevolver != respuestas[2].ValorADevolver {
		cfg.t.Fatalf("Se ha leido de ME una entrada distinta en clave 123 %s",
			t.Name())
	}

	respuestas[3] = cfg.someterOperacionPersonalizadaLider(liderActual,
		raft.TipoOperacion{Operacion: "leer", Clave: "789", Valor: ""})

	if respuestas[1].ValorADevolver != respuestas[3].ValorADevolver {
		cfg.t.Fatalf("Se ha leido de ME una entrada distinta en clave 789 %s",
			t.Name())
	}

	// Parar réplicas alamcenamiento en remoto
	cfg.stopDistributedProcesses() // Parametros

	if respuestas[0].IdLider == liderActual &&
		respuestas[1].IdLider == liderActual &&
		respuestas[2].IdLider == liderActual &&
		respuestas[3].IdLider == liderActual {
		fmt.Println("Entradas comprometidas correctamente tras recuperar nodos")
		fmt.Println("Además de lecturas correctas de las escrituras a ME")
		fmt.Println(".............", t.Name(), "Superado")
	} else {
		cfg.t.Fatalf("No se ha mantenido mismo lider en todo momento %s", t.Name())
	}
}

// Se someten 5 operaciones de forma concurrente -- 3 NODOS RAFT
func (cfg *configDespliegue) SometerConcurrentementeOperaciones(t *testing.T) {
	//t.Skip("SKIPPED SometerConcurrentementeOperaciones")

	fmt.Println(t.Name(), ".....................")

	cfg.startDistributedProcesses()

	// Obtener un lider y, a continuación someter una operacion
	fmt.Printf("Lider inicial\n")
	liderActual := cfg.pruebaUnLider(3)
	fmt.Println("Lider actual es ", liderActual)

	escrito := cfg.someterOperacionPersonalizadaLider(liderActual,
		raft.TipoOperacion{Operacion: "escribir", Clave: "123", Valor: "Hola mundo!"})
	fmt.Println("Acabo de escribir en clave 123 " +
		escrito.ValorADevolver)

	// Someter 5  operaciones concurrentes
	for j := 0; j < 5; j++ {
		go cfg.someterOperacionPersonalizadaLider(liderActual,
			raft.TipoOperacion{Operacion: "escribir", Clave: strconv.Itoa(j), Valor: "Rand"})
	}

	time.Sleep(5000 * time.Millisecond)

	// Comprobar estados de nodos Raft
	for j := 0; j < 5; j++ {
		leido := cfg.someterOperacionPersonalizadaLider(liderActual,
			raft.TipoOperacion{Operacion: "leer", Clave: strconv.Itoa(j), Valor: ""})

		fmt.Println("Se ha leido \"" + leido.ValorADevolver + "\"")

		if leido.ValorADevolver != "Rand" {
			cfg.t.Fatalf("Entrada de GoRutina %d no comprometida %s",
				j, t.Name())
		}
	}

	// Parar réplicas alamcenamiento en remoto
	cfg.stopDistributedProcesses() // Parametros

	fmt.Println("Entradas comprometidas correctamente de forma concurrente")
	fmt.Println("Además de lecturas correctas de las escrituras a ME")
	fmt.Println(".............", t.Name(), "Superado")
}

// --------------------------------------------------------------------------
// FUNCIONES DE APOYO
// Comprobar que hay un solo lider
// probar varias veces si se necesitan reelecciones
func (cfg *configDespliegue) pruebaUnLider(numreplicas int) int {
	for iters := 0; iters < 10; iters++ {
		time.Sleep(500 * time.Millisecond)
		mapaLideres := make(map[int][]int)
		for i := 0; i < numreplicas; i++ {
			if cfg.conectados[i] {
				if _, mandato, eslider, _ := cfg.obtenerEstadoRemoto(i); eslider {
					mapaLideres[mandato] = append(mapaLideres[mandato], i)
				}
			}
		}

		ultimoMandatoConLider := -1
		for mandato, lideres := range mapaLideres {
			if len(lideres) > 1 {
				cfg.t.Fatalf("mandato %d tiene %d (>1) lideres",
					mandato, len(lideres))
			}
			if mandato > ultimoMandatoConLider {
				ultimoMandatoConLider = mandato
			}
		}

		if len(mapaLideres) != 0 {

			return mapaLideres[ultimoMandatoConLider][0] // Termina

		}
	}
	cfg.t.Fatalf("un lider esperado, ninguno obtenido")

	return -1 // Termina
}

func (cfg *configDespliegue) obtenerEstadoRemoto(
	indiceNodo int) (int, int, bool, int) {
	var reply raft.EstadoRemoto
	err := cfg.nodosRaft[indiceNodo].CallTimeout("NodoRaft.ObtenerEstadoNodo",
		raft.Vacio{}, &reply, 10*time.Millisecond)
	check.CheckError(err, "Error en llamada RPC ObtenerEstadoRemoto")

	return reply.IdNodo, reply.Mandato, reply.EsLider, reply.IdLider
}

// start  gestor de vistas; mapa de replicas y maquinas donde ubicarlos;
// y lista clientes (host:puerto)
func (cfg *configDespliegue) startDistributedProcesses() {
	//cfg.t.Log("Before starting following distributed processes: ", cfg.nodosRaft)

	for i, endPoint := range cfg.nodosRaft {
		cfg.conectados[i] = true
		despliegue.ExecMutipleHosts(EXECREPLICACMD+
			" "+strconv.Itoa(i)+" "+
			rpctimeout.HostPortArrayToString(cfg.nodosRaft),
			[]string{endPoint.Host()}, cfg.cr, PRIVKEYFILE)

		// dar tiempo para se establezcan las replicas
		//time.Sleep(2000 * time.Millisecond)
	}

	// aproximadamente 500 ms para cada arranque por ssh en portatil
	time.Sleep(3000 * time.Millisecond)
}

// start un único nodo raft dado su identificador
func (cfg *configDespliegue) startDistributedProcess(node int) {
	//cfg.t.Log("Before starting following distributed processes: ", cfg.nodosRaft)
	cfg.conectados[node] = true

	despliegue.ExecMutipleHosts(EXECREPLICACMD+
		" "+strconv.Itoa(node)+" "+
		rpctimeout.HostPortArrayToString(cfg.nodosRaft),
		[]string{cfg.nodosRaft[node].Host()}, cfg.cr, PRIVKEYFILE)

	// aproximadamente 500 ms para cada arranque por ssh en portatil
	time.Sleep(3000 * time.Millisecond)
}

func (cfg *configDespliegue) stopDistributedProcesses() {
	var reply raft.Vacio

	for i, endPoint := range cfg.nodosRaft {
		if cfg.conectados[i] {
			err := endPoint.CallTimeout("NodoRaft.ParaNodo",
				raft.Vacio{}, &reply, 10*time.Millisecond)
			check.CheckError(err, "Error en llamada RPC Para nodo")
		}
	}
}

// Comprobar estado remoto de un nodo con respecto a un estado prefijado
func (cfg *configDespliegue) comprobarEstadoRemoto(idNodoDeseado int,
	mandatoDeseado int, esLiderDeseado bool, IdLiderDeseado int) {
	idNodo, mandato, esLider, idLider := cfg.obtenerEstadoRemoto(idNodoDeseado)

	//cfg.t.Log("Estado replica 0: ", idNodo, mandato, esLider, idLider, "\n")

	if idNodo != idNodoDeseado || mandato != mandatoDeseado ||
		esLider != esLiderDeseado || idLider != IdLiderDeseado {
		cfg.t.Fatalf("Estado incorrecto en replica %d en subtest %s",
			idNodoDeseado, cfg.t.Name())
	}

}

// Función para parar nodo especificado como parametro del cluster
func (cfg *configDespliegue) detenerNodoRemoto(indiceNodo int) {
	var reply raft.Vacio
	err := cfg.nodosRaft[indiceNodo].CallTimeout("NodoRaft.ParaNodo",
		raft.Vacio{}, &reply, 10*time.Millisecond)
	check.CheckError(err, "Error en llamada RPC ParaNodo")
	cfg.conectados[indiceNodo] = false
}

// Función para someter operación a lider estandar, que siempre escribira
// "Hola mundo!" en la clave "123" de la maquina de estados
func (cfg *configDespliegue) someterOperacionLider(indiceNodo int) raft.ResultadoRemoto {
	var reply raft.ResultadoRemoto
	OP := raft.TipoOperacion{Operacion: "escribir", Clave: "123", Valor: "Hola mundo!"}
	err := cfg.nodosRaft[indiceNodo].CallTimeout("NodoRaft.SometerOperacionRaft",
		OP, &reply, 10*time.Millisecond)
	check.CheckError(err, "Error en llamada RPC SometerOperacionRaft")
	return reply
}

// Función para someter operación escribir/leer a lider con texto
// y clave a elegir
func (cfg *configDespliegue) someterOperacionPersonalizadaLider(indiceNodo int,
	OP raft.TipoOperacion) raft.ResultadoRemoto {

	var reply raft.ResultadoRemoto
	err := cfg.nodosRaft[indiceNodo].CallTimeout("NodoRaft.SometerOperacionRaft",
		OP, &reply, 10*time.Millisecond)
	check.CheckError(err, "Error en llamada RPC SometerOperacionRaft")
	return reply
}

// Función para someter operación escribir/leer a lider con texto
// y clave a elegir, en el caso de que pueda comprometer entrada devolvera error
// ya que no debería porque no recibe mayoría de nodos replicados
func (cfg *configDespliegue) someterOperacionPersTimeoutLider(indiceNodo int,
	OP raft.TipoOperacion) raft.ResultadoRemoto {

	var reply raft.ResultadoRemoto
	err := cfg.nodosRaft[indiceNodo].CallTimeout("NodoRaft.SometerOperacionRaft",
		OP, &reply, 10*time.Millisecond)

	if err == nil {
		cfg.t.Fatalf("Operación comprometida y no debería porque no hay mayoría")
	} else {
		fmt.Println("Ha habido timeout esperado en SometerOperación, " +
			"correcto ya que se han dormido 2 nodos y no se dispone de mayoría")
	}
	return reply
}
