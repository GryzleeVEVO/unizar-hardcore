/*
    BASES DE DATOS
    Práctica 1: Liga de fútbol
    Fichero: Crear_Base_Liga.sql
    Autores:    Álvaro Seral Gracia                 816456  816456@unizar.es
                Cristian Andrei Selivanov Dobrisan  819425  819425@unizar.es
                Dorian Boleslaw Wozniak             817570  817570@unizar.es
    Resumen: Fichero de creación de tablas para la base de datos de ligas de la
                práctica 1

    Entregado el 4 de abril de 2022
*/

-- TABLA CON NOMBRES OFICIALES DE LAS LIGAS
CREATE TABLE Liga (
--  Atributo        Dominio         Restricción
    clvLiga         VARCHAR(20)     PRIMARY KEY
);

-- TABLA CON TEMPORADAS SEGÚN AÑO Y DIVISIÓN
CREATE TABLE Temporada (
--  Atributo        Dominio         Restricción
    clvTemporada    NUMBER(4)       PRIMARY KEY,
    anyo            CHAR(9)         NOT NULL,
    clvLiga         VARCHAR(20)     REFERENCES Liga(clvLiga)
);

-- TABLA CON JORNADAS SEGÚN TEMPORADA
CREATE TABLE Jornada (
--  Atributo        Dominio         Restricción
    clvJornada      NUMBER(6)       PRIMARY KEY,
    numero          NUMBER(2)       NOT NULL,
    clvTemporada    NUMBER(4)       REFERENCES Temporada(clvTemporada)
);

-- TABLA CON ESTADIOS
CREATE TABLE Estadio (
--  Atributo        Dominio         Restricción
    clvEstadio      VARCHAR(50)     PRIMARY KEY,
    capacidad       NUMBER(6)       NOT NULL,
    inauguracion    NUMBER(4)       NOT NULL
);

-- TABLA CON EQUIPOS QUE HAN PARTICIPADO EN EL SISTEMA
CREATE TABLE Equipo (
--  Atributo        Dominio         Restricción
    clvEquipo       VARCHAR(20)     PRIMARY KEY,
    nomOficial      VARCHAR(60)     NOT NULL UNIQUE,
    nomHistorico    VARCHAR(60)     NOT NULL,
    ciudad          VARCHAR(60)     NOT NULL,
    fundacion       NUMBER(4)       NOT NULL,
    clvEstadio      VARCHAR(50)     REFERENCES Estadio(clvEstadio)
);

-- TABLA CON APODOS SEGÚN EQUIPO
CREATE TABLE Apodo (
--  Atributo        Dominio         Restricción
    clvEquipo       VARCHAR(20)     REFERENCES Equipo(clvEquipo),
    otroNombre      VARCHAR(50),
    PRIMARY KEY (clvEquipo, otroNombre)
);

-- TABLA CON TODOS LOS PARTIDOS DISPUTADOS
CREATE TABLE Partido (
--  Atributo        Dominio         Restricción
    clvPartido      NUMBER          PRIMARY KEY,
    resLocal        NUMBER(2)       NOT NULL CHECK (resLocal >= 0),
    resVisit        NUMBER(2)       NOT NULL CHECK (resVisit >= 0),
    clvJornada      NUMBER(6)       REFERENCES Jornada(clvJornada),
    clvEqLocal      VARCHAR(20)     REFERENCES Equipo(clvEquipo),
    clvEqVisit      VARCHAR(20)     REFERENCES Equipo(clvEquipo),
    CHECK (clvEqLocal <> clvEqVisit),
    UNIQUE (clvJornada, clvEqLocal),
    UNIQUE (clvJornada, clvEqVisit)
);

-- TABLA CON LAS POSICIONES DE LOS EQUIPOS SEGÚN CLAVE DE JORNADA
CREATE TABLE Compiten (
--  Atributo        Dominio         Restricción
    clvJornada      NUMBER(6)       REFERENCES Jornada(clvJornada),
    clvEquipo       VARCHAR(20)     REFERENCES Equipo(clvEquipo),
    puntuacion      NUMBER(3)       NOT NULL,
    puesto          NUMBER(2)       NOT NULL,
    PRIMARY KEY (clvJornada, clvEquipo)
);
