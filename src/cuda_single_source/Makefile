EXEC = saxpy

include ../defs.mk
include ../rules.mk

# Override default build rule for now
CUDA_CXXFLAGS = --offload-arch=$(CUDA_OFFLOAD_ARCH) -L${CUDA_HOME}/lib64 -lcudart -fplugin=$(ENZYME_CLANG_PLUGIN)

# CUDA compile/link rule
$(EXEC): saxpy.cu
	$(CXX) $(CUDA_CXXFLAGS) saxpy.cu -o $(EXEC)

