# === Star Formation Simulation Makefile ===

CXX = g++
CXXFLAGS += -std=c++17 -O0 -g -pg -Iinclude
LDFLAGS  += -pg

# CXXFLAGS += -std=c++17 -O2 -g -pg -Iinclude
# LDFLAGS  += -pg


SRC_DIR = src
SRC = $(wildcard $(SRC_DIR)/*.cpp)

OBJ_DIR = bin
OBJ = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))

BIN_DIR = bin
# TARGET = $(BIN_DIR)/simulation
TARGET = $(OBJ_DIR)/simulation

TEST_DIR = tests
TEST_EXEC = $(TEST_DIR)/test_two_body
TEST_FREEFALL = $(TEST_DIR)/test_freefall
TEST_MOMENTUM_EXEC = $(TEST_DIR)/test_momentum


# Default rule
all: $(OBJ_DIR) $(TARGET)

# Ensure directories exist
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(TEST_DIR):
	mkdir -p $(TEST_DIR)


# Build main simulation
$(TARGET): $(OBJ)
	@echo "Linking simulation..."
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile source files into bin/
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling $< ..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run simulation
run: $(TARGET)
	./$(TARGET)


# ===== Tests =====
OBJS = src/gravity.o src/init.o src/integrator.o src/utils.o src/density.o src/hydro.o

test_two_body: $(TEST_DIR) tests/test_two_body.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) -g -o $(TEST_EXEC) tests/test_two_body.cpp $(OBJS)

test_freefall: $(TEST_DIR) tests/test_freefall.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) -g -o $(TEST_FREEFALL) tests/test_freefall.cpp $(OBJS)

test_momentum: $(TEST_DIR) tests/test_momentum.cpp $(OBJS)
	$(CXX) $(CXXFLAGS) -g -o $(TEST_MOMENTUM_EXEC) tests/test_momentum.cpp $(OBJS)



# Build test_two_body without linking main.cpp
# test_two_body: $(TEST_DIR) tests/test_two_body.cpp src/gravity.o src/init.o src/integrator.o src/utils.o src/density.o src/hydro.o
# 	$(CXX) $(CXXFLAGS) -g -o $(TEST_EXEC) tests/test_two_body.cpp src/gravity.o src/init.o src/integrator.o src/utils.o
# # test_two_body: $(TEST_DIR) tests/test_two_body.cpp src/gravity.o src/init.o src/integrator.o src/utils.o
# # 	$(CXX) $(CXXFLAGS) -o $(TEST_EXEC) tests/test_two_body.cpp src/gravity.o src/init.o src/integrator.o src/utils.o

run_test_two_body: test_two_body
	./$(TEST_EXEC)

# Build test_freefall without linking main.cpp
# test_freefall: $(TEST_DIR) tests/test_freefall.cpp src/gravity.o src/init.o src/integrator.o src/utils.o src/density.o src/hydro.o
# 	$(CXX) $(CXXFLAGS) -o $(TEST_FREEFALL) tests/test_freefall.cpp src/gravity.o src/init.o src/integrator.o src/utils.o

run_test_freefall: test_freefall
	./$(TEST_FREEFALL)

# test_momentum: $(TEST_DIR) tests/test_momentum.cpp src/gravity.o src/init.o src/integrator.o src/utils.o src/density.o src/hydro.o
# 	$(CXX) $(CXXFLAGS) -o $(TEST_MOMENTUM_EXEC) tests/test_momentum.cpp src/gravity.o src/init.o src/integrator.o src/utils.o

run_test_momentum: test_momentum
	./$(TEST_MOMENTUM_EXEC)

tests: run_test_two_body run_test_freefall run_test_momentum


# Cleanup
clean:
	rm -f $(OBJ) $(TARGET) $(TEST_EXEC) $(TEST_FREEFALL) $(TEST_MOMENTUM_EXEC)


.PHONY: all run clean tests test_two_body test_freefall


# rm -f $(OBJ) $(TARGET) $(TEST_EXEC) $(TEST_FREEFALL)