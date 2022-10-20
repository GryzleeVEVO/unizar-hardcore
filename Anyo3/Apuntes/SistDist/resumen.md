---
title: Sistemas Distribuidos
subtitle: Resumen
author: Dorian Wozniak
---

# COORDINACIÓN BÁSICA

## Modelos temporales

### Modelo asíncrono

### Modelo síncrono

### Modelo parcialmente síncrono

## Relojes

### Relojes físicos

### Relojes lógicos

Los relojes lógicos se basan en **relaciones de causalidad** entre eventos. Si un proceso $P_i$ secuencial observa un evento $e$ antes que $e'$, se conoce como una **relación de orden local** ($e \rightarrow _i e'$). Importan especialmente las relaciones entre envíos y recepciones entre procesos.

#### Relojes escalares (de Lamport)

En los relojes de Lamport se establecen **relaciones de orden global** (*happened before*) entre eventos. Si $a$ ocurre antes que $b$, ($a \rightarrow b$) y $a$ afecta casualmente a $b$. 

- Los eventos se ordenan causalmente
- La relación es transitiva
- La relación es de orden parcial (no reflexiva), pero puede ser de orden total si se incluye los identificadores de proceso
- Si $a \not\rightarrow b$ y $b \not\rightarrow a$, son eventos **concurrentes** ($a || b$)
 
Los relojes $C_i$ deben satisfacer las siguientes condiciones:

- Si $a \rightarrow b$ en el proceso $P_i$, $C_i(a) < C_i(b)$
- Si $a$ es un envío desde $P_i$, y $b$ es una recepción en $P_j$, $C_i(a) < C_j(b)$

Se siguen las siguientes **reglas de implementación**:

- Cada evento local de $P_i$ se estampilla con $C_i = C_i + 1$
- Al realizar un envío de un mensaje $m$ de $P_i$ a $P_j$
  - $P_i$ envía $C_i$ junto a $m$, siendo $C_i = C_i + 1$ 
  - $P_j$ guarda el mensaje, siendo $C_j = max(C_i,C_j) + 1$

[TODO]: # (Añadir figuras Lamport)

En eventos no locales, $C(a) < C(b)$ no implica automaticamente que $a$ ocurra antes que $b$ puesto que no hay traza de donde procede el avance del reloj.

[TODO]: # (Añadir figuras Lamport 2)

#### Relojes vectoriales

En un sistema de $N$ procesos, cada uno contiene un vector $V_i[N]$, donde $V_i[j]$ contiene el mejor valor conocido por el proceso $P_i$ de un proceso $P_j$, y $V_i[i]$ contiene su reloj.

Se siguen las siguientes **reglas de implementación**:

- Cada evento local de $P_i$ se estampilla con $V_i[i] = V_i[i] + 1$
- Al realizar un envío de un mensaje $m$ de $P_i$ a $P_j$
  - $P_i$ envía $V_i$ junto a $m$, siendo $V_i[i] = V_i[i] + 1$ 
  - $P_j$ guarda el mensaje, siendo $V_j$:
    - $V_j[j] = $V_j[j] + 1$
    - $V_j[k] = max(V_i[k], V_j[k]) \quad \forall k \ne j$

[TODO]: # (Añadir ejemplo reloj vectorial)

Los relojes cumplen las siguientes propiedades:

- Un proceso tiene siempre la versión mas actualizada de su propio reloj
- $a \rightarrow b$ solo si $V[a] < V[b]$
- Dados dos relojes vectoriales:
  - $V = V'$ si $V[i] = V'[i] \quad \forall i \in {1..N}$
  - $V \ge V'$ si $V[i] \ge V'[i] \quad \forall i \in {1..N}$
  - $V > V'$ si $V \ge V'$ y $V \ne V'$
  - $V || V'$ si $V \not\ge V'$ y $V' \not\ge V$

## Estados globales



### Historia local

Cada proceso $P_i$ contiene una historia (secuencia de eventos): $h_i = [e_i^0, e_i^1, e_i^2,...]$

- La historia puede ser finita o infinita
- Un k-prefijo de $h_i$ es la historia de $h_i$ hasta $k$: $h_i^k$
- Cada evento puede ser local o de comunicación

### Estado del proceso

$s_i^k$ es el estado de un proceso $P_i$ antes del evento $e_i^k$

- $s_i^k$ memoriza todos los eventos en $h_i^k-1$
- $s_i^0$ es el estado inical de $P_i$

### Historia global

La historia global es la unión de las historias locales: $H = \bigcup_{i=1..N} h_i$



### Corte

### Algoritmo de Chandy-Lamport (instantáneas)

# COORDINACIÓN DE RECURSOS COMPARTIDOS

## Algoritmos distribuidos centralizados

## Algoritmos distribuidos descentralizados

### Algoritmo de Lamport

### Algoritmo de Ricart-Agrawala

### Algoritmo de Suzuki-Kasami (paso de testigo)

### Algoritmo de Raymond

# TOLERANCIA A FALLOS

## Modelos de fallo

## Detección de fallos

## Gestión de fallos