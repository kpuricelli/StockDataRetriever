#
# Test Makefile v1.?
#

# Directories
SRC_DIR := .
OBJ_DIR := obj
BIN_DIR := ../bin

# Path to src and its *.o files
SRC_DIR_MAIN := ../src
OBJ_DIR_MAIN := ../obj

# Executable name
PROGNAME := RunTests
TARGET := $(BIN_DIR)/$(PROGNAME)

# Test files
SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Main source files
# We need all the object files, except main.o, since we're linking with
# catch's main
SRC_MAIN := $(wildcard $(SRC_DIR_MAIN)/*.cpp)
SRC_MAIN := $(filter-out $(SRC_DIR_MAIN)/main.cpp, $(SRC_MAIN))
OBJ2 := $(SRC_MAIN:$(SRC_DIR_MAIN)/%.cpp=$(OBJ_DIR_MAIN)/%.o)

# Compiler args
CXX := g++
CXXFLAGS := -std=c++20 -Wall -Werror -Wextra -Wconversion -pedantic

# Libs
LDLIBS := -lcurl

#===============================================================================
# Targets
#===============================================================================
.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ $(OBJ2) $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(OBJ_DIR)

-include $(OBJ:.o=.d)
