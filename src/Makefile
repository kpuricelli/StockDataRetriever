#==============================================================================
# Makefile to build the src executable
#==============================================================================

# Include the common makefile so we get $(CXX), $(LDLIBS), etc
include ../Makefile.common.defs.mk

# Executable name
PROGNAME := StockDataRetriever
TARGET := $(PROJ_BIN_DIR)/$(PROGNAME)
SRC := $(wildcard $(PROJ_SRC_DIR)/*.cpp)
OBJ := $(SRC:$(PROJ_SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

#===============================================================================
# Targets
#===============================================================================
.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ) | $(PROJ_BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ $(LDLIBS) -o $@

$(OBJ_DIR)/%.o: $(PROJ_SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(PROJ_BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(PROJ_ROOT)/bin $(OBJ_DIR)

-include $(OBJ:.o=.d)
