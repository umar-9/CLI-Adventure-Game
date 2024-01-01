# Makefile

# Output executable
TARGET = main

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra

# Source files
SRC_FILES = main.cpp helpers.cpp

# Object files
OBJ_FILES = $(SRC_FILES:.cpp=.o)

# Release build flags
RELEASE_CXXFLAGS = -O2 -DNDEBUG
RELEASE_TARGET = $(TARGET)_release

# Debug build flags
DEBUG_CXXFLAGS = -g
DEBUG_TARGET = $(TARGET)_debug

# Build rules
release: $(OBJ_FILES)
	$(CXX) $(RELEASE_CXXFLAGS) -o $(RELEASE_TARGET) $(OBJ_FILES)

debug: $(OBJ_FILES)
	$(CXX) $(DEBUG_CXXFLAGS) -o $(DEBUG_TARGET) $(OBJ_FILES)

# Rule for compiling each source file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule
clean:
	rm -f $(TARGET) $(RELEASE_TARGET) $(DEBUG_TARGET) $(OBJ_FILES)