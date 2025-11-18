# === Star Formation Simulation Makefile ===

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -O2 -Iinclude

# Source and target
SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)
TARGET = star_formation_sim

# Default rule
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJ)
	@echo "Linking..."
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile each source file into an object file
%.o: %.cpp
	@echo "Compiling $< ..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run the program
run: $(TARGET)
	@echo "Running simulation..."
	./$(TARGET)

# Clean up build files
clean:
	@echo "Cleaning up..."
	rm -f $(OBJ) $(TARGET)


tests: $(OBJ)
	$(CXX) $(CXXFLAGS) -o test_two_body tests/test_two_body.cpp $(OBJ)
	$(CXX) $(CXXFLAGS) -o test_freefall tests/test_freefall.cpp $(OBJ)


.PHONY: all run clean
