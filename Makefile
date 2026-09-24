CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
BUILD    = build

SRC = $(wildcard src/*/*.cpp)
OBJ = $(SRC:src/%.cpp=$(BUILD)/%.o)

.PHONY: all run clean test

all: $(BUILD)/app

$(BUILD)/app: $(OBJ) src/main.cpp
	@mkdir -p $(BUILD)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(BUILD)/app
	./$(BUILD)/app

# Chạy 1 test: make test_Date
test_%: tests/test_%.cpp $(OBJ)
	@mkdir -p $(BUILD)
	$(CXX) $(CXXFLAGS) $^ -o $(BUILD)/$@
	./$(BUILD)/$@

clean:
	rm -rf $(BUILD)
