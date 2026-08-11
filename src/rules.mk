### Default build rules
.PHONY: clean pybind

# Executable linking rule
ifdef EXEC
$(EXEC): $(OBJ_FILES)
	$(CXX) $(ENZYME_LDFLAGS) $^ -o $(EXEC)
endif

# pybind11 rules
ifdef PYBIND_OBJS
pybind: $(PYBIND_MODULE_FULL)

# Note: Because we run Enzyme at link time, we need to run Enzyme when linking pybind as well
$(PYBIND_MODULE_FULL): $(OBJ_FILES) $(PYBIND_OBJ_FILES)
	$(CXX) $(ENZYME_LDFLAGS) $(PYBIND_LDFLAGS) $^ -o $@ 

%_pybind.o: %_pybind.cpp
	$(CXX) $(PYBIND_CXXFLAGS) -c $< -o $@
endif

%.o: %.cpp %.h
	$(CXX) $(ENZYME_CXXFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(ENZYME_CXXFLAGS) -c $< -o $@

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

