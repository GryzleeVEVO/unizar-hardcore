#	Dorian Boleslaw Wozniak	817570
#	Practica 1 Tecnología de Programación
#	Makefile
#	Modificado para compilar adicionalmente main-dynamic-stack y main

.SUFFIXES: .cc .o

PROG:=main-static-stack-struct \
main-static-stack \
main-dynamic-stack \
main
SRCS:=main-static-stack-struct.cc \
main-static-stack.cc \
main-dynamic-stack.cc \
main.cc

CXX:=g++ -Wall

all: $(PROG)

%: %.o
	$(CXX) -o $@ $^

%.o: %.cc
	$(CXX) -c $<

.PHONY: edit go

edit:
	@geany -s -i $(SRCS) &

go:
	@$(PROG)

clean:
	@rm -f $(PROG) *.o core tags *.gch

main-static-stack-struct.o: static-stack-struct.h
main-static-stack.o: static-stack.h
main-dynamic-stack.o: dynamic-stack.h
main.o: static-stack.h dynamic-stack.h
