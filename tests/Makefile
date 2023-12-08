#==============================================================================
# Makefile for linking the Catch2 tests against all necessary object files -
# note 'main.o' is specifically excluded 
#==============================================================================

#
# kptodo
# only compile tests if the src object files exist
#

include ../Makefile.common.defs

# Executable name
PROGNAME := RunTests
TST_TARGET := $(TEST_BIN_DIR)/$(PROGNAME)
SRC := $(wildcard $(TEST_SRC_DIR)/*.cpp)
OBJ := $(SRC:$(TEST_SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# kptodo rm
$(info src_DIR: $(TEST_SRC_DIR))
$(info src: $(SRC))
$(info objs: $(OBJS))

# Test-specific source files
# We need all the object files, except main.o, since we're linking with
# Catch2's main.o
SRC_DIR  := $(TEST_ROOT)/src
SRC_MAIN := $(wildcard $(SRC_DIR)/*.cpp)
SRC_MAIN := $(filter-out $(SRC_DIR)/main.cpp, $(SRC_MAIN))
SRC_OBJ_DIR := $(SRC_DIR)/$(OBJ_DIR)
SRC_OBJS := $(SRC_MAIN:$(SRC_DIR)/%.cpp=$(SRC_OBJ_DIR)/%.o)
ALL_OBJS := $(OBJ) $(SRC_OBJS)

#===============================================================================
# Targets
#===============================================================================
.PHONY: all clean

all: $(TST_TARGET)

$(TST_TARGET): $(ALL_OBJS) | $(TEST_BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(TEST_SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) -c $< -o $@

$(TEST_BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(TEST_ROOT)/bin $(OBJ_DIR)

-include $(OBJ:.o=.d)
