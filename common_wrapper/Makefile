CXX = g++

CXXFLAGS = -std=c++17 -Wall -Wextra -O2


TARGET = wrapper

SRC = wrapper.cpp

HEADERS = \
	wrapper_config.h \
	assignment_wrapper.h \
	assignment_config.h \
	utils.h


.PHONY: all clean


all: $(TARGET)


$(TARGET): $(SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) \
		$(SRC) \
		-o $(TARGET)


clean:
	rm -f $(TARGET)