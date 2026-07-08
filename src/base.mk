### Global variables

ENZYME_DIR = $(HOME)/src/Enzyme
SO_EXT = so

CPYTHON_EXT = $(shell python3-config --extension-suffix)
CPTHON_EXT ?= .cpython.so 
PYTHON_C_COMP_FLAGS = -I../../extern/pybind11/include $(shell python3-config --include)
PYTHON_C_LINK_FLAGS = -shared

## System specific logic
UNAME = $(shell uname)

ifeq ($(UNAME), Linux)
  # Linux
  PYTHON_C_COMP_FLAGS += -fPIC
 
  # Specific Linux distributions
  LINUX_DIST = $(shell lsb_release -is)
  ifeq ($(LINUX_DIST), SUSE)
    # SUSE (e.g. Perlmutter)
    LLVM_INSTALL_DIR ?= $(shell spack location -i llvm@$(LLVM_VERSION))
  endif 
else ifeq ($(UNAME), Darwin)
  # macOS
  LLVM_INSTALL_DIR ?= $(shell brew --prefix)/opt/llvm@$(LLVM_VERSION)
  SO_EXT = dylib
  
  PYTHON_C_LINK_FLAGS += -shared -undefined dynamic_lookup
endif

## System agnostic logic
 
# LLVM_VERSION and LLVM_INSTALL_DIR should be set as environment variables,
# or they will default to the values here
LLVM_VERSION ?= 16
LLVM_INSTALL_DIR ?= /usr/lib/llvm-$(LLVM_VERSION)

CC = $(LLVM_INSTALL_DIR)/bin/clang
CXX = $(LLVM_INSTALL_DIR)/bin/clang++

ENZYME_CLANG_PLUGIN = $(ENZYME_DIR)/build/Enzyme/ClangEnzyme-$(LLVM_VERSION).$(SO_EXT)
ENZYME_LLD_PLUGIN = $(ENZYME_DIR)/build/Enzyme/LLDEnzyme-$(LLVM_VERSION).$(SO_EXT)


### Builtin rules
# Should define these variables in child Makefile
#   EXEC = executable name
#   OBJS = list of object files
#   PYBIND_MODULE = name of pybind module
#   PYBIND_OBJS = list of files with pybind11 bindings

OBJ_FILES = $(addsuffix .o, $(OBJS))

.PHONY: clean pybind

ifdef EXEC
$(EXEC): $(OBJ_FILES)
	$(CXX) -fplugin=$(ENZYME_CLANG_PLUGIN) $^ -o $(EXEC)
endif

%.o: %.cpp %.h
	$(CXX) -flto -fplugin=$(ENZYME_CLANG_PLUGIN) -c $< -o $@

# pybind11 rules
ifdef PYBIND_OBJS
PYBIND_OBJ_FILES = $(addsuffix _pybind.o,$(PYBIND_OBJS))

pybind: $(OBJ_FILES) $(PYBIND_OBJ_FILES)
	$(CXX) -fplugin=$(ENZYME_CLANG_PLUGIN) $(PYTHON_C_LINK_FLAGS) $^ -o  $(PYBIND_MODULE)$(CPYTHON_EXT)

%_pybind.o: %_pybind.cpp
	$(CXX) $(PYTHON_C_COMP_FLAGS) -c $< -o $@
endif

# TODO: Should move all examples to C++ so we can use pybind11
%.o: %.c %.h
	$(CC) -flto -fplugin=$(ENZYME_CLANG_PLUGIN) -c $< -o $@

clean:
	rm -f *.o *.ll	
	rm -f $(EXEC)
	rm -rf *$(CPYTHON_EXT)
	rm -rf __pycache__

print-%:
	@echo '$*=$($*)'

