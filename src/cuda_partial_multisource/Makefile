EXEC = saxpy
OBJS = main saxpy

include ../defs.mk
include ../rules.mk

# For now we override the default build rules
CUDA_CXXFLAGS = --offload-arch=$(CUDA_OFFLOAD_ARCH) -fgpu-rdc -fplugin=$(ENZYME_CLANG_PLUGIN)
CUDA_LDFLAGS = --offload-arch=$(CUDA_OFFLOAD_ARCH) -fgpu-rdc --offload-new-driver -L${CUDA_HOME}/lib64 -lcudart -fplugin=$(ENZYME_CLANG_PLUGIN)

# CUDA linking rule
$(EXEC): $(OBJ_FILES)
	$(CXX) $(CUDA_LDFLAGS) $^ -o $(EXEC)

# CUDA multisource compile rules
%.o: %.cu %.h
	$(CXX) $(CUDA_CXXFLAGS) -c $< -o $@

%.o: %.cu
	$(CXX) $(CUDA_CXXFLAGS) -c $< -o $@

