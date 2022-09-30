---
title: Sistemas Distribuidos
subtitle: Resumen
author: Dorian Wozniak
---

# COORDINACIÓN BÁSICA

Un objetivo esencial de los sistemas concurrentes es coordinar la ejecución de procesos interdependiente que solo interactuan mediante paso de mensajes. Para ello ese requiere gestionar el tiempo mediante la correcta ordenación del tiempo, tal que cumplan con el **principio de causalidad**.

La generación de eventos cumple dos propiedades básicas:

- **Vivacidad** (liveness): La ejecución de procesos no se bloquea indefinidamente.
- **Corrección** (safety): La ejecución de procesos es correcta según la especificación.

## Modelos temporales

Hay tres modelos temporales básicos de algoritmos distribuidos:

- **Modelo asíncrono**
  - Indeterminismo temporal: No hay límite de tiempo para entrega de mensajes y ejecución de acciones.
  -  Los fallos influyen notablemente en el indeterminismo.
  -  Se puede gestionar el paso del tiempo mediante entregas de mensajes, sin relojes físicos.
  -  No todos los problemas se pueden resolver de forma asíncrona.
- **Modelo síncrono**
  - Determinismo en límites de comportamiento: El tiempo de retraso de mensajes y ejecución de acciones está acotado.
  - Los relojes físicos de cada nodo estan sincronizados dentro de un límite máximo de sincronización. 
  - Es dificil diseñar sistemas distribuidos que cumplan con los límites de tiempo con alta probabilidad.
- **Modelo parcialmente síncrono**: Sistema generalmente asíncrono que circunstancialmente se vuelve síncrono.

Los algoritmos asíncronos son válidos para sistemas síncronos, pero no viceversa.

## Relojes

Los **relojes** son la herramienta base de coordinación y sincronización.

Hay dos tipos de relojes básicos. Normalmente se utiliza una mezcla de ambos:

### Relojes físicos

Los relojes físicos son utilizados para modelos síncronos y parcialmente síncronos.

- Avanzan con frecuencia diferente entre ellos. Se trata de mantenerlos dentro de un límite respecto a un reloj global.
- Dos modelos de sincronización de relojes físicos:
  - Interno (entre relojes del sistema)
  - Externo (con un reloj fuera del sistema, ej. UTC)
- La sincronización se modeliza:
  - $\bold{r}$ **Deriva** (*drift*): Diferencia de frecuencia de dos relojes
  - $\bold{d}$ **Desviación** (*skew*): Máxima deriva permitida
  - $\bold{R}$: **Intervalo de resincronización**: Relación entre deriva y desviación

### Reloj lógicos

Los relojes lógicos son usados para modelos asíncronos.

Representan una relación de causalidad entre eventos de envío y recepción de mensajes entre procesos secuenciales. 

**Relación de orden local**: En un sistema de N procesos, es una relación binaria tal que si un proceso $P_i$ observa e antes de e', $e \rightarrow _i e'$

#### Relojes de Lamport (escalares)

Los relojes de Lamport definen relaciones de orden global. Así, si $a$ localmente precede a $b$ en un proceso, $a$ precede a $b$ globalmente ($a \rightarrow b$). 

- Para todo mensaje $m$, su envío precede a su recepción
- La relación es casi de orden parcial salvo por la no reflexividad
- En $a \rightarrow b$, $a$ afecta causalmente a $b$
- Los eventos no ordenados causalmente son **eventos concurrentes**


*Condiciones que deben satisfacer*

- $C_i$ es el reloj local del proceso $P_i$
- Si $a \rightarrow b$ en el proceso $P_i$, $C_i(a) \le C_i(b)$
- Sea $a$ un envío de mensaje $m$ desde $P_i$, y $b$ la recepción del mensaje $m$ eb $P_j$, $C_i(a) < C_j(b)$

*Reglas de implementación*

- Antes de estampillar un **evento local** del proces $P_i$, hacer $C_i = C_i + 1$
- Siempre que un mensaje $m$ **se envía** de $P_i$ a $P_j$:
  - $P_j$ ejecuta $C_i = C_i + 1$ y envía el nuevo $C_i$ con $m$
  - $P_j$ recibe $C_i$ con $m$ y ejecuta $C_j = max(C_j, C_i) + 1$. El evento recibir($m$) se almacena con el nuevo $C_j$
 

#### Relojes vectoriales

Se diferencia de los relojes escalares en que no solo mantiene el valor de su reloj sino de todos los relojes que (cree) que tienen el resto de los procesos en un vector. Los procesos ahora comparten el vector de relojes, no solo un reloj.

- **Reloj vectorial de un proceso i**: $V_i[1..N], N=$ nº procesos 
- El valor del reloj que cree que un proceso $i$ tiene de un proceso $j \ne i$ es $V_i[j]$
- El vector empieza en 0 en todos sus campos

*Reglas de actualización*

- Antes de estampillan un **evento local** de $P_i$, $V_i[i] = V_i[i] + 1$
- Siempre que un mensaje $m$ se envía de $P_i$ a $P_j$:
  - En $P_i$ suma 1 a su reloj y envía el vector junto al mensaje a $P_j$, y estampilla 
  - En $P_j$, suma 1 a su reloj y al recibir un mensaje de $P_i$, antes de estampillar, mezcla ambos vectores:
    - $V_j[j] = V_j[j]$ (su reloj se mantiene igual)
    - $V_j[k] = max(V_j[k], V_i[k]) \forall k \ne j$ (actualiza relojes retrasados respecto a $P_i$)

*Características*

- Para todo $i, j, $V_i[i] \ge V_j[i]$ (El reloj de un proceso más actualizado es la del propio proceso)
- $a \rightarrow b$ solo si $V[a] \le V[b]$ 
- Notación
  - $V = V'$ si $V[i] = V'[i] \forall i \in {1..N}$ (Dos vectores son iguales sólo si sus componentes son iguales)
  - $V \ge V'$ si $V[i] \ge V'[i] \forall i \in {1..N}$ (Un vector es mayor o igual que otro si todas sus componentes son mayores o iguales al otro)
  - $V \gt V'$ si $V \ge V' \And V \ne V'$ (Un vector es mayor que otro es mayor o igual que este y no son iguales)
  - $V || V'$ si $V \not \gt V' \And V' \not \gt V$ (Dos vectores son concurrentes si ninguno de los dos es mayor que el otro)

## Estados globales

Algunas funcionalidades necesitan que se cumplan propiedades globales:

- *Snapshots* (Momentos previos del sistema)
- Detección de terminación distribuida
- Detección de bloqueos distribuidos
- Recogida de basura distribuida

Todas estas propiedades son estables (se cumplen mientras no se intervenga intencionalmente). Pero para poder obtener la información de los nodos de forma consistente es necesario un criterio para una recolección global de información.

### Historias

#### Historia local y estado del proceso

Cada proceso $P_i$ contiene una serie de eventos $h_i = [e_i⁰, e_i¹, e_i², ...]$.

- La historia hasta un evento k se denota k-prefijo de $h_i$, $h_i^k$.
- Cada $e_i^j$ es un evento o local o de comunicación

$s_i^k$ es el estado del proceso $P_i$ justo antes del evento $e_i^k$ (es decir, $h_i^{k-1}$).

- $s_i⁰$ es el estado inicial de $P_i$

#### Historia y estado global

Se puede mezclar todas las historias locales en una historia global usando una ordenación total de eventos: $H = \cup_{i=1..N} h_i$.

Igualmente un conjunto de estados locales forman un estado global: $S = (s_1, ..., s_N)$.

Un estado global es consistente si para todo mensaje recibido en el estado, el correspondiente envío está también en el estado.

Un corte es una mezcla de k-prefijos: $K = \cup_{i=1..N} h_i^{c_i}$, donde $h_i^{c_i}$ es la historia de $P_i$ hasta el evento $e_i^{ci}$

Un corte es consistente si para todo evento $e' \in K, e \rightarrow e' implica que e \in K$. Un estado global es consistente si corresponde a un corte consistente.

Una historia global que es consistente con la relación $\rightarrow$, se llama linearización o ejecución consistente. Solo atraviesa estados globales consistentes.


