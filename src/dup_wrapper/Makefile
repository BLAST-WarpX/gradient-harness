include ../defs.mk

.PHONY: all
all: dup_wrapper.o 

# Generate LLVM IR
dup_wrapper.ll: dup_wrapper.c dup_wrapper.h
	$(CC) $< -S -emit-llvm -O2 -fno-vectorize -fno-slp-vectorize -fno-unroll-loops -o $@

# Apply Enzyme plugin to LLVM IR
dup_wrapper_enzyme.ll: dup_wrapper.ll
	$(LLVM_INSTALL_DIR)/bin/opt $< -S --load-pass-plugin=$(ENZYME_DIR)/build/Enzyme/LLVMEnzyme-$(LLVM_VERSION).$(ENZYME_PLUGIN_EXT) -passes=enzyme -o $@ 

# Optimize Enzyme output and compile
dup_wrapper.o: dup_wrapper_enzyme.ll
	$(CC) $< -O2 -o $@

.PHONY: clean
clean:
	rm -f *.o *.ll
