include ../defs.mk

.PHONY: all
all: dot.o 

# Generate LLVM IR
dot.ll: dot.c dot.h
	$(CC) $< -S -emit-llvm -O2 -fno-vectorize -fno-slp-vectorize -fno-unroll-loops -o $@

# Apply Enzyme plugin to LLVM IR
dot_enzyme.ll: dot.ll
	$(LLVM_INSTALL_DIR)/bin/opt $< -S --load-pass-plugin=$(ENZYME_DIR)/build/Enzyme/LLVMEnzyme-$(LLVM_VERSION).$(ENZYME_PLUGIN_EXT) -passes=enzyme -o $@ 

# Optimize Enzyme output and compile
dot.o: dot_enzyme.ll
	$(CC) $< -O2 -o $@

.PHONY: clean
clean:
	rm -f *.o *.ll
