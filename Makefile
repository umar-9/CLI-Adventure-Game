# Makefile

# Output executable
TARGET = main

# Compiler
CXX = g++

# Compiler flags
# CXXFLAGS = -Wall -Wextra

# Source files
SRC_FILES = main.cpp helpers.cpp

# Object files
OBJ_FILES = $(SRC_FILES:.cpp=.o)

# Build rule
$(TARGET): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ_FILES)

# Rule for compiling each source file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(TARGET) $(OBJ_FILES)