CXX = g++

CXXFLAGS = -Wall -Wextra -pedantic -std=c++11 -O3
LDFLAGS =

# External libraries or external includes are not used
LDLIBS =
INCLUDE =

# Directories
SRC_DIR = ./src
OBJ_DIR = ./build

# Get all principal source files and create its dependencies
DEPS = $(wildcard $(SRC_DIR)/*.cc)
OBJS = $(DEPS:$(SRC_DIR)/%.cc=$(OBJ_DIR)/%.o)

# TARGET1 binary to generate
TARGET1 = practica2

# Compile TARGET1 and TARGET2 by default
all: $(TARGET1)

# Compile and link TARGET1 library
$(TARGET1): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) $^ -o $@

# Compile objects
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create builld folder if missing
$(OBJ_DIR):
	mkdir $@

PHONY: clean

clean:
	$(RM) $(TARGET1) $(OBJS) square.dimacs square.sat result.txt