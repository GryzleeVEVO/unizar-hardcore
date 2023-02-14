---
mainfont: Liberation Serif
monofont: JetBrains Mono 

title: Algoritmia básica
---

# ALGORITMOS VORACES

Los algoritmos voraces se utilizan para resolver problemas de optimización: de un **conjunto de elementos candidatos**, se quiere obtener un **subconjunto solución factible** que maximice/minimice una **función objetivo**.

1. Se inicia con un conjunto vacío de candidatos.
2. Se intenta añadir el mejor elemento no escogido siguiendo una **función de selección**.
3. Si añadiendo el elemento el problema aún es **completable** (se puede llegar a la solución añadiendo mas elementos) se añade, de lo contrario se rechaza y elimina como posible elemento.
4. Si no es solución, volver al paso 2

## El problema de la mochila

Un ejemplo sencillo de un problema que se puede resolver con un algoritmo voraz es el de la mochila:

- Se dispone de una **mochila** con **capacidad limitada** ($C$).
- Se dispone de una serie de **objetos** ($n$). Los objetos se pueden dividir ($x_i$). Además, tienen un peso asociado ($p_i$).
- Introducir un objeto o una fracción de este da un **beneficio**, proporcional a la cantidad de dicho objeto ($b_ix_i$).
- Se desea llenar la mochila con el **máximo beneficio** sin exceder su capacidad ($(x_1,...x_n)$ tal que $max(\sum_{1\le i \le n}b_ix_i)$ y $\sum{1\le i \le n}p_ix_i\le C$).


El problema es trivial si se pueden meter todos los objetos en la mochila sin sobrecargarla. Si el peso total supera la capacidad de la mochila, la estrategia mas óptima es **tomar los objetos que proporcionen mayor beneficio por unidad de peso**.


```cpp
/* 
    Pre: Los objetos deben estar ordenados de mayor a menor según la proporción 
         de beneficio obtenido por unidad de peso:
         ∀i∈1..n:peso[i]>0 ∧ ∀i∈1..n-1:benef[i]/peso[i]≥benef[i+1]/peso[i+1]
    Post: sol es solución óptima del problema de la mochila
 */
double[n] mochila(double benef[n], double peso[n], double cap) 
{
    // Inicialización
    double sol[n] = {0}; 
    Capacidad resto = cap; 
    int i = 1;

    // Selecciona tantos objetos como pueda
    for (i = 1; i <= n && peso[i] <= resto; i++)
    {
        sol[i] = 1;
        resto -= peso[i];
    }

    // Coge la fracción del último objeto que llene la mochila
    if (i <= n) sol[i] = resto/peso[i]

    return sol;
}
```
Su coste temporal es lineal (o logarítmico $\Theta(n\log\space n)$ si hay que ordenar los vectores).

## Caminos mínimos en grafos

Se desea encontrar el camino de menor peso de un grafo etiquetado con pesos no negativos para cada arista. Los grafos pueden ser dirigidos o no dirigidos.

Los grafos se pueden representar de varias maneras:

- Una **matriz de adyacencia** es una matriz cuadrada donde el elemento $[i,j]$ indica si existe una arista entre los vértices $i$ y $j$ (y su peso si lo tuviera).
- Una **lista de adyacencia** contiene los sucesores de un vértice $i$. Se tiene tantas listas como vértices en el grafo.
- Las **listas de adyacencia inversas** son similares, pero se almacenan los predecesores de cada vértice. Se puede compactar en una **lista múltiple**, donde cada lista contiene el origen y destino de una arista.

Representación Coste espacial     Consulta existencia arista Consulta sucesores
-------------- ------------------ -------------------------- ------------------
Matriz         $\Theta(n^2)$      $\Theta(1)$                $\Theta(n)$       
Listas         $\Theta(max(n,a))$ $\Theta(n)$                $\Theta(1)$

### Algoritmo de Dijkstra

El **algoritmo de Dijkstra** permite encontrar el camino mínimo desde un vértice hasta el resto.

1. Inicialmente **todos los vértices están sin visitar**. El vértice origen se etiqueta con 0 y el resto se suponen a distancia infinita.
2. Calcular la **distancia mínima** de los vértices que son **vecinos del vértice actual**. Si la distancia a un vértice es menor que la etiquetada, se actualiza.
3. Si quedan vértices por visitar, se **elige el vértice cuya distancia desde el inicio sea la menor** y repetir paso 2.
4. Si se **agotan los vértices** por visitar, o son inaccesibles (todos están etiquetados con distancia infinita), se ha acabado. Alternativamente, se acaba al llegar al vértice destino (si lo hubiera).


```cpp
/*
    Pre: g es un grafo dirigido etiquetado no negativo
    Post: D=caminosMínimos(g,v)

    Implementación adaptada de
    https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-greedy-algo-7/
*/
int[n] algoritmoDijkstra(int grafo[n][n], int verticeOrigen) 
{    
    // Inicialización
    int distancia[n] = { -1 }; 
    bool visitado[n] = { false };
    distancia[verticeOrigen] = 0;

    for (int i = 1; i <= n - 1; i++) 
    {
        // Calcula la minima distancia (=verticeOrigen en la primera iteración)
        int actual = -1, distActual = -1;

        for (int j = 1; j <= n; j++) 
        {
            if (!visitado[j] && (!actual || distancia[j] <= distActual))
            {
                actual = j; distActual = distancia[j]
            }
        }

        // Si la distancia es infinita, no hay nada mas que hacer
        if (actual == - 1) break;

        // Marca vértice consultado como visitado    
        visitado[actual] = true;

        // Recalcula caminos mínimos entre vértices
        for (int vecino = 1; j <= v; j++) 
        {
            if (!visitado[vecino] && grafo[actual][vecino] && 
                (!distancia[vecino] || distancia[actual] +
                 grafo[actual][vecino] < distancia[vecino])) 
            {
                distancia[vecino] = distancia[actual] + grafo[actual][vecino];
            }
        }
    }

    return distancia;
}
```
### Dijkstra usando colas de prioridades

El algoritmo anterior tiene un coste temporal de $\theta(n^2)$. Si el grafo es disperso, se puede utilizar una versión del algoritmo que utilice listas de adyacencia y colas de prioridades. En ese caso, se puede conseguir un coste de $\theta(a\log n)$.

```cpp

// Aristas (sucesor/peso)
typedef pair<int,int> Arista;

// Vector de listas de adyacencia (sucesores/vértice)
typedef list<Arista>[n] Grafo;

int[n] algoritmoDijkstra(Grafo grafo, int verticeOrigen) 
{
    // Se crea una cola de prioridades y añade nodo origen a distancia 0
    // Nota: Aquí el peso va primero. El código para ordenar con 
    // segundo elemento de la pareja es mas complicado
    priority_queue<Arista, vector<Arista>, greater<Arista>> cola;
    cola.push(make_pair(0, VerticeOrigen))

    // Vector de distancias
    int distancias[n] = { -1 };
    distancia[verticeOrigen] = 0;

    while (!cola.empty()) 
    {
        // Extrae de la cola el vértice a menor distancia
        int actual = cola.top().second();
        cola.pop();

        // Actualiza distancias
        for (auto i : grafo[actual]) 
        {
            int vecino = i->first, pesoArista = i->second;

            if (!distancia[vecino] 
                || distancia[actual] + pesoArista < distancia[vecino]) 
            {
                distancia[vecino] = distancia[actual] + pesoArista;
                cola.push(make_pair(distancia[vecino], vecino))
            }
        }
    }   

    return distancia;
}
```

## Árboles de recubrimiento de coste mínimo

Se desea, a partir de un grafo, obtener otro con las conexiones imprescindibles para una optimización global de las conexiones entre nodos (es decir, eliminar vértices que no sean parte del camino mínimo de cualquier pareja de nodos).

Para ello, la solución debe ser:

- Un **árbol libre** (Acíclico, $n$ vértices, $n-1$ aristas).
- De **recubrimiento** (Árbol libre con todos los vértices).
- De **coste mínimo** (Árbol de recubrimiento cuya suma de pesos de aristas sea la menor).

### Algoritmo de Prim

El algoritmo de Prim utiliza la siguiente propiedad reiteradamente: si hay un conjunto de vertices $U\subset V$ que pertenecen a un grafo no dirigido conexo, la arista <$u,v$>, donde $v$ no pertenece al conjunto $U$, es cubierta por algún árbol de recubrimiento.

1. Se inicia con un vértice aleatorio del grafo.
2. Se añade al conjunto el vértice con la arista de menor peso que la una con el conjunto.
3. Se repite hasta agotar las entradas.

```cpp
int[n][n] algoritmoPrim(int grafo[n][n]) 
{
    // Inicialización
    int solucion[n][n] = { -1 };

    int sol[n] = { -1; } 
    int peso[n] = { -1 };
    bool anyadido[n] = { false };
    
    int actual = rand() % n + 1;
    sol[actual] = 0;

    for (int i = 1; i <= n; i++) 
    {
        // Escoge el vértice mas cercano fuera de la solución
        int siguiente = -1, pesoSiguiente = -1;

        for (int j = 1, j <= n; j++) 
        {
            if (!anyadido[j] && (!peso[j] || peso[j] < pesoSiguiente))
            {
                siguiente = j; pesoSiguiente = peso[j]; 
            }    
        }

        anyadido[siguiente] = true;

        // Actualiza distancias de nodos alcanzables
        for (int j = 1; j <= n; j++) 
        {
            if (grafo[siguiente][j] && !anyadido[j] && grafo[siguiente][j]) 
            {
                sol[j] = siguiente;
            }
        }
    }

    for (int 

    return solucion;
}

```


### Algoritmo de Kruskal


## Códigos de Huffman

## El problema de la selección de actividades

## El problema de la minimización del tiempo de espera

## Fundamentos teóricos del esquema voraz

## Un problema de planificación de tareas a plazo fijo

## Heurísticas voraces

### Coloreado de grados

### El problema del viajante de comercio

# DIVIDE Y VENCERÁS

# PROGRAMACIÓN DINÁMICA

# BÚSQUEDA CON RETROCESO

# RAMIFICACIÓN Y PODA

# PROGRAMACIÓN LINEAL Y REDUCCIONES

# ¿¿¿¿PROBLEMAS????