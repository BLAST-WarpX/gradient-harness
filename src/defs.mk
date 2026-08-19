######## Global variables #########
# The following variables should be defined in the subproject Makefile
#   EXEC = executable name
#   OBJS = list of object files
#   PYBIND_MODULE = name of pybind module
#   PYBIND_OBJS = list of files with pybind11 bindings

OBJ_FILES = $(addsuffix .o, $(OBJS))
PYBIND_OBJ_FILES = $(addsuffix _pybind.o,$(PYBIND_OBJS))

# Update or export LLVM_VERSION_MAJOR if you are using a different LLVM version.
# If you have LLVM installed in a specific location, export the environment variable
# LLVM_DIR.
LLVM_VERSION_MAJOR ?= 21

LD_NAME = ld.lld

ENZYME_DIR = $(HOME)/src/Enzyme
SO_EXT = so

# CUDA settings
CUDA_OFFLOAD_ARCH = sm_80

CPYTHON_EXT = $(shell python3-config --extension-suffix)
CPTHON_EXT ?= .cpython.so 
PYTHON_INCLUDES = $(shell python3-config --includes)
PYBIND_MODULE_FULL = $(PYBIND_MODULE)$(CPYTHON_EXT)

######## pybind11 flags ########
PYBIND_CXXFLAGS = -I../../extern/pybind11/include $(PYTHON_INCLUDES) 
PYBIND_LDFLAGS = -shared

## System specific logic
UNAME = $(shell uname)

ifeq ($(UNAME), Linux)
  # Linux
  PYBIND_CXXFLAGS += -fPIC
 
  # Specific Linux distributions
  LINUX_DIST = $(shell lsb_release -is)
  ifeq ($(LINUX_DIST), SUSE)
    # SUSE (e.g. Perlmutter)
     
    # On Perlmutter and other HPC platforms, LLVM can be loaded with lmod or installed with spack

	# For lmod: load the llvm module for the version you're using
    # E.g. On Perlmutter: `module load llvm/nightly`

    # For spack: uncomment line below, or load llvm in a spack environment 
    #LLVM_DIR ?= $(shell spack location -i llvm@$(LLVM_VERSION_MAJOR))
  endif
else ifeq ($(UNAME), Darwin)
  # macOS
  LLVM_DIR ?= $(shell brew --prefix)/opt/llvm@$(LLVM_VERSION_MAJOR)

  # On mac, brew installs lld with llvm@16
  # TODO: other versions of llvm might install lld too
  ifeq ($(LLVM_VERSION_MAJOR),16)
    LLD_DIR ?= $(LLVM_DIR)
  else
    LLD_DIR ?= $(shell brew --prefix)/opt/lld@$(LLVM_VERSION_MAJOR)
  endif

  LD_NAME = ld64.lld
  SO_EXT = dylib
  
  PYBIND_LDFLAGS += -shared -undefined dynamic_lookup
endif
 
ifndef LLVM_DIR
  # User didn't provide LLVM_DIR, use compilers/linkers in PATH
  CC = clang
  CXX = clang++
  LD = lld
else
  CC = $(LLVM_DIR)/bin/clang
  CXX = $(LLVM_DIR)/bin/clang++
  
  ifndef LLD_DIR
    LD = lld 
  else
	LD = $(LLD_DIR)/bin/$(LD_NAME)
  endif
endif

ENZYME_CLANG_PLUGIN = $(ENZYME_DIR)/build/Enzyme/ClangEnzyme-$(LLVM_VERSION_MAJOR).$(SO_EXT)
ENZYME_LLD_PLUGIN = $(ENZYME_DIR)/build/Enzyme/LLDEnzyme-$(LLVM_VERSION_MAJOR).$(SO_EXT)

######## Enzyme flags ########
# In order for Enzyme to access functions defined across multiple translation units,
# we compile with link time optimization (LTO) and then run Enzyme only during
# the linking step.
# Adding the ClangEnzyme plugin to the compile step wih enzyme-enable=0 is necessary
# for the compiler to pick up on frontend clang attributes like __attribute__((enzyme_inactive)).
ENZYME_CXXFLAGS = -O3 -flto -fplugin=$(ENZYME_CLANG_PLUGIN) -mllvm -enzyme-enable=0
ENZYME_LDFLAGS = -fuse-ld=$(LD) -flto -Wl,-mllvm,-load=$(ENZYME_LLD_PLUGIN) -Wl,--load-pass-plugin=$(ENZYME_LLD_PLUGIN)

#ENZYME_LDFLAGS += -Wl,-mllvm,-enzyme-fast-math=0
#ENZYME_LDFLAGS += -Wl,-mllvm,-enzyme-max-type-depth=2

# Enzyme debugging options
#ENZYME_LDFLAGS += -Wl,--lto-O1
#ENZYME_LDFLAGS += -Wl,-mllvm,-enzyme-print=1
#ENZYME_LDFLAGS += -Wl,-mllvm,-enzyme-print-activity=1
#ENZYME_LDFLAGS += -Wl,-mllvm,-enzyme-print-type=1
#ENZYME_LDFLAGS += -Wl,-mllvm,-enzyme-globals-default-inactive=1

# For now keep C and CXX flags the same
ENZYME_CFLAGS = $(ENZYME_CXXFLAGS)

# Rule: print contents of Makefile variable
print-%:
	@echo '$*=$($*)'

