---
title: Sistemas de Información
subtitle: Resumen
author: Dorian Wozniak
---

# INTRODUCCIÓN

**Datos**: Valores en crudo que representan hechos.

**Información**: Colección de datos organizados de forma que proporcionen valor añadido.

**Conocimiento**: Conciencia o familiaridad adquirida por la experiencia de echos o situaciones a través del aprendizaje, observación o introspección.

**Proceso**: Conjunto de tareas lógicamente relacionadas que a partir de datos de entrada proporciona resultados.

**Algoritmo**: Lista ordenada de pasos o especificación de instrucciones para llevar a cabo una determinada tarea.

**Sistema**: Conjunto de elementos que interactúan para lograr un objetivo

## Sistemas de información

Un **sistema de información** es un sistema que está compuesto por un conjunto de elementos interrelacionados que **recogen** (entrada), **manipulan** (proceso), **almacenan** información, **diseminan** datos (salida) y proporcionan **mecanismos correctores** (retroalimentación) para alcanzar un objetivo.

Se pueden clasificar en:

- **Sistemas de procesado transaccional** (TPS): Gestionan información referente a las transacciones diarias de una organización. Centrado en recolección de datos.
- **Sistemas de información de gestión** (MIS): Orientados a los responsables técnicos de las diferentes áreas de la organización para procedimientos rutinarios.
- **Sistemas de información de apoyo a la toma de decisiones** (DSS): Dan soporte a la toma de decisiones para un problema complejo específico. En general la información a considerar para analizar el problema no está definida.
- **Sistemas de información empresarial** (EIS): DSS para altos ejecutivos, datos solo parcialmente extraidos. Uso informativo para alcanzar objetivos estratégicos.

## Aplicaciones empresariales

Las aplicaciones empresariales presentan las siguientes **características**:

- **Almacenan y manipulan datos**
  - Bases de datos (relacionales, no relacionales, objetuales, etc...)
  - Ficheros (XML, JSON, etc...)
- **Realizan transacciones** (propiedades ACID)
  - Atomicidad
  - Consistencia
  - Aislamiento
  - Durabilidad
- Escalables (horizontal/verticalmente)
- Disponibles (mínimo tiempo sin servicio)
- Seguras
- Integración

Se pueden encontrar en diferentes **arquitecturas**:

- Aplicaciones **monocapa**
  - Modelo de datos sin tener en cuenta integración
  - Persistencia en ficheros
  - Rápidos y de proposito específico
  - No portable, duplicidad
- Aplicaciones de **dos capas**
  - Separación entre interfaz y modelo
  - Cada capa puede ser desarrollada por personal con perfiles específicos
  - Reuso de capa modelo para diferentes dispositivos
  - Cambios en el modelo requieren reinstalación en todos los clientes
- Aplicaciones de **tres capas**
  - Cambios en el modelo solo afectan al servidor de la aplicación
  - Clientes ligeros
  - Cambios en interfaz requieren reinstalación en las máquinas cliente
- Aplicaciones de **tres capas con interfaz web**
  - Cambios en la interfaz solo requieren reinstalación en capa interfaz del servidor de aplicaciones web
  - Los servidores web suelen gestionar bien escalabilidad y disponibilidad
- Aplicaciones de **cuatro capas**
  - Se utiliza cuando la interfaz web y la capa modelo estan construidas con tecnologías diferentes
  
Se utilizan las siguientes **tecnologías**:

- **Acceso a bases de datos**
  - JDBC
  - ODBC
- **Aplicaciones Web**
  - Servlets y JSP (interfaz), Java EE (modelo)
  - ASP.NEet (interfaz), C# y ADO.Net (modelo)
  - Otros (PHP, Ruby on Rails, Python, MEAN,...)
- **Servidores Web**
  - Tomcat, Jboss, Jetty, WebSphere
  - Internet Information Server

## *Cloud computing*

La **computación en la nube** es un paradigma que permite el acceso ubicuo bajo demanda a servicios TIC a través de Internet. Es fruto de externalizar servicios y para ahorrar costes.

Se pueden clasificar en:

- Funcionalidad
  - **Software as a service** (SaaS) (ej. Google Play)
    - Se ofrecen aplicaciones sin controlar cliente, infraestructura ni configuración
    - Clientes comparten infrasetructura, con espacio personal para cada cliente 
  - **Platform as a service** (PaaS) (ej. *Marketplaces*)
    - Entornos de desarrollo comperativo y despliegue rápido
    - Diseñado para sustentar ciclo de vida de apliacaciones
  - **Infrastructure as a service** (IaaS) (ej. AWS)
    - Ofrece recursos de computación
    - Virtualización
    - Eficiente, seguro y baja inversión inicial
- Compartición
  - Público
  - Privado
  - Híbrido
  - Comunitario