#=========================================================
# Name: Caleb Morse
# EECS 348 – Lab 10: More C++ Programming
# Date: November 04, 2025
# Description: Build configuration that compiles main.cpp and functions.cpp
# with g++ using C++17 standard, and creates executable "lab10".
# Contributors: Code written and verified by Caleb Morse
#               with support from ChatGPT and Cursor.
#=========================================================

CXX = g++
CXXFLAGS = -std=c++17 -Wall
TARGET = lab10
SOURCES = main.cpp functions.cpp
OBJECTS = $(SOURCES:.cpp=.o)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: clean

