# Inspired from https://www.partow.net/programming/makefile/index.html
CXX := -c++
CXXFLAGS := -pedantic-errors -Wall -Wextra -O3 -std=c++20
LDFLAGS := -L/usr/lib -lstdc++ -lm
BUILD := ./build
OBJ_DIR := $(BUILD)
TARGET := othello-bot
TEST_TARGET := test-runner
INCLUDE := -Iinclude/
SRC := $(wildcard src/*.cpp)
TEST_SRC := src/Unittests.cpp
OBJECTS := $(SRC:%.cpp=$(OBJ_DIR)/%.o)
# For main target: exclude Unittests.o (contains mainTests, not main)
MAIN_OBJECTS := $(filter-out $(OBJ_DIR)/src/Unittests.o, $(OBJECTS))
# For test target: exclude main.o if it exists, include Unittests.o
TEST_OBJECTS := $(filter-out $(OBJ_DIR)/src/main.o, $(OBJECTS))
DEPENDENCIES := $(OBJECTS:.o=.d)
TEST_DEPENDENCIES := $(TEST_OBJECTS:.o=.d)

all: build $(TARGET)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -MMD -o $@

$(TARGET): $(MAIN_OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $^ $(LDFLAGS)

$(TEST_TARGET): $(TEST_OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $^ $(LDFLAGS)

-include $(DEPENDENCIES)
-include $(TEST_DEPENDENCIES)

.PHONY: all build clean run test remise

build:
	@mkdir -p $(OBJ_DIR)

run: all
	-./$(TARGET)

test: build $(TEST_TARGET)
	-./$(TEST_TARGET)

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(TARGET)
	-@rm -rvf $(TEST_TARGET)

remise:
	zip -r othello-bot.zip src/ include/ Makefile distance.txt