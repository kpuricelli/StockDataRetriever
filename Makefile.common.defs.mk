#==============================================================================
# This top level makefile is designed to extract common defs / paths
# needed by other makefiles
#==============================================================================

#==============================================================================
# Start by including the common definitions
#==============================================================================
OS := $(shell uname -s).$(shell uname -m)
PROJ_ROOT := $(shell bash -cf "pwd | sed -e 's:\(.*\)/src\(.*\):\1:'")
TEST_ROOT := $(shell bash -cf "pwd | sed -e 's:\(.*\)/tests\(.*\):\1:'")

#==============================================================================
# Directories
#==============================================================================
PROJ_SRC_DIR := $(PROJ_ROOT)/src
TEST_SRC_DIR := $(TEST_ROOT)/tests
OBJ_DIR      := $(OS)
PROJ_BIN_DIR := $(PROJ_ROOT)/bin/$(OBJ_DIR)
TEST_BIN_DIR := $(TEST_ROOT)/bin/$(OBJ_DIR)

#==============================================================================
# Compiler flags
#==============================================================================
CXX := g++ -std=c++20
OptFlags := -O2
DebugFlags := -g

# Add the opt / debug flags if desired
ifeq ($(STX_OPTIMIZE),none)
  CXX_DEBUG := $(DebugFlags)
  CXX_OPTIMIZE :=
endif
ifeq ($(STX_OPTIMIZE),both)
  CXX_DEBUG := $(DebugFlags)
  CXX_OPTIMIZE := $(OptFlags)
endif
ifeq ($(STX_OPTIMIZE),all)
  CXX_DEBUG :=
  CXX_OPTIMIZE := $(OptFlags)
endif

# More flags
CXX_WARN := -Wall -Werror -Wextra -Wconversion -pedantic
CXXFLAGS := $(CXX_WARN) $(CXX_DEBUG) $(CXX_OPTIMIZE)

# Libs
ifeq (, $(findstring Linux, $($(shell uname -s))))
  LDLIBS := -lcurl -I/usr/include/boost 
else
  LDLIBS := -lcurl -I/usr/local/Cellar/boost/1.85.0/include/boost
endif

#===============================================================================
# Targets: none
#===============================================================================
