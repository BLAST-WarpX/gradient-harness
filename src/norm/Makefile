include ../defs.mk

.PHONY: all
all: norm.o 

# Generate LLVM IR
norm.ll: norm.c norm.h
	$(CC) $< -S -emit-llvm -O2 -fno-vectorize -fno-slp-vectorize -fno-unroll-loops -o $@

# Apply Enzyme plugin to LLVM IR
norm_enzyme.ll: norm.ll
	$(LLVM_DIR)/bin/opt $< -S --load-pass-plugin=$(ENZYME_DIR)/build/Enzyme/LLVMEnzyme-16.dylib -passes=enzyme -o $@ 

# Optimize Enzyme output and compile
norm.o: norm_enzyme.ll
	$(CC) $< -O2 -o $@

.PHONY: clean
clean:
	rm -f *.o *.ll
