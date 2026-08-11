EXEC = saxpy
OBJS = main saxpy

include ../defs.mk
include ../rules.mk

# For now we override the default build rules
# When/if the process of building multisource Enzyme/CUDA projects is formalized, we
# should add these flags and rules to rules.mk

CUDA_CXXFLAGS = --offload-arch=$(CUDA_OFFLOAD_ARCH) -fgpu-rdc -foffload-lto -O3 -flto -fplugin=$(ENZYME_CLANG_PLUGIN) -mllvm -enzyme-enable=0
CUDA_LDFLAGS = --offload-arch=$(CUDA_OFFLOAD_ARCH) -fgpu-rdc --offload-new-driver -L${CUDA_HOME}/lib64 -lcudart -foffload-lto -fuse-ld=lld -flto -Wl,--pass-plugin=$(ENZYME_LLD_PLUGIN) -Wl,--device-linker=-v -Wl,--wrapper-verbose -v

# CUDA linking rule
$(EXEC): $(OBJ_FILES)
	$(CXX) $(CUDA_LDFLAGS) $^ -o $(EXEC)

# CUDA multisource compile rules
%.o: %.cu %.h
	$(CXX) $(CUDA_CXXFLAGS) -c $< -o $@

%.o: %.cu
	$(CXX) $(CUDA_CXXFLAGS) -c $< -o $@

