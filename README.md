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

Currently AMReX is only needed for `amrex_heat_equation`.

To install dependencies:

```
cd ~/src
git clone git@github.com:EnzymeAD/Enzyme.git
git clone git@github.com:amrex-codes/amrex.git
```

## Building Enzyme
Instructions to build Enzyme from the source are on the [Enzyme website](https://enzyme.mit.edu/Installation/). We state
the current build steps below, but we should check their website for updates. Since the purpose of `gradient-harness` is to test 
Enzyme on different code examples and make modifications if necessary, we should build directly from the source rather than
using a package manager.

If lit (LLVM Integration Tester) is not installed in your path and you wish to run Enzyme's unit tests, you need to install it.
Normally lit is installed with pipx. The install location should be `~/.local/bin/lit`, but check your install to confirm.
If you are not using the unit tests, you can omit `-DLLVM_EXTERNAL_LIT` from the cmake command below. 
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

### Enzyme unit tests
Note that the Enzyme unit tests require the LLVM FileCheck command. By default, this is not installed with Perlmutter's llvm module,
so you will need to create a spack virtual environment and install LLVM there with the `+utils` modification
(see [Spack virtual environments](#spack-virtual-environments)).

To run unit tests:
```
ninja check-enzyme
```

# Build Instructions
Each example can be built by running `make` from the source subdirectory (e.g. `gradient-harness/src/square`). You will have to set 
the environment variable `LLVM_VERSION_MAJOR` to your version before building. 

```
export LLVM_VERSION_MAJOR=<LLVM version>
```

The build will attempt to locate LLVM based on the operating system, or you can manually set the location with the environment variable `LLVM_INSTALL_DIR`: 
```
export LLVM_INSTALL_DIR=<LLVM install location>
```

## Perlmutter build instructions
If you are using lmod, load the `llvm` module before building: 
```
module load llvm
```

This will automatically add the desired versions of clang/clang++ to your path.

If you are using a spack virtual environment, activate the environment:
```
SPACK_ENV_NAME=<env name>
spack env activate ${SPACK_ENV_NAME}
```

# Tests
On Perlmutter, load the python module before running the `gradient-harness` unit tests:
```
module load python
```

To run the unit tests:
```
make pybind
python -m unittest -v <test file>.py
```

# Additional Perlmutter notes

## Spack virtual environments
If you wish to test with a specific version of LLVM on Perlmutter which isn't available as a module, you will have to set up a spack
virtual environment. First set you spack user cache to `$PSCRATCH/.spack` in `~/.bashrc`. This forces spack to install packages in
the scratch file system, so your home directory doesn't fill up. 
```
echo -e '\n# Set spack user cache\nexport SPACK_USER_CACHE_PATH=${PSCRATCH}/.spack' >> ~/.bashrc
source ~/.bashrc
```

Then create and activate the environment:
```
export LLVM_VERSION_MAJOR=<llvm version>
export SPACK_ENV_NAME=gradient-harness-llvm-${LLVM_VERSION_MAJOR}

spack env create ${SPACK_ENV_NAME}
spack env activate ${SPACK_ENV_NAME}
```

Install LLVM and other desired packages. Note that LLVM builds are computationally intensive, so you should run the build through 
an interactive slurm job (commands below). To prevent issues with ssh disconnects, you should start a tmux sessions before running `salloc`.
Note also that we need the `+utils` variant, which adds utilities that are used by LLVM's unit tests.
```
spack add llvm@${LLVM_VERSION_MAJOR}+utils 

# You should run salloc in a tmux session in case ssh disconnects
# Note: DO NOT run spack install with `-j 64` (64 threads) on log in nodes
#       This uses too many resources in the shared login node environment
salloc --time=04:00:00 --qos=interactive --constraint=cpu --nodes=1
spack install -j 64
```

