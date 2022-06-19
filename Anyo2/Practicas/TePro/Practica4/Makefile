PROG:=main
SRCS:=

CXX:=g++ -Wall

OBJS:=$(SRCS:.cc=.o)
DEPS:=$(SRCS:.cc=.d)

all: main main-test

main: $(OBJS) main.cc
	$(CXX) -o $@ $^

main-test: $(OBJS) main-test.cc
	$(CXX) -o $@ $^

%.o: %.cc
	$(CXX) -MMD -c $<

.PHONY: edit

edit:
	@geany -s -i $(SRCS) *.h &

clean:
	@rm -f $(PROG) *.o *.d core

-include $(DEPS)
