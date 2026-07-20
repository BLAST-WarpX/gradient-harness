# gradient-harness
C/C++ code for R&amp;D and testing of [EnzymeAD](https://github.com/EnzymeAD/).

# Cloning
```
git clone git@github.com:BLAST-WarpX/gradient-harness
git submodule update --init extern/
```

# Dependencies

[Enzyme](https://github.com/EnzymeAD/Enzyme/)
[AMReX](https://github.com/amrex-codes/amrex)

# Build Instructions

Each example can be built by running `make` from the source subdirectory (e.g. `gradient-harness/src/square`). The build will attempt to locate LLVM based on the operating system, or you can manually set the location with the environment variables below: 

```
export LLVM_VERSION=<LLVM version>
export LLVM_INSTALL_DIR=<LLVM install location>
```

On Perlmutter, load the llvm module before building: 
```
module load llvm/nightly
```

# Tests
On Perlmutter, load the python module before running the unit tests:

```
module load python
```

To run the unit tests:

```
make pybind
python -m unittest -v <test file>.py
```
