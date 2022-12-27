/*
    BASES DE DATOS
    Practica 2

    Fichero: crear.sql

    Autores:    
        Alvaro Seral Gracia                     819425
        Cristian Andrei Selivanov Dobrisan      816456
        Dorian Boleslaw Wozniak                 817570

    Descripción:
        Sentencias SQL para definir las tablas de la base de datos de cine
*/

-- TABLA DE TODAS LAS PERSONAS QUE PUEDEN ESTAR INVOLUCRADAS
CREATE TABLE Persona (
--  Atributo        Dominio         Restricción
    clvPersona      NUMBER          PRIMARY KEY,
    Nombre          VARCHAR(50)     NOT NULL
);

-- TABLA CON LAS PERSONAS QUE FORMAN PARTE DEL REPARTO
CREATE TABLE Reparto (
--  Atributo        Dominio         Restricción
    clvReparto      NUMBER          PRIMARY KEY REFERENCES Persona(clvPersona) 
);

-- TABLA CON LAS PERSONAS QUE FORMAN PARTE DE LOS ACTORES Y ACTRICES
CREATE TABLE Actor (
--  Atributo        Dominio         Restricción
    clvActor        NUMBER          PRIMARY KEY REFERENCES Persona(clvPersona)
);

-- TABLA CON LOS PERSONAJES QUE APARECEN EN LAS OBRAS
CREATE TABLE Personaje (
--  Atributo        Dominio         Restricción
    clvPersonaje    NUMBER          PRIMARY KEY,
    Descripcion     VARCHAR(100)    NOT NULL
);

-- TABLA CON TODAS LAS OBRAS DISPONIBLES
CREATE TABLE Obra (
--  Atributo        Dominio         Restricción
    clvObra         NUMBER          PRIMARY KEY,
    Titulo          VARCHAR(150)    NOT NULL,
    Estreno         NUMBER(4)       NOT NULL CHECK (Estreno >= 0)
);

-- TABLA CON LOS GENEROS DE CADA UNA DE LAS OBRAS
CREATE TABLE Tema (
--  Atributo        Dominio         Restricción
    Genero          VARCHAR(50),
    clvObra         NUMBER          REFERENCES Obra(clvObra),
    PRIMARY KEY (Genero, clvObra)
);

-- TABLA CON LAS PERSONAS DEL REPARTO QUE PARTICIPAN EN CADA OBRA
CREATE TABLE Participa (
--  Atributo        Dominio         Restricción
    clvReparto      NUMBER          REFERENCES Reparto(clvReparto),
    clvObra         NUMBER          REFERENCES Obra(clvObra),
    PRIMARY KEY (clvReparto, clvObra)
);

-- TABLA CON EL ROL DE CADA UNA DE LAS PARTICIPACIONES DEL REPARTO
CREATE TABLE Labor (
--  Atributo        Dominio         Restricción
    Rol             VARCHAR(50)    CHECK (Rol<>'actor' AND Rol<>'actress'),
    clvReparto      NUMBER,
    clvObra         NUMBER,     
    PRIMARY KEY (Rol, clvReparto, clvObra),
    FOREIGN KEY (clvReparto, clvObra) REFERENCES Participa(clvReparto, clvObra)
);

-- TABLA CON LOS ACTORES QUE PARTICIPAN EN CADA OBRA
CREATE TABLE Actua (
--  Atributo        Dominio         Restricción
    clvActor        NUMBER          REFERENCES Actor(clvActor),
    clvObra         NUMBER          REFERENCES Obra(clvObra),
    PRIMARY KEY (clvActor, clvObra)
);

-- TABLA CON LOS PERSONAJES QUE INTERPRETA CADA ACTOR EN CADA OBRA
CREATE TABLE Interpreta (
--  Atributo        Dominio         Restricción
    clvActor        NUMBER,
    clvObra         NUMBER,
    clvPersonaje    NUMBER          REFERENCES Personaje(clvPersonaje),
    PRIMARY KEY (clvActor, clvObra, clvPersonaje),
    FOREIGN KEY (clvActor, clvObra) REFERENCES Actua(clvActor, clvObra)
);

-- TABLA CON LAS PELICULAS QUE SE ENCUENTRAN EN OBRAS
CREATE TABLE Pelicula (
--  Atributo        Dominio         Restricción
    clvPelicula      NUMBER          PRIMARY KEY REFERENCES Obra(clvObra)
);

-- TABLA CON LAS RELACIONES ENTRE PELICULAS: PRECUELAS, SECUELAS, REMAKES
CREATE TABLE Categoriza (
--  Atributo        Dominio         Restricción
    clvPrincipal    NUMBER          REFERENCES Pelicula(clvPelicula), 
    clvReferencia   NUMBER          REFERENCES Pelicula(clvPelicula),
    Tipo            VARCHAR(50)     NOT NULL,
    PRIMARY KEY (clvPrincipal, clvReferencia),
    CHECK (clvPrincipal <> clvReferencia)
);

-- TABLA CON LAS SERIES QUE SE ENCUENTRAN EN OBRAS
CREATE TABLE Serie (
--  Atributo        Dominio         Restricción
    clvSerie        NUMBER          PRIMARY KEY REFERENCES Obra(clvObra),
    Periodo         VARCHAR(9)      NOT NULL
);

-- TABLA CON LOS CAPITULOS ASIGANDOS A UNA SERIE
CREATE TABLE Capitulo (
--  Atributo        Dominio         Restricción
    clvCapitulo     NUMBER          PRIMARY KEY,
    NombreC         VARCHAR(50)     NOT NULL,
    NumC            NUMBER          CHECK (NumC >= 0),
    NumT            NUMBER          CHECK (NumT >= 0),
    clvSerie        NUMBER          NOT NULL REFERENCES Serie(clvSerie)
);