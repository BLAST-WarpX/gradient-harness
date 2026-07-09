### Global variables
LD_NAME = ld.lld

ENZYME_DIR = $(HOME)/src/Enzyme
SO_EXT = so

# On Perlmutter and other HPC platforms, the tests assume python is loaded
# E.g. On Perlmutter: `module load python`
CPYTHON_EXT = $(shell python3-config --extension-suffix)
CPTHON_EXT ?= .cpython.so 
PYBIND_CFLAGS = -I../../extern/pybind11/include $(shell python3-config --includes)
PYBIND_LDFLAGS = -shared

## System specific logic
UNAME = $(shell uname)

ifeq ($(UNAME), Linux)
  # Linux
  PYBIND_CFLAGS += -fPIC
 
  # Specific Linux distributions
  LINUX_DIST = $(shell lsb_release -is)
  ifeq ($(LINUX_DIST), SUSE)
    # SUSE (e.g. Perlmutter)
     
    # On Perlmutter and other HPC platforms, we assume LLVM has been loaded with lmod
    # E.g. On Perlmutter: `module load llvm/nightly`

    # Uncomment if using spack
    #LLVM_DIR ?= $(shell spack location -i llvm@$(LLVM_VERSION))
  endif
else ifeq ($(UNAME), Darwin)
  # macOS
  LLVM_DIR ?= $(shell brew --prefix)/opt/llvm@$(LLVM_VERSION)
  LD_NAME = ld64.lld
  SO_EXT = dylib
  
  PYBIND_LDFLAGS += -shared -undefined dynamic_lookup
endif

## System agnostic logic
 
# LLVM_VERSION and LLVM_DIR should be set as environment variables,
# or we assume 
ifndef LLVM_DIR
  # User didn't provide LLVM_DIR, use compilers/linkers in PATH
  CC = clang
  CXX = clang++
  LD = lld
else
  CC = $(LLVM_DIR)/bin/clang
  CXX = $(LLVM_DIR)/bin/clang++
  LD = $(LLVM_DIR)/bin/$(LD_NAME)
endif

ifndef LLVM_VERSION
  $(error LLVM_VERSION environment variable is required.)
endif

ENZYME_CLANG_PLUGIN = $(ENZYME_DIR)/build/Enzyme/ClangEnzyme-$(LLVM_VERSION).$(SO_EXT)
ENZYME_LLD_PLUGIN = $(ENZYME_DIR)/build/Enzyme/LLDEnzyme-$(LLVM_VERSION).$(SO_EXT)

ENZYME_CFLAGS = -flto
ENZYME_LDFLAGS = -fuse-ld=$(LD) -flto -Wl,--lto-O0 -Wl,-mllvm -Wl,-load=$(ENZYME_LLD_PLUGIN) -Wl,--load-pass-plugin=$(ENZYME_LLD_PLUGIN) 

### Builtin rules
# Should define these variables in child Makefile
#   EXEC = executable name
#   OBJS = list of object files
#   PYBIND_MODULE = name of pybind module
#   PYBIND_OBJS = list of files with pybind11 bindings

.PHONY: clean pybind

OBJ_FILES = $(addsuffix .o, $(OBJS))

ifdef EXEC
$(EXEC): $(OBJ_FILES)
	$(CXX) $(ENZYME_LDFLAGS) $^ -o $(EXEC)
endif

%.o: %.cpp %.h
	$(CXX) $(ENZYME_CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(ENZYME_CFLAGS) -c $< -o $@

# pybind11 rules
ifdef PYBIND_OBJS
PYBIND_OBJ_FILES = $(addsuffix _pybind.o,$(PYBIND_OBJS))

pybind: $(OBJ_FILES) $(PYBIND_OBJ_FILES)
	$(CXX) $(PYBIND_LDFLAGS) $^ -o  $(PYBIND_MODULE)$(CPYTHON_EXT)

%_pybind.o: %_pybind.cpp
	$(CXX) $(PYBIND_CFLAGS) -c $< -o $@
endif

# TODO: Should move all examples to C++ so we can use pybind11
%.o: %.c %.h
	$(CC) $(ENZYME_CFLAGS) -c $< -o $@

clean:
	rm -f *.o *.ll	
	rm -f $(EXEC)
	rm -f *$(CPYTHON_EXT)
	rm -rf __pycache__

print-%:
	@echo '$*=$($*)'

