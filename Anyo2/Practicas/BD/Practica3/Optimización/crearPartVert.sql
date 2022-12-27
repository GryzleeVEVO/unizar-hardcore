/*
    BASES DE DATOS
    Practica 3

    Fichero: crear.sql

    Autores:    
        Alvaro Seral Gracia                     819425
        Cristian Andrei Selivanov Dobrisan      816456
        Dorian Boleslaw Wozniak                 817570

    Descripción:
        Sentencias SQL para definir las tablas de la base de datos
*/

CREATE TABLE Aeropuerto (
--  Atributo        Dominio         Restriccion
    IATA            VARCHAR(4)      PRIMARY KEY,
    NombreA         VARCHAR(50)     NOT NULL,
    Ciudad          VARCHAR(50),
    Estado          VARCHAR(2)
);

CREATE TABLE Fabricante (
--  Atributo        Dominio         Restriccion
    NombreF         VARCHAR(50)     PRIMARY KEY
);

CREATE TABLE Modelo (
--  Atributo        Dominio         Restriccion
    nombreM         VARCHAR(50),
    clvFabricante   VARCHAR(50)     NOT NULL REFERENCES Fabricante(NombreF),
    Motor           VARCHAR(50)     NOT NULL,
    PRIMARY KEY (nombreM, clvFabricante)
);

CREATE TABLE Avion (
--  Atributo        Dominio         Restriccion
    Matricula       VARCHAR(7)       PRIMARY KEY,
    Montaje         NUMBER(4),
    clvModelo       VARCHAR(50),
    clvFabricante   VARCHAR(50),
    FOREIGN KEY (clvModelo, clvFabricante) REFERENCES Modelo(NombreM,clvFabricante)
);

CREATE TABLE Compania (
--  Atributo        Dominio         Restriccion
    idCompania      VARCHAR(7)      PRIMARY KEY,
    NombreC         VARCHAR(100)    UNIQUE NOT NULL
);

CREATE TABLE VueloPrincipal (
--  Atributo        Dominio         Restriccion
    idVuelo         NUMBER          PRIMARY KEY,
    clvOrigen       VARCHAR(4)      NOT NULL REFERENCES Aeropuerto(IATA),
    clvDestino      VARCHAR(4)      NOT NULL REFERENCES Aeropuerto(IATA),
    clvCompania     VARCHAR(7)      NOT NULL REFERENCES Compania(idCompania),
    clvAvion        VARCHAR(7)      REFERENCES Avion(Matricula),
    CHECK (clvOrigen <> clvDestino)
);

CREATE TABLE VueloSecundario (
--  Atributo        Dominio         Restriccion
    idVuelo         NUMBER          PRIMARY KEY REFERENCES VueloPrincipal(idVuelo),
    numVuelo        NUMBER(4)       NOT NULL,
    Salida          NUMBER(4)       NOT NULL CHECK (Salida >=0),
    Llegada         NUMBER(4)       NOT NULL CHECK (Llegada >=0),
    Fecha           VARCHAR(10)     NOT NULL,
    Cancelado       NUMBER(1)       NOT NULL CHECK (Cancelado='0' OR Cancelado='1'),
    Retraso         NUMBER(3)       CHECK (Retraso >0),
    CHECK ((Cancelado=1 AND Retraso=NULL) OR Cancelado=0)
);

CREATE VIEW Vuelo AS
  SELECT
    VP.idVuelo,         
    numVuelo,        
    Salida,          
    Llegada,         
    Fecha,           
    Cancelado,       
    Retraso,         
    clvOrigen,       
    clvDestino,     
    clvCompania,     
    clvAvion       
  FROM VueloPrincipal VP, VueloSecundario VS;

CREATE TABLE Desvio (
--  Atributo        Dominio         Restriccion
    Escala          NUMBER(1)       CHECK (Escala='1' OR Escala='2'),
    clvVuelo        NUMBER          NOT NULL REFERENCES VueloPrincipal(idVuelo),
    clvAeropuerto   VARCHAR(4)      NOT NULL REFERENCES Aeropuerto(IATA),
    clvAvion        VARCHAR(7)      REFERENCES Avion(Matricula),
    PRIMARY KEY (Escala, clvVuelo)
);