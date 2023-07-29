PROG:=main

CXX:=g++ -Wall

SRCS:=
SRCS+=$(wildcard *.cc)
SRCS+=$(wildcard *.cpp)
SRCS+=$(wildcard *.c++)

OBJS:=$(addsuffix .o,$(basename $(SRCS)))
DEPS:=$(addsuffix .d,$(basename $(SRCS)))

all: $(PROG)

$(PROG): $(OBJS)
	$(CXX) -o $@ $^

%.o: %.cc
	$(CXX) -MMD -c $<

%.o: %.cpp
	$(CXX) -MMD -c $<

%.o: %.c++
	$(CXX) -MMD -c $<

.PHONY: edit go

go: $(PROG)
	$(PROG)

edit:
	@geany -s -i $(SRCS) *.h &

clean:
	@rm -f $(PROG) *.o *.d core tags

-include $(DEPS)
