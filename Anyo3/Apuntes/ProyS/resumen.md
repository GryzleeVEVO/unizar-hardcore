---
mainfont: Liberation Serif
monofont: JetBrains Mono

title: Proyecto Software
---

# Gestión de configuraciones y *Git*

*Git* es un sistema de control de versiones (VCS). Permite seguir cambios en el proyecto y permite trabajar con diferentes versiones de una aplicación. También permite controlar quién tiene permitido modificar qué partes del proyecto, y automatizar la construcción del *software*. 

## Repositorios

Un proyecto de git tiene:

- Un repositorio (`.git`), que contiene la historia del proyecto).
- El directorio/árbol de trabajo (donde está la copia del repositorio).
- El área de preparación o índice (dentro de `.git`, con los cambios del proyecto a comprometer).

```bash
# Inicializar nuevo repositorio en el directorio actual
git init

# Clonar repositorio existente, ya sea local, desde una URL acabada en .git o 
# desde un servidor SSH
git clone <DIRECCIÓN>
```

## Rastrear ficheros

Los ficheros del directorio de trabajo pueden estar rastreados (*tracked*) o no (*untracked*). Los ficheros se pueden ignorar (indicados en el fichero `.gitignore`).

Los ficheros rastreados pueden estar comprometidos (*committed*, sin cambios desde último *commit*), modificados (*modified*) o preparados (*staged*, listos para ser comprometidos)

```bash
# Añadir fichero a rastrear.
git add <FICHERO>

# Dejar de rastrear un fichero (sin eliminarlo del directorio de trabajo).
git rm --cached <FICHERO>
```

## Comprometer modificaciones

En el repositorio, se almacenan instantáneas del proyecto (formada por *commits*). Git contiene el estado de todos los ficheros en dicho momento. Los commits están identificados por un *hash* que sirve para verificar su integridad.

```bash
# Comprometer ficheros preparados.
git commit -m <MENSAJE>

# Con -a añade ficheros rastreados y modificados al commit automáticamente
git commit -a ...

# --amend permite corregir el commit anterior.
git commit --amend ...
```

Si se modifica un fichero pero se quiere desistir de estos cambios:

```bash
# Marca fichero como no preparado sin modificarlo.
git restore --staged <FICHERO>

# Restaurar fichero a estado en último commit (se pierden datos).
git checkout -- <FICHERO>
```

Alternativamente, se puede deshacer cambios entre commits:

```bash
# Realiza un commit que deshace los cambios del anterior.
git revert HEAD

# Vuelve a un commit anterior y borra los posteriores (se pierden datos).
git reset --hard <HASH>
```

## Ramas

Las ramas son bifurcaciones del historial de *commits* a partir de uno. Cada rama apunta a un *commit* (normalmente el más actualizado), y la rama de trabajo se llama HEAD. Al crear un repositorio, la rama inicial se llama normalmente *master/main*.

```bash
# Crear una nueva rama a partir del commit del HEAD.
git branch <NOMBRE>

# Eliminar rama
git branch -d <RAMA>

# Cambiar de rama.
git checkout <RAMA>

# -b crea una nueva rama y cambia a ella.
git checkout -b ...

# Cambiar de rama modifica el directorio de trabajo. Si hay cambios en el
# directorio, o hay ficheros preparados, se puede forzar (se pierden datos).
git checkout --force ...
```
Las ramas se pueden volver a unir de varias formas:

- Si la rama actual se fusiona con un descendiente, se realiza un ***fast-forward***.
- Si las ramas son paralelas pero no hay conflictos, se realiza un ***merge commit*** (combina recursivamente los cambios).
- Si hay conflictos, lo indicará y no realizará el *merge*. Además añadirá **marcadores** dentro de los ficheros afectados para facilitar encontrarlos.

```bash
# Mezclar los cambios de una rama con otra 
git merge <RAMA A FUSIONAR>
```

## Consultar repositorio

Es recomendable utilizar herramientas gráficas para realizar consultas del estado del repositorio, pues normalmente se muestra

```bash
# Consultar estado del repositorio (rama, commit, cambios preparados, ficheros
# sin rastrear...).
git status

# Consultar diferencias entre versiones (preparadas o comprometidas).
git diff [<HASH 1> <HASH 2>]

# Consultar historial de commits.
git log

# Consultar cambios entre ramas
git diverge
```

## Repositorios remotos

Los repositorios remotos se encuentran en un servidor o en línea en vez de en una máquina local. Son lo que permite la colaboración sobre un mismo repositorio. Por defecto se le da el nombre de *origin*.

```bash
# Consultar repositorios remotos
git remote

# Añadir repositorio remoto
git remote add <NOMBRE> <URL>

# Descargar cambios del repositorio remoto.
git fetch <REMOTO>

# Mezclar cambios del repositorio remoto
git merge <REMOTO>/<RAMA>

# Descargar y mezclar cambios del repositorio remoto.
git pull <REMOTO>

# Subir cambios a repositorio remoto.
git push <REMOTO> <RAMA>
```
Para subir cambios al remoto, es necesario que el repositorio local tenga los últimos cambios. Por ello, para trabajar se realiza un *pull* primero para obtener el último estado de la rama, y un *push* para enviar cambios una vez se hayan realizado.

# Arquitectura de los sistemas *software*

La **arquitectura del *software*** es su estructura fundamental: elementos, propiedades, relaciones, etc...

- La **especificación** establece cómo queremos que sea.
- La **documentación** describe cómo es.

El diseño y la implementación se desarrollan al mismo tiempo: lo importante son las decisiones de diseño arquitecturales, pero cuestiones menores normalmente se deciden sobre la marcha.

## Vistas arquitecturales

### Vista de módulos

Describe los paquetes o clases que conforman el proyecto así como relaciones fundamentales entre ellos, mostrando cómo esta estructurado. Reflejan el sistema en desarrollo, y permite facilitar repartir el trabajo.

Los módulos describen una unidad de la implementación con responsabilidades coherentes y una interfaz. Normalmente se representan en los lenguajes como clases, espacios de nombres, interfaces, etc...

> TODO: Poner imagen ejemplo UML

Las relaciones se pueden clasificar en:

- **Decomposición**: El módulo está dividido en varios subpaquetes con responsabilidades claras que trabajan en conjunto.
- **Generalización-especialización**: Formada por herencias (módulos padre e hijo), interfaces (módulos con una serie de métodos comunes), etc...
- **Capas**: Agrupaciones de módulos relacionadas de forma ordenada y unidireccional, relaciones de tipo *autorizado a usar*.
- **Asociación, agregación y composición**: Describen relaciones de acceso entre instancias de módulos. La agregación y composición, respectivamente, representan relaciones todo-parte donde las partes pueden existir independientemente del todo o no.
- **Modelo de datos**.

### Vista de componentes y conectores

Se compone de los objetos y/o procesos en tiempo de ejecución junto a los caminos que permiten interactuar entre ellos, como pueden ser las API. Se suelen agrupar en ***tiers***.

Los **componentes** son los procesos, objetos, almacenes de datos, ... , que componen los datos. Los **conectores** son las llamadas a métodos, invocaciones de servicios, colas de mensajes, etc...

> TODO: Poner ejemplo vista componentes

### Vista de despliegue

Refleja la relación entre componentes y conectores, y el hardware donde se ejecutan, en una situación de ejecución con un despliegue concreto. 

> TODO: Poner ejemplo vista despliegue

## Comportamiento

Las comunicaciones son una combinación de estímulos (un componente comunica a otro algo) e intercambio de datos (un componente hace llegar datos a otro).

La comunicación puede ser:

- **Local** o **remota** (en red).
- **Síncrona** (el emisor no sigue hasta que le contestan) o **asíncrona** (el emisor no espera respuesta activamente).
  
La comunicación se puede expresar:

- **Trazas**: secuencia de actividades o interacciones que describen cómo el sistema responde a ciertos estímulos. Se centran en una fracción del comportamiento global entre componentes.
- **Modelos comprensivos**: documentan el comportamiento completo de uno o más elementos estructurales. Se centra en el comportamiento de un grupo de elementos entero.

> TODO: Ejemplo trazas y modelos comprensivos

# Arquetipos de aplicaciones

## Arquetipos de aplicaciones comunes

### Aplicaciones móviles

> TODO: Imágenes aaaaaaaaaaaaaaaaaaaaaaaaaa


Cliente móvil pesado:

- Generalmente siguen el patrón **modelo-vista**.
- La lógica de la aplicación se encuentran en el cliente.
- La BD local actúa como cache de la BD del servidor.

Cliente móvil pesado alternativo:

- Parte de la lógica está implementada en el servidor.
- Opción si se necesita desarrollar múltiples clientes o se comunica con una API web. 

### Aplicaciones web

Cliente web ligero:

- Sigue un patrón **modelo-vista-controlador**.
- El cliente realiza poco procesamiento: el grueso está en el propio servidor.
- Seguida por muchas aplicaciones existentes.

Cliente web pesado:

- Siguen un patrón **cliente-servidor**.
- Parecido a las arquitecturas móviles.
- Se puede ofrecer una API web para su uso por terceros.

### Aplicaciones de escritorio

- Basados en el patrón **modelo-vista**, pero con una UI bastante distinta.
- Suele haber solo una BD.
- El acceso a la BD se puede hacer directamente y sin cifrar si se está en una red confiable.

## Estrategias de despliegue

### Despliegue en 2 niveles

- Despliegue **cliente-servidor**.
- Se separa lógica de la aplicación de la BD.
- Solo se utiliza realmente en aplicaciones de Intranet.

### Despliegue en 3 niveles

- El cliente interactúa con una **aplicación en otro *tier* distinto**.
- El software de aplicación interactúa con la BD.
- Utilizada en aplicaciones y servicios *web* de baja complejidad y simple de escalar.
- Normalmente se establece un **WAF** (*web application firewall*) para filtrar el tráfico HTTP.

### Despliegue en 4 niveles

- Se separa el servidor web del servidor de aplicaciones.

## Bases de datos

Se puede elegir entre:

- **BD relacional (SQL)**
  - Transacciones **ACID**
  - Esquema de datos rígido.
  - Buen rendimiento de consultas para modelos de datos desagregados.
  - Escalado vertical (mejor máquina, mejor rendimiento).
- **No relacionales (NoSQL)**
  - Transacciones limitadas.
  - Esquema de datos flexible.
  - Escalado horizontal (mas máquinas, mas rendimiento).

Se puede utilizar un *framework de persistencia* o mapeador objeto-relacional para facilitar la programación y evitar código repetitivo. 

Es recomendable crear un *pool* de conexiones con la BD, donde se pueda pedir una conexión ya abierta para acceder a ella, pues abrir conexiones es costoso.

## Aspectos transversales 

- Los componentes se deben distribuir sólo lo estrictamente necesario.
- Es mejor interfaces con muchos parámetros que muchas llamadas diferentes.
- Se debe considerar si las comunicaciones contienen estado (lo almacena el servidor por cada petición) o son sin estado (las peticiones vienen acompañadas del estado del cliente).

# Documentación arquitectural



# Construcción automática



# Escritura de documentos técnicos



# Métricas y estimaciones



# Visión general



# Herramientas de planificación



# Entorno industrial de desarrollo



# Legislación



# Gestión del entorno



# Gestión de calidad



# Administración pública



# Gestión de equipo humano



# Trabajo con clientes