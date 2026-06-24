# gradient-harness
C/C++ gradient harness for R&amp;D and testing of [EnzymeAD](https://github.com/EnzymeAD/).

# Build Instructions

Each example can be built by running `make` from the source subdirectory (e.g. `gradient-harness/src/square`). The file `defs.mk` should updated to use your version of LLVM and Enzyme:

```
# gradient-harness/src/defs.mk

LLVM_DIR = <LLVM installation directory>
ENZYME_DIR = <Enzyme installation directory>
```

Note that Enzyme currently only supports up to LLVM version 16.
