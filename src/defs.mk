# Global Make variables
UNAME = $(shell uname)
 
LLVM_VERSION ?= 16
ENZYME_DIR = $(HOME)/src/Enzyme
ENZYME_PLUGIN_EXT = so

# Use LLVM_DIR environment variable if set
ifeq ($(UNAME), Linux)
	LINUX_DIST = $(shell lsb_release -is)

	ifeq ($(LINUX_DIST), SUSE)
		# Perlmutter
		LLVM_INSTALL_DIR ?= $(shell spack location -i llvm@$(LLVM_VERSION))
	endif 
else ifeq ($(UNAME), Darwin)
	# macOS
	LLVM_INSTALL_DIR ?= $(shell brew --prefix)/opt/llvm@$(LLVM_VERSION)
	ENZYME_PLUGIN_EXT = dylib
endif

LLVM_INSTALL_DIR ?= /usr/lib/llvm-$(LLVM_VERSION)

CC = $(LLVM_INSTALL_DIR)/bin/clang
CXX = $(LLVM_INSTALL_DIR)/bin/clang++

# Builtin commands
print-%:
	@echo '$*=$($*)'
