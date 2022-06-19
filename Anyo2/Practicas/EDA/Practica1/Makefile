CC = g++
CPPFLAGS = -I -std=c++11
LDFLAGS = 
SRC = src/
BLD = build/

all: practica1

build/practica1.o: ${SRC}practica1.cpp ${SRC}coleccionITF.h ${SRC}programas.h ${SRC}fechas.h
	${CC} -c ${SRC}practica1.cpp -o ${BLD}practica1.o ${CPPFLAGS}

build/programas.o: ${SRC}programas.cc ${SRC}programas.h ${SRC}fechas.h
	${CC} -c ${SRC}programas.cc -o ${BLD}programas.o ${CPPFLAGS}

build/fechas.o: ${SRC}fechas.cc ${SRC}fechas.h
	${CC} -c ${SRC}fechas.cc -o ${BLD}fechas.o ${CPPFLAGS}

practica1: ${BLD}practica1.o ${BLD}programas.o ${BLD}fechas.o
	${CC} ${BLD}practica1.o ${BLD}programas.o ${BLD}fechas.o -o practica1