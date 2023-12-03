#
# Makefile v1.3
#

# Directories
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

# Executable name
PROGNAME := StockDataRetriever
TARGET := $(BIN_DIR)/$(PROGNAME)
SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

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
	$(CXX) $(CXXFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) -c $< -o $@

$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(BIN_DIR) $(OBJ_DIR)

-include $(OBJ:.o=.d)
