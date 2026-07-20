### Global variables

# Update this if you are using a different LLVM version, or use an environment variable
# E.g. export LLVM_VERSION_MAJOR = <your version>
LLVM_VERSION_MAJOR ?= 22

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
     
    # On Perlmutter and other HPC platforms, LLVM can be loaded with lmod or installed with spack

	# For lmod: load the llvm module for the version you're using
    # E.g. On Perlmutter: `module load llvm/nightly`

    # For spack: uncomment line below, or load llvm in a spack environment 
    #LLVM_INSTALL_DIR ?= $(shell spack location -i llvm@$(LLVM_VERSION_MAJOR))
  endif
else ifeq ($(UNAME), Darwin)
  # macOS
  LLVM_INSTALL_DIR ?= $(shell brew --prefix)/opt/llvm@$(LLVM_VERSION_MAJOR)

  # On mac, brew installs lld with llvm@16
  # TODO: other versions of llvm might install lld too
  ifeq ($(LLVM_VERSION_MAJOR),16)
    LLD_INSTALL_DIR ?= $(LLVM_INSTALL_DIR)
  else
    LLD_INSTALL_DIR ?= $(shell brew --prefix)/opt/lld@$(LLVM_VERSION_MAJOR)
  endif

  LD_NAME = ld64.lld
  SO_EXT = dylib
  
  PYBIND_LDFLAGS += -shared -undefined dynamic_lookup
endif
 
# LLVM_VERSION_MAJOR and LLVM_INSTALL_DIR should be set as environment variables,
# or we assume they are in PATH
ifndef LLVM_INSTALL_DIR
  # User didn't provide LLVM_INSTALL_DIR, use compilers/linkers in PATH
  CC = clang
  CXX = clang++
  LD = lld
else
  CC = $(LLVM_INSTALL_DIR)/bin/clang
  CXX = $(LLVM_INSTALL_DIR)/bin/clang++
  
  ifndef LLD_INSTALL_DIR
    LD = lld 
  else
	LD = $(LLD_INSTALL_DIR)/bin/$(LD_NAME)
  endif
endif

ENZYME_CLANG_PLUGIN = $(ENZYME_DIR)/build/Enzyme/ClangEnzyme-$(LLVM_VERSION_MAJOR).$(SO_EXT)
ENZYME_LLD_PLUGIN = $(ENZYME_DIR)/build/Enzyme/LLDEnzyme-$(LLVM_VERSION_MAJOR).$(SO_EXT)

# Default Enzyme flags. We run Enzyme only during linking. 
# Adding the ClangEnzyme plugin to the compile step wih enzyme-enable=0 seems to be necessary
# for the compiler to pick up on attributes like __attribute__((enzyme_inactive))
ENZYME_CXXFLAGS = -flto -fplugin=$(ENZYME_CLANG_PLUGIN) -mllvm -enzyme-enable=0
ENZYME_LDFLAGS = -fuse-ld=$(LD) -flto -Wl,-mllvm,-load=$(ENZYME_LLD_PLUGIN) -Wl,--load-pass-plugin=$(ENZYME_LLD_PLUGIN) 

# Enzyme debugging options
#ENZYME_LDFLAGS += -Wl,--lto-O1
#ENZYME_LDFLAGS += -Wl,-mllvm,-enzyme-print=1
#ENZYME_LDFLAGS += -Wl,-mllvm,-enzyme-print-activity=1
#ENZYME_LDFLAGS += -Wl,-mllvm,-enzyme-print-type=1
#ENZYME_LDFLAGS += -Wl,-mllvm,-enzyme-globals-default-inactive=1

# For now keep C and CXX the same 
ENZMYE_CFLAGS = $(ENZYME_CXXFLAGS)

# Rule: print contents of Makefile variable
print-%:
	@echo '$*=$($*)'

