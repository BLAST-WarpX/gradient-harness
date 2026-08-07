# gradient-harness
C/C++ code for R&amp;D and testing of [EnzymeAD](https://github.com/EnzymeAD/).

# Cloning
```
cd ~/src
git clone git@github.com:BLAST-WarpX/gradient-harness.git

cd gradient-harness
git submodule update --init extern/
```

# Dependencies

[Enzyme](https://github.com/EnzymeAD/Enzyme/)

[AMReX](https://github.com/amrex-codes/amrex)

To install dependencies:

```
cd ~/src
git clone git@github.com:EnzymeAD/Enzyme.git
git clone git@github.com:amrex-codes/amrex.git
```

## Build Enzyme
Instructions to build Enzyme from the source are located on the [Enzyme website](https://enzyme.mit.edu/Installation/). We state the current build steps below, but we should check their website for updates.

If lit (LLVM Integration Tester) is not installed in your path, you need to install it. Normally lit is installed with pipx. The install location should be `~/.local/bin/lit`, but check your install to confirm.
```
python3 -m pip install pipx
pipx install lit
```

To build Enzyme:
```
LLVM_INSTALL_DIR=<your install location>
LIT=${HOME}/.local/bin/lit   # Update lit install location if necessary

cd ~/src/Enzyme
mkdir -p build && cd build
cmake -G Ninja ../enzyme -DLLVM_DIR=${LLVM_INSTALL_DIR}/lib/cmake/llvm -DLLVM_EXTERNAL_LIT=${LIT}
ninja
```

This will build the Enzyme plugins corresponding to your LLVM version, e.g. `~/src/Enzyme/build/Enzyme/LLDEnzyme-21.so`.

To run unit tests:
```
ninja check-enzyme
```

# Build Instructions

Each example can be built by running `make` from the source subdirectory (e.g. `gradient-harness/src/square`). You will have to set the environment variable LLVM_VERSION_MAJOR to your version before building. 

```
export LLVM_VERSION_MAJOR=<LLVM version>
```

The build will attempt to locate LLVM based on the operating system, or you can manually set the location with the environment variables below: 

```
export LLVM_INSTALL_DIR=<LLVM install location>
```

On Perlmutter, load the llvm module before building: 
```
module load llvm
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
