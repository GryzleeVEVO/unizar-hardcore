---
title: Ingeniería del Software
subtitle: Resumen
author: Dorian Wozniak
---

# FASE DE REQUISITOS

## **Requisitos**

Los **requisitos** son características que deben incluirse en el sistema.

Los requisitos se pueden dividir en:

- Requisitos de **usuario**: Condiciones o capacidades necesarias para que el usuario pueda resolver un problema o alcanzar un objetivo.
- Requisitos del **sistema**: Condiciones o capacidades que debe reunir el sistema para satisfacer un contrato, estándar o cualquier otro documento formalmente impuesto.
    - Requisitos **funcionales**: Establecen la funcionalidad o comportamiento del sistema. Describen las interacciones entre sistema y entorno.
    - Requisitos **no funcionales**: Criterios para juzgar cómo funciona el sistema. Son atributos de calidad.

Todo aquello que no indica qué hace o como opera el software no es un criterio. Cuestiones como tecnología de implementación, entorno de desarrollo, metodología y/o arquitectura son **restriccions**.

## **Fase de requisitos**

La **fase de requisitos** consiste en la recopilación de información y hechos para representar los requisitos.

Algunas de las técnicas de recopilación de información son:

- Entrevistas
  - Fundamental para confeccionar requisitos. Maximizar información por sesión.
  - Objetivo definido, preguntas directas, entrevistado es persona clave.
- Reuniones
  - Varios entrevistados y entrevistadores
  - Agenda formal (puntos importantes) + informal (*brainstorming*)
- Cuestionarios
  - Contiene todas las preguntas que busca el analista.
  - Útil para obtener la misma información de muchos usuarios, especialmente datos numéricos y opciones símples.
  - Fácil de distribuir
  

Otras fuentes de información son:

- Formularios y documentos
- Manuales de procedimiento
- Informes
- Programas
- etc...

## **Representación de requisitos**

Una vez recopilados, los requisitos se sintetizan en un catálogo:

### Tabla de requisitos

Los requisitos son representados en una tabla. Cada requisito tiene un código y una descripción. Los requisitos se pueden agrupar o ordenar tal que sea mas conveniente.

**Ej**: "Construir tabla de requisitos para la aplicación "Gestión Aula Informática""

Un aula informática de una escuela universitaria tiene la siguiente política de reservas:

- Los alumnos pueden reservar un máximo de 4 horas a la semana.
- Las reservas pueden realizarse hasta el día anterior al uso.
- Cuando un alumno hace una reserva A, se le entrega un justificante de la misma. El formato del justificante es el siguiente.

![Formato justificante](./IngSoft/resources/gestion_aula_info1.jpg){ height=70px }

- Cuando se abre el Aula hay un listado de las reservas realizadas sobre cada equipo, de modo que la gente puede usar los equipos no reservados. El formato del listado de reservas es el siguiente:

![Formato listado](./IngSoft/resources/gestion_aula_info2.jpg){ height=100px }

- El horario del aula es de 10:00 a 18:00 todos los días lectivos y las reservas comienzan siempre a una hora en punto

----- -------------------------------------------------------------------
Cod   Descripción                                                                                                                                                                                
----- -------------------------------------------------------------------
RF-1  El sistema debe permitir reservar equipos del aula informático a alumnos   
                                                                                                                
RF-2  Un alumno puede reservar equipos como máximo 4 horas cada semana  
                                                                                                                         
RF-3  Se debe entregar al alumno un justificante al realizar una reserva 
                                                                                                                        
RF-4  Las reservas deben ser para un dia posterior al actual                                 
                                                                                                    
RF-5  El sistema generará un listado con las reservas realizadas de los equipos en un día dado        
                                                                                           
RF-6  Las reservas de un equipo deben realizarse en el horario entre las 10 y las 18, comenzando en punto         
                                                                               
RF-7  Las reservas deben realizarse en horario lectivo                                             
                                                                                              
RNF-1 El formato de un justificante debe incluir un número de expediente; el nombre y apellidos de quien realiza la reserva; el número del equipo reservado; y el día y la hora de la reserva  
  
RNF-2 El formato de un listado para un día dado debe incluir el día al que pertenecen las reservas; y para cada entrada de la lista, el equipo, hora y nombre del alumno que realiza la reserva 
 
----- -------------------------------------------------------------------

### Árboles y tablas de decisión

Si un requisito describe una toma de decisión, se puede especificar mediante un árbol/tabla de decisión.

Un árbol de decisión la raíz comienza la secuencia de decisiones, yendo a una rama según las condiciones que cumpla.

**Ej**: A un cliente se le dará trato preferencial si cumple una de estas tres condiciones:

- Compra más de 10.000 € por año y tiene una buena historia de pagos
- Compra más de 10.000 € por año y ha comerciado con nosotros por más de 20 años
- Compran 10.000 € o menos por año, pero tiene una buena historia de pagos 

![Árbol de decisión](./IngSoft/resources/arbol_decisiones.jpg){ height=250px }

Una tabla se divide en dos partes y cuatro secciones:

| Tabla de decisiones           |                                                            |
| ----------------------------- | ---------------------------------------------------------- |
| Identificación de condiciones | Entrada de condiciones (1 condición /columna)              |
| Identificación de acciones    | Entrada de acciones (reglas de decisión marcadas con cruz) |

**Ej**: (cont anterior)

|                                                        |     |     |     |     |     |     |     |     |
| ------------------------------------------------------ | --- | --- | --- | --- | --- | --- | --- | --- |
| C1: ¿El cliente tiene mas de 10000€ de compras al año? | S   | S   | S   | S   | N   | N   | N   | N   |
| C2: ¿El cliente tiene buena historia de pagos?         | S   | S   | N   | N   | S   | S   | N   | N   |
| C3: ¿EL cliente ha comerciado durante mas de 20 años?  | S   | N   | S   | N   | S   | N   | S   | N   |
| A1: Trato normal                                       |     |     |     | x   |     |     | x   | x   |
| A2: Trato preferencial                                 | x   | x   | x   |     | x   | x   |     |     |

## **Validación de requisitos**

La **validación** es un proceso manual donde se comprueba que los requisitos:

- Son precisos y claros 
- Es válido 
- Consistencia 
- Completitud 
- Realizable 
- Verificable 
- Trazable
- Manejable
- Libre de detalles de implementación

[//]: <> (TODO: Ejemplo validación requisitos)