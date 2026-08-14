CXX = g++

CXXFLAGS = -std=c++17 -Wall -Wextra -O2

INCLUDE_DIR = include
DRIVER_DIR = driver
SRC_DIR = src
BIN_DIR = bin

COMMON_DIR = ../common_wrapper

ASSIGNMENT_DRIVER = $(BIN_DIR)/driver
GEMM_DRIVER = $(BIN_DIR)/gemm_driver
CSR_DRIVER = $(BIN_DIR)/csr_driver


.PHONY: all clean


all: $(ASSIGNMENT_DRIVER) \
     $(GEMM_DRIVER) \
     $(CSR_DRIVER)


# Create bin directory if it does not exist.
$(BIN_DIR):
	mkdir -p $(BIN_DIR)


# --------------------------------------------------
# Assignment-level driver
# --------------------------------------------------

$(ASSIGNMENT_DRIVER): \
	$(DRIVER_DIR)/driver.cpp \
	$(COMMON_DIR)/assignment_wrapper.cpp \
	$(COMMON_DIR)/assignment_wrapper.h \
	$(INCLUDE_DIR)/assignment_config.h

	$(CXX) $(CXXFLAGS) \
		-I$(INCLUDE_DIR) \
		-I$(COMMON_DIR) \
		$(DRIVER_DIR)/driver.cpp \
		$(COMMON_DIR)/assignment_wrapper.cpp \
		-o $(ASSIGNMENT_DRIVER)


# --------------------------------------------------
# GEMM driver
# --------------------------------------------------

$(GEMM_DRIVER): \
	$(DRIVER_DIR)/gemm_driver.cpp \
	$(SRC_DIR)/gemm.cpp \
	$(INCLUDE_DIR)/gemm.h \
	$(INCLUDE_DIR)/gemm_driver.h

	$(CXX) $(CXXFLAGS) \
		-I$(INCLUDE_DIR) \
		$(DRIVER_DIR)/gemm_driver.cpp \
		$(SRC_DIR)/gemm.cpp \
		-o $(GEMM_DRIVER)


# --------------------------------------------------
# CSR driver
# --------------------------------------------------

$(CSR_DRIVER): \
	$(DRIVER_DIR)/csr_driver.cpp \
	$(SRC_DIR)/csr.cpp \
	$(INCLUDE_DIR)/csr.h \
	$(INCLUDE_DIR)/csr_driver.h

	$(CXX) $(CXXFLAGS) \
		-I$(INCLUDE_DIR) \
		$(DRIVER_DIR)/csr_driver.cpp \
		$(SRC_DIR)/csr.cpp \
		-o $(CSR_DRIVER)


# --------------------------------------------------
# Clean
# --------------------------------------------------

clean:
	rm -rf $(BIN_DIR)