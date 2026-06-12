include ../defs.mk

.PHONY: all
all: square.o 

# Generate LLVM IR
square.ll: square.c square.h
	$(CC) $< -S -emit-llvm -O2 -fno-vectorize -fno-slp-vectorize -fno-unroll-loops -o $@

# Apply Enzyme plugin to LLVM IR
square_enzyme.ll: square.ll
	$(LLVM_DIR)/bin/opt $< -S --load-pass-plugin=$(ENZYME_DIR)/build/Enzyme/LLVMEnzyme-16.dylib -passes=enzyme -o $@ 

# Optimize Enzyme output and compile
square.o: square_enzyme.ll
	$(CC) $< -O2 -o $@

.PHONY: clean
clean:
	rm -f *.o *.ll
