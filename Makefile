CXX = g++
CXXFLAGS = -std=c++11 -I.

SRC = aes.cpp main.cpp
OBJ = $(SRC:.cpp=.o)

TARGET = AES_Project

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean
