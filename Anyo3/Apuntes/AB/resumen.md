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

Se tiene una mochila con capacidad limitada ($C$), y una serie de objetos fraccionables ($n$) con un peso determinado($p_i$). Meter una fracción de un objeto ($x_i$) da un beneficio ($b_ix_i$). Se quiere llenar la mochila con el máximo beneficio sin superar el peso ($(x_1,...x_n)$ tal que $max(\sum_{1\le i \le n}b_ix_i)$ y $\sum_{1\le i \le n}p_ix_i\le C$).

Si el peso total supera la capacidad de la mochila, la estrategia mas óptima es tomar los objetos que proporcionen mayor beneficio por unidad de peso.

```cpp
const int n = /* Número objetos */

/* 
    Pre: Los objetos deben estar ordenados de mayor a menor según la proporción 
         de beneficio obtenido por unidad de peso:
         ∀i∈1..n:peso[i]>0 ∧ ∀i∈1..n-1:benef[i]/peso[i]≥benef[i+1]/peso[i+1]
    Post: sol es solución óptima del problema de la mochila
 */  
double[n] mochila(double[n] benef, peso, double cap) 
{
    double[n] sol = {0}; double resto = cap; int i = 1;

    // Selecciona tantos objetos como pueda
    for (i; i <= n && peso[i]<= resto; i++) 
    {
        sol[i] = 1;
        resto -= peso[i];
    }

    // Coge la fracción del último objeto que llene la mochila
    if (i <= n) sol[i] = resto/peso[i]

    return sol;
}
```

## Caminos mínimos en grafos (Dijkstra)

## Árboles de recubrimiento de coste mínimo

## Códigos de Huffman

## El problema de la selección de actividades

## El problema de la minimización del tiempo de espera

## Fundamentos teóricos del esquema voraz

## Un problema de planificaciónn de tareas a plazo fijo

## Heurísticas voraces

### Coloreado de grados

### El problema del viajante de comercio

# DIVIDE Y VENCERÁS

# PROGRAMACIÓN DINÁMICA

# BÚSQUEDA CON RETROCESO

# RAMIFICACIÓN Y PODA

# PROGRAMACIÓN LINEAL Y REDUCCIONES

# ¿¿¿¿PROBLEMAS????