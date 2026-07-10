### Default build rules
# Should define these variables in child Makefile
#   EXEC = executable name
#   OBJS = list of object files
#   PYBIND_MODULE = name of pybind module
#   PYBIND_OBJS = list of files with pybind11 bindings

.PHONY: clean pybind

OBJ_FILES = $(addsuffix .o, $(OBJS))
 
# LLVM_VERSION and LLVM_DIR should be set as environment variables,
# or we assume 
ifndef LLVM_DIR
  # User didn't provide LLVM_DIR, use compilers/linkers in PATH
  CC = clang
  CXX = clang++
  LD = lld
else
  CC = $(LLVM_DIR)/bin/clang
  CXX = $(LLVM_DIR)/bin/clang++
  LD = $(LLVM_DIR)/bin/$(LD_NAME)
endif

ifdef EXEC
$(EXEC): $(OBJ_FILES)
	$(CXX) $(ENZYME_LDFLAGS) $^ -o $(EXEC)
endif

%.o: %.cpp %.h
	$(CXX) $(ENZYME_CXXFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(ENZYME_CXXFLAGS) -c $< -o $@

# pybind11 rules
ifdef PYBIND_OBJS
PYBIND_OBJ_FILES = $(addsuffix _pybind.o,$(PYBIND_OBJS))

pybind: $(OBJ_FILES) $(PYBIND_OBJ_FILES)
	$(CXX) $(PYBIND_LDFLAGS) $^ -o  $(PYBIND_MODULE)$(CPYTHON_EXT)

%_pybind.o: %_pybind.cpp
	$(CXX) $(PYBIND_CFLAGS) -c $< -o $@
endif

# TODO: Should move all examples to C++ so we can use pybind11
%.o: %.c %.h
	$(CC) $(ENZYME_CFLAGS) -c $< -o $@

%.o: %.c
	$(CC) $(ENZYME_CFLAGS) -c $< -o $@

clean:
	rm -f *.o *.ll	
	rm -f $(EXEC)
	rm -f *$(CPYTHON_EXT)
	rm -rf __pycache__

print-%:
	@echo '$*=$($*)'

