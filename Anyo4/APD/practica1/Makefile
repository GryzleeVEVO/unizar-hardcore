CXX = g++

CXXFLAGS = -Wall -Wextra -pedantic -std=c++11
LDFLAGS =

# External libraries or external includes are not used
LDLIBS =
INCLUDE =

# Directories
SRC_DIR = ./src
TST_DIR = ./test
OBJ_DIR = ./build

# Get all principal source files and create its dependencies
DEPS = $(wildcard $(SRC_DIR)/*.cc)
OBJS = $(DEPS:$(SRC_DIR)/%.cc=$(OBJ_DIR)/%.o)

# Get all test source files and create its dependencies
DEPS_TST = $(wildcard $(TST_DIR)/*.cc)
OBJS_TST = $(DEPS_TST:$(TST_DIR)/%.cc=$(OBJ_DIR)/%.o)

# TARGET1 binary to generate
TARGET1 = practica1
TARGET2 = generarPruebas

# Compile TARGET1 and TARGET2 by default
all: $(TARGET1) $(TARGET2)

# Compile and link TARGET1 library
$(TARGET1): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) $^ -o $@

# Compile objects
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile and link TARGET1 library
$(TARGET2): $(OBJS_TST)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(LDLIBS) $^ -o $@

# Compile objects
$(OBJ_DIR)/%.o: $(TST_DIR)/%.cc | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create builld folder if missing
$(OBJ_DIR):
	mkdir $@

PHONY: clean

clean:
	$(RM) $(TARGET1) $(TARGET2) $(OBJS)