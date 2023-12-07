#==============================================================================
# Makefile to build the non-test executable
# To build the test executable, run "make test"
#==============================================================================
include ../Makefile.common.defs

# Executable name
PROGNAME := StockDataRetriever
TARGET := $(PROJ_BIN_DIR)/$(PROGNAME)
SRC := $(wildcard $(PROJ_SRC_DIR)/*.cpp)
OBJ := $(SRC:$(PROJ_SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

#===============================================================================
# Targets
#===============================================================================
.PHONY: all clean tests

all: $(TARGET)

$(TARGET): $(OBJ) | $(PROJ_BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(PROJ_SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) -c $< -o $@

$(PROJ_BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(PROJ_BIN_DIR) $(OBJ_DIR)

# Rule to make tests
test: $(TARGET)
	make -C ../tests

-include $(OBJ:.o=.d)
