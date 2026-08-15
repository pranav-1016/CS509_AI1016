CXX = g++

CXXFLAGS = -std=c++17 -Wall -Wextra -O2

TARGET = driver
MST_TARGET = mst_driver

SRC_DIR = src
DRIVER_DIR = driver
BIN_DIR = bin

ASSIGNMENT_01_DIR = ../assignment_01
COMMON_WRAPPER_DIR = ../common_wrapper

CSR_SRC = $(ASSIGNMENT_01_DIR)/src/csr.cpp
COMMON_WRAPPER_SRC = $(COMMON_WRAPPER_DIR)/assignment_wrapper.cpp

SOURCES = \
	$(DRIVER_DIR)/driver.cpp \
	$(DRIVER_DIR)/mst_driver.cpp \
	$(SRC_DIR)/prims.cpp \
	$(SRC_DIR)/kruskal.cpp \
	$(SRC_DIR)/dsu.cpp \
	$(CSR_SRC) \
	$(COMMON_WRAPPER_SRC)

OBJECTS = $(SOURCES:.cpp=.o)

INCLUDES = \
	-Iinclude \
	-I$(ASSIGNMENT_01_DIR)/include \
	-I$(COMMON_WRAPPER_DIR)

.PHONY: all clean

all: $(BIN_DIR)/$(TARGET) $(BIN_DIR)/$(MST_TARGET)


# --------------------------------------------------
# Assignment-level driver
# --------------------------------------------------

$(BIN_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@


# --------------------------------------------------
# Standalone MST driver
# --------------------------------------------------

$(BIN_DIR)/$(MST_TARGET): $(DRIVER_DIR)/mst_driver_standalone.o \
	$(SRC_DIR)/prims.o \
	$(SRC_DIR)/kruskal.o \
	$(SRC_DIR)/dsu.o \
	$(ASSIGNMENT_01_DIR)/src/csr.o

	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) \
		$(DRIVER_DIR)/mst_driver_standalone.o \
		$(SRC_DIR)/prims.o \
		$(SRC_DIR)/kruskal.o \
		$(SRC_DIR)/dsu.o \
		$(ASSIGNMENT_01_DIR)/src/csr.o \
		-o $@


# --------------------------------------------------
# Assignment driver
# --------------------------------------------------

$(DRIVER_DIR)/driver.o: $(DRIVER_DIR)/driver.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@


# --------------------------------------------------
# MST driver for assignment-level executable
# --------------------------------------------------

$(DRIVER_DIR)/mst_driver.o: $(DRIVER_DIR)/mst_driver.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) \
		-DMST_DRIVER_NO_MAIN \
		-c $< -o $@


# --------------------------------------------------
# MST driver standalone version
# --------------------------------------------------

$(DRIVER_DIR)/mst_driver_standalone.o: $(DRIVER_DIR)/mst_driver.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) \
		-c $< -o $@


# --------------------------------------------------
# Algorithm sources
# --------------------------------------------------

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@


# --------------------------------------------------
# Assignment 01 CSR
# --------------------------------------------------

$(ASSIGNMENT_01_DIR)/src/%.o: $(ASSIGNMENT_01_DIR)/src/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@


# --------------------------------------------------
# Common wrapper
# --------------------------------------------------

$(COMMON_WRAPPER_DIR)/%.o: $(COMMON_WRAPPER_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@


# --------------------------------------------------
# Clean
# --------------------------------------------------

clean:
	rm -f $(DRIVER_DIR)/*.o
	rm -f $(SRC_DIR)/*.o
	rm -f $(ASSIGNMENT_01_DIR)/src/csr.o
	rm -f $(COMMON_WRAPPER_DIR)/assignment_wrapper.o
	rm -rf $(BIN_DIR)