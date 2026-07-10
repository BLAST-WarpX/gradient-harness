### Global variables

# Update this if you are using a different LLVM version, or use an environment variable
# E.g. export LLVM_VERSION = <your version>
LLVM_VERSION ?= 23

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

ENZYME_CLANG_PLUGIN = $(ENZYME_DIR)/build/Enzyme/ClangEnzyme-$(LLVM_VERSION).$(SO_EXT)
ENZYME_LLD_PLUGIN = $(ENZYME_DIR)/build/Enzyme/LLDEnzyme-$(LLVM_VERSION).$(SO_EXT)

ENZMYE_CFLAGS = -flto
ENZYME_CXXFLAGS = -flto
ENZYME_LDFLAGS = -fuse-ld=$(LD) -flto -Wl,-mllvm -Wl,-load=$(ENZYME_LLD_PLUGIN) -Wl,--load-pass-plugin=$(ENZYME_LLD_PLUGIN) 

