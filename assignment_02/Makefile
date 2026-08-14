CXX = g++

CXXFLAGS = -std=c++17 -Wall -Wextra -O2


# ------------------------------------------------------------
# Directories
# ------------------------------------------------------------

BIN_DIR = bin

ASSIGNMENT_01 = ../assignment_01
COMMON_WRAPPER = ../common_wrapper

A1_INCLUDE = $(ASSIGNMENT_01)/include
A1_SRC = $(ASSIGNMENT_01)/src


# ------------------------------------------------------------
# Include paths
# ------------------------------------------------------------

INCLUDES = -Iinclude \
           -I$(A1_INCLUDE) \
           -I$(COMMON_WRAPPER)


# ------------------------------------------------------------
# Assignment 1 CSR dependency
# ------------------------------------------------------------

CSR_SRC = $(A1_SRC)/csr.cpp
CSR_OBJ = $(BIN_DIR)/csr.o


# ------------------------------------------------------------
# Common Wrapper dependency
# ------------------------------------------------------------

WRAPPER_SRC = $(COMMON_WRAPPER)/assignment_wrapper.cpp
WRAPPER_OBJ = $(BIN_DIR)/assignment_wrapper.o


# ------------------------------------------------------------
# Bellman-Ford
# ------------------------------------------------------------

BF_SRC = src/bellman_ford.cpp
BF_DRIVER_SRC = driver/bellman_ford_driver.cpp

BF_OBJ = $(BIN_DIR)/bellman_ford.o
BF_DRIVER_OBJ = $(BIN_DIR)/bellman_ford_driver.o

BF_TARGET = $(BIN_DIR)/bellman_ford_driver


# ------------------------------------------------------------
# Floyd-Warshall
# ------------------------------------------------------------

FW_SRC = src/floyd_warshall.cpp
FW_DRIVER_SRC = driver/floyd_warshall_driver.cpp

FW_OBJ = $(BIN_DIR)/floyd_warshall.o
FW_DRIVER_OBJ = $(BIN_DIR)/floyd_warshall_driver.o

FW_TARGET = $(BIN_DIR)/floyd_warshall_driver


# ------------------------------------------------------------
# Assignment-level driver
# ------------------------------------------------------------

ASSIGNMENT_DRIVER_SRC = driver/driver.cpp
ASSIGNMENT_DRIVER_OBJ = $(BIN_DIR)/driver.o

ASSIGNMENT_TARGET = $(BIN_DIR)/driver


# ------------------------------------------------------------
# Default target
# ------------------------------------------------------------

.PHONY: all

all: directories $(ASSIGNMENT_TARGET) $(BF_TARGET) $(FW_TARGET)


# ------------------------------------------------------------
# Create bin directory
# ------------------------------------------------------------

.PHONY: directories

directories:
	mkdir -p $(BIN_DIR)


# ------------------------------------------------------------
# Assignment 1 CSR
# ------------------------------------------------------------

$(CSR_OBJ): $(CSR_SRC) $(A1_INCLUDE)/csr.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@


# ------------------------------------------------------------
# Common Wrapper
# ------------------------------------------------------------

$(WRAPPER_OBJ): $(WRAPPER_SRC) \
                $(COMMON_WRAPPER)/assignment_wrapper.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@


# ------------------------------------------------------------
# Bellman-Ford implementation
# ------------------------------------------------------------

$(BF_OBJ): $(BF_SRC) \
           include/bellman_ford.h \
           $(A1_INCLUDE)/csr.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@


# ------------------------------------------------------------
# Bellman-Ford driver
# ------------------------------------------------------------

$(BF_DRIVER_OBJ): $(BF_DRIVER_SRC) \
                  include/bellman_ford.h \
                  include/assignment_config.h \
                  $(A1_INCLUDE)/csr.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@


# ------------------------------------------------------------
# Bellman-Ford executable
# ------------------------------------------------------------

$(BF_TARGET): $(BF_OBJ) $(BF_DRIVER_OBJ) $(CSR_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@


# ------------------------------------------------------------
# Floyd-Warshall implementation
# ------------------------------------------------------------

$(FW_OBJ): $(FW_SRC) \
           include/floyd_warshall.h \
           $(A1_INCLUDE)/csr.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@


# ------------------------------------------------------------
# Floyd-Warshall driver
# ------------------------------------------------------------

$(FW_DRIVER_OBJ): $(FW_DRIVER_SRC) \
                  include/floyd_warshall.h \
                  include/assignment_config.h \
                  $(A1_INCLUDE)/csr.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@


# ------------------------------------------------------------
# Floyd-Warshall executable
# ------------------------------------------------------------

$(FW_TARGET): $(FW_OBJ) $(FW_DRIVER_OBJ) $(CSR_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@


# ------------------------------------------------------------
# Assignment-level driver
# ------------------------------------------------------------

$(ASSIGNMENT_DRIVER_OBJ): $(ASSIGNMENT_DRIVER_SRC) \
                          include/assignment_config.h \
                          $(COMMON_WRAPPER)/assignment_wrapper.h
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@


# ------------------------------------------------------------
# Assignment-level executable
# ------------------------------------------------------------

$(ASSIGNMENT_TARGET): $(ASSIGNMENT_DRIVER_OBJ) $(WRAPPER_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $@


# ------------------------------------------------------------
# Individual targets
# ------------------------------------------------------------

.PHONY: bellman_ford

bellman_ford: $(BF_TARGET)


.PHONY: floyd_warshall

floyd_warshall: $(FW_TARGET)


.PHONY: assignment

assignment: $(ASSIGNMENT_TARGET)


# ------------------------------------------------------------
# Clean
# ------------------------------------------------------------

.PHONY: clean

clean:
	rm -rf $(BIN_DIR)