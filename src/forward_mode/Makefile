include ../defs.mk

.PHONY: all
all: forward_mode.o 

# Generate LLVM IR
forward_mode.ll: forward_mode.c forward_mode.h
	$(CC) $< -S -emit-llvm -O2 -fno-vectorize -fno-slp-vectorize -fno-unroll-loops -o $@

# Apply Enzyme plugin to LLVM IR
forward_mode_enzyme.ll: forward_mode.ll
	$(LLVM_DIR)/bin/opt $< -S --load-pass-plugin=$(ENZYME_DIR)/build/Enzyme/LLVMEnzyme-16.dylib -passes=enzyme -o $@ 

# Optimize Enzyme output and compile
forward_mode.o: forward_mode_enzyme.ll
	$(CC) $< -O2 -o $@

.PHONY: clean
clean:
	rm -f *.o *.ll
