# Inspired from https://www.partow.net/programming/makefile/index.html
CXX      := -c++
CXXFLAGS := -pedantic-errors -Wall -Wextra -O3 -std=c++20
LDFLAGS  := -L/usr/lib -lstdc++ -lm
BUILD    := ./build
OBJ_DIR  := $(BUILD)
TARGET   := othello-bot
INCLUDE  := -Iinclude/
SRC      := $(wildcard src/*.cpp)

OBJECTS  := $(SRC:%.cpp=$(OBJ_DIR)/%.o)
DEPENDENCIES := $(OBJECTS:.o=.d)

all: build $(TARGET)

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -MMD -o $@

$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $^ $(LDFLAGS)

-include $(DEPENDENCIES)

.PHONY: all build clean run remise

build:
	@mkdir -p $(OBJ_DIR)

run: all
	-./$(TARGET)

clean:
	-@rm -rvf $(OBJ_DIR)/*
	-@rm -rvf $(TARGET)

remise:
	zip -r othello-bot.zip src/ include/ Makefile distance.txt