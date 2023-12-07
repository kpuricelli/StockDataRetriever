#==============================================================================
# Makefile for linking the Catch2 tests against all necessary object files -
# note 'main.o' is specifically excluded 
#==============================================================================
include ../Makefile.common.defs

# Executable name
PROGNAME := RunTests
TEST_ROOT := .
TST_TARGET := $(PROJ_BIN_DIR)/$(PROGNAME)
SRC := $(wildcard $(TEST_ROOT)/*.cpp)
OBJ := $(SRC:$(TEST_ROOT)/%.cpp=$(OBJ_DIR)/%.o)


# kptodo rm
$(info PROJ_ROOT is: $(PROJ_ROOT))
$(info TEST_SRC_DIR is: $(TEST_SRC_DIR))
$(info TEST_ROOT is: $(TEST_ROOT))
$(info PROGNAME is: $(PROGNAME))
$(info TST_TARGET is: $(TST_TARGET))
$(info SRC is: $(SRC))
$(info OBJ is: $(OBJ))

# Test-specific source files
# We need all the object files, except main.o, since we're linking with
# Catch2's main.o
#SRC_MAIN := $(wildcard $(PROJ_SRC_DIR)/*.cpp)
#SRC_MAIN := $(filter-out $(PROJ_SRC_DIR)/main.cpp, $(SRC_MAIN))

SRC_MAIN := $(wildcard ../src/*.cpp)
SRC_MAIN := $(filter-out ../src/main.cpp, $(SRC_MAIN))

OBJ2     := $(SRC_MAIN:../src/%.cpp=$(OBJ_DIR)/%.o)

OBJS := $(OBJ) $(OBJ2)

# KPTODO RM
$(info OBJ_DIR is: $(OBJ_DIR))
$(info PROJ_BIN_DIR is: $(PROJ_BIN_DIR))
$(info SRC_MAIN is: $(SRC_MAIN))
$(info OBJ2 is: $(OBJS))

#===============================================================================
# Targets
#===============================================================================
.PHONY: all clean

all: $(TST_TARGET)

$(TST_TARGET): $(OBJS) | $(PROJ_BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(TEST_ROOT)/%.cpp | $(OBJ_DIR)
	$(CXX) -c $< -o $@

$(PROJ_BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(PROJ_BIN_DIR) $(OBJ_DIR)

-include $(OBJ:.o=.d)
