CXX = g++

CXXFLAGS = -std=c++17 -Wall -Wextra -O2

TARGET = driver
COLORING_TARGET = vertex_coloring_driver
PAGERANK_TARGET = pagerank_driver

SRC_DIR = src
DRIVER_DIR = driver
BIN_DIR = bin

ASSIGNMENT_01_DIR = ../assignment_01
COMMON_WRAPPER_DIR = ../common_wrapper

CSR_SRC = $(ASSIGNMENT_01_DIR)/src/csr.cpp
COMMON_WRAPPER_SRC = $(COMMON_WRAPPER_DIR)/assignment_wrapper.cpp

SOURCES = \
	$(DRIVER_DIR)/driver.cpp \
	$(DRIVER_DIR)/vertex_coloring_driver.cpp \
	$(DRIVER_DIR)/pagerank_driver.cpp \
	$(SRC_DIR)/vertex_coloring.cpp \
	$(SRC_DIR)/pagerank.cpp \
	$(CSR_SRC) \
	$(COMMON_WRAPPER_SRC)

OBJECTS = $(SOURCES:.cpp=.o)

INCLUDES = \
	-Iinclude \
	-I$(ASSIGNMENT_01_DIR)/include \
	-I$(COMMON_WRAPPER_DIR)

.PHONY: all clean

all: $(BIN_DIR)/$(TARGET) \
	$(BIN_DIR)/$(COLORING_TARGET) \
	$(BIN_DIR)/$(PAGERANK_TARGET)


# --------------------------------------------------
# Assignment-level driver
# --------------------------------------------------

$(BIN_DIR)/$(TARGET): \
	$(DRIVER_DIR)/driver.o \
	$(COMMON_WRAPPER_DIR)/assignment_wrapper.o

	@mkdir -p $(BIN_DIR)

	$(CXX) $(CXXFLAGS) \
		$(DRIVER_DIR)/driver.o \
		$(COMMON_WRAPPER_DIR)/assignment_wrapper.o \
		-o $@


# --------------------------------------------------
# Standalone Vertex Coloring driver
# --------------------------------------------------

$(BIN_DIR)/$(COLORING_TARGET): \
	$(DRIVER_DIR)/vertex_coloring_driver_standalone.o \
	$(SRC_DIR)/vertex_coloring.o \
	$(ASSIGNMENT_01_DIR)/src/csr.o

	@mkdir -p $(BIN_DIR)

	$(CXX) $(CXXFLAGS) \
		$(DRIVER_DIR)/vertex_coloring_driver_standalone.o \
		$(SRC_DIR)/vertex_coloring.o \
		$(ASSIGNMENT_01_DIR)/src/csr.o \
		-o $@


# --------------------------------------------------
# Standalone PageRank driver
# --------------------------------------------------

$(BIN_DIR)/$(PAGERANK_TARGET): \
	$(DRIVER_DIR)/pagerank_driver_standalone.o \
	$(SRC_DIR)/pagerank.o \
	$(ASSIGNMENT_01_DIR)/src/csr.o

	@mkdir -p $(BIN_DIR)

	$(CXX) $(CXXFLAGS) \
		$(DRIVER_DIR)/pagerank_driver_standalone.o \
		$(SRC_DIR)/pagerank.o \
		$(ASSIGNMENT_01_DIR)/src/csr.o \
		-o $@


# --------------------------------------------------
# Assignment driver
# --------------------------------------------------

$(DRIVER_DIR)/driver.o: $(DRIVER_DIR)/driver.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) \
		-c $< -o $@


# --------------------------------------------------
# Vertex Coloring driver
# --------------------------------------------------

$(DRIVER_DIR)/vertex_coloring_driver.o: \
	$(DRIVER_DIR)/vertex_coloring_driver.cpp

	$(CXX) $(CXXFLAGS) $(INCLUDES) \
		-DVERTEX_COLORING_DRIVER_NO_MAIN \
		-c $< -o $@


# --------------------------------------------------
# Vertex Coloring standalone driver
# --------------------------------------------------

$(DRIVER_DIR)/vertex_coloring_driver_standalone.o: \
	$(DRIVER_DIR)/vertex_coloring_driver.cpp

	$(CXX) $(CXXFLAGS) $(INCLUDES) \
		-c $< -o $@


# --------------------------------------------------
# PageRank driver
# --------------------------------------------------

$(DRIVER_DIR)/pagerank_driver.o: \
	$(DRIVER_DIR)/pagerank_driver.cpp

	$(CXX) $(CXXFLAGS) $(INCLUDES) \
		-DPAGERANK_DRIVER_NO_MAIN \
		-c $< -o $@


# --------------------------------------------------
# PageRank standalone driver
# --------------------------------------------------

$(DRIVER_DIR)/pagerank_driver_standalone.o: \
	$(DRIVER_DIR)/pagerank_driver.cpp

	$(CXX) $(CXXFLAGS) $(INCLUDES) \
		-c $< -o $@


# --------------------------------------------------
# Algorithm sources
# --------------------------------------------------

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) \
		-c $< -o $@


# --------------------------------------------------
# Assignment 01 CSR
# --------------------------------------------------

$(ASSIGNMENT_01_DIR)/src/%.o: \
	$(ASSIGNMENT_01_DIR)/src/%.cpp

	$(CXX) $(CXXFLAGS) $(INCLUDES) \
		-c $< -o $@


# --------------------------------------------------
# Common wrapper
# --------------------------------------------------

$(COMMON_WRAPPER_DIR)/%.o: \
	$(COMMON_WRAPPER_DIR)/%.cpp

	$(CXX) $(CXXFLAGS) $(INCLUDES) \
		-c $< -o $@


# --------------------------------------------------
# Clean
# --------------------------------------------------

clean:
	rm -f $(DRIVER_DIR)/*.o
	rm -f $(SRC_DIR)/*.o
	rm -f $(ASSIGNMENT_01_DIR)/src/csr.o
	rm -f $(COMMON_WRAPPER_DIR)/assignment_wrapper.o
	rm -rf $(BIN_DIR)