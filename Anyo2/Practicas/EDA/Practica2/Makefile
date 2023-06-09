CC=g++
CPPFLAGS=-Wall -Wextra -I. -std=c++11
LDFLAGS=
SRC=src/
BLD=build/

all: practica2

build/practica2.o: ${SRC}practica2.cpp ${SRC}coleccionITF.h ${SRC}programas.h ${SRC}fechas.h
	${CC} -c ${SRC}practica2.cpp -o ${BLD}practica2.o ${CPPFLAGS}

build/programas.o: ${SRC}programas.cc ${SRC}programas.h ${SRC}fechas.h
	${CC} -c ${SRC}programas.cc -o ${BLD}programas.o ${CPPFLAGS}

build/fechas.o: ${SRC}fechas.cc ${SRC}fechas.h
	${CC} -c ${SRC}fechas.cc -o ${BLD}fechas.o ${CPPFLAGS}

practica2: ${BLD}practica2.o ${BLD}programas.o ${BLD}fechas.o
	${CC} ${BLD}practica2.o ${BLD}programas.o ${BLD}fechas.o -o practica2

clean:
	${RM} ./build/* practica2