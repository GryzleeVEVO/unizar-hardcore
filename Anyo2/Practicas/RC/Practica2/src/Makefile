# Makefile escrito por Jesús Alastruey Benedé (jalastru@unizar.es)

# Selección de arquitectura: x86/sparc
#    a=0 genera binario x86 (valor por defecto)
#    a=1 genera binario sparc
# Puede cambiarse el valor por defecto desde la línea de comandos:
#    "make -f Makefile a=1"
# usará el valor a=1, independientemente del valor por defecto.
a=0

# Selección del compilador
CC = gcc

CFLAGS = -Wall -Wextra -Wshadow

ifeq ($(a),0)
    LIBRARIES =
else
    LIBRARIES = -lsocket -lnsl
endif

INCLUDES =

#
# Directorios
#
OBJDIR := obj
BINDIR := bin

#
# todas las fuentes
#
# SRCS = $(wildcard *.c)
# HDRS = $(wildcard *.h)

#
# fuentes de cada binario
#
MIGETADDRINFO_SRCS = comun.c migetaddrinfo.c
CLIENTEVOCALESTCP_SRCS = comun.c clientevocalesTCP.c
SERVIDORVOCALESTCP_SRCS = comun.c servidorvocalesTCP.c

# replace .c by .o and prepend $OBJDIR
MIGETADDRINFO_OBJS  = $(patsubst %.c, $(OBJDIR)/%.o, $(MIGETADDRINFO_SRCS))
CLIENTEVOCALESTCP_OBJS  = $(patsubst %.c, $(OBJDIR)/%.o, $(CLIENTEVOCALESTCP_SRCS))
SERVIDORVOCALESTCP_OBJS = $(patsubst %.c, $(OBJDIR)/%.o, $(SERVIDORVOCALESTCP_SRCS))

#
# all target
#
all: $(BINDIR)/migetaddrinfo  $(BINDIR)/clientevocalesTCP  $(BINDIR)/servidorvocalesTCP
	@echo "compilacion finalizada"

#
# alias to avoid errors when executing, for instance, $ make migetaddrinfo
#
migetaddrinfo: $(BINDIR)/migetaddrinfo
	@echo "HOLA" > /dev/null
clientevocalesTCP: $(BINDIR)/clientevocalesTCP
	@echo "HOLA" > /dev/null
servidorvocalesTCP: $(BINDIR)/servidorvocalesTCP
	@echo "HOLA" > /dev/null	

#
# dependencies to force the creation of the $(OBJDIR) and $(BINDIR) directories
# @: suppress the echoing of the command
#
$(OBJDIR):
	@mkdir -p $@
$(BINDIR):
	@mkdir -p $@

#
# La misma regla de compilación para todos los ficheros
# Para evitar problemas, debería forzar que cada fichero .c dependiera de todos los .h
# pero no funciona correctamente
# $(OBJDIR)/%.o: %.c $(HDRS) | $(OBJDIR)
# Para hacer que solamente dependa del propio:
# $(OBJDIR)/%.o: %.c %.h | $(OBJDIR)
#
$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@
#    $(info HDRS is $(HDRS))
#    $(info SRCS is $(SRCS))

#
# En caso de que se necesitaran distintas líneas de compilación,
# habría que usar el procedimiento descrito en el siguiente enlace:
# https://www.gnu.org/software/make/manual/html_node/Static-Usage.html
#
# $<: name of the first prerequisite
# $@: file name of the target of the rule
# http://www.gnu.org/software/make/manual/make.html#Automatic-Variables
#
#$(patsubst %.c, $(OBJDIR)/%.o, $(SRCS)): $(OBJDIR)/%.o: %.c | $(OBJDIR)
#	$(CC) $(CFLAGS) -c $< -o $@


#
# binaries
# $^: names of all the prerequisites
# http://www.gnu.org/software/make/manual/make.html#Automatic-Variables
#
$(BINDIR)/migetaddrinfo: ${MIGETADDRINFO_OBJS}  |  $(BINDIR)
	$(CC) $(CFLAGS) $(INCLUDES)  $^  $(LIBRARIES) -o $@
#	strip $@

$(BINDIR)/clientevocalesTCP: ${CLIENTEVOCALESTCP_OBJS}  |  $(BINDIR)
	$(CC) $(CFLAGS) $(INCLUDES)  $^  $(LIBRARIES)  -o $@
#	strip $@

$(BINDIR)/servidorvocalesTCP: ${SERVIDORVOCALESTCP_OBJS}  |  $(BINDIR)
	$(CC) $(CFLAGS) $(INCLUDES)  $^  $(LIBRARIES)  -o $@
#	strip $@

#
# other targets
#
clean:
	rm -f  core  $(OBJDIR)/*.o  $(OBJDIR)/*.d  $(BINDIR)/migetaddrinfo  $(BINDIR)/clientevocalesTCP  $(BINDIR)/servidorvocalesTCP

flags:
	@$(CC) $(CFLAGS) -E -v - </dev/null 2>&1 | grep cc1

gccversion:
	@$(CC) -v 2>&1 | tail -1

.PHONY: clean all flags gccversion
