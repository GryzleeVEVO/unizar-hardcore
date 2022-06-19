CC = g++
CPPFLAGS = -I -std=c++11
SRC1 = src/ej_1/
SRC2 = src/ej_2/
BLD = build/
BIN = bin/

all: ej1 ej2

# Compilacion
build/prueba_ej1.o: ${SRC1}prueba_ej1.cc
	${CC} -c ${SRC1}prueba_ej1.cc -o ${BLD}prueba_ej1.o ${CPPFLAGS}
build/producto.o: ${SRC1}producto.cc ${SRC1}producto.h
	${CC} -c ${SRC1}producto.cc -o ${BLD}producto.o ${CPPFLAGS}
build/contacto.o: ${SRC1}contacto.cc ${SRC1}contacto.h
	${CC} -c ${SRC1}contacto.cc -o ${BLD}contacto.o ${CPPFLAGS}

build/fechas.o: ${SRC2}fechas.cc ${SRC2}fechas.h
	${CC} -c ${SRC2}fechas.cc -o ${BLD}fechas.o ${CPPFLAGS}

build/programas.o: ${SRC2}programas.cc ${SRC2}programas.h
	${CC} -c ${SRC2}programas.cc -o ${BLD}programas.o ${CPPFLAGS}

build/prueba_ej2.o: ${SRC2}prueba_ej2.cc ${SRC2}coleccionITF.h
	${CC} -c ${SRC2}prueba_ej2.cc -o ${BLD}prueba_ej2.o ${CPPFLAGS}



#linkado

ej1: ${BLD}prueba_ej1.o ${BLD}producto.o ${BLD}contacto.o
	${CC} ${BLD}prueba_ej1.o ${BLD}producto.o ${BLD}contacto.o -o ${BIN}prueba_ej1

ej2: ${BLD}fechas.o ${BLD}programas.o ${BLD}prueba_ej2.o 
	${CC} ${BLD}prueba_ej2.o ${BLD}programas.o ${BLD}fechas.o -o ${BIN}prueba_ej2

#limpieza
clean:
	${RM} ${BLD}*.o