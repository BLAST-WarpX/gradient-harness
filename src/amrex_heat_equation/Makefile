# This example is adapted from amrex-tutorials/ExampleCodes/Basic/HeatEquation_EX0_C 
# https://github.com/AMReX-Codes/amrex-tutorials/tree/main/ExampleCodes/Basic/HeatEquation_EX0_C

# AMREX_HOME defines the directory in which we will find all the AMReX code.
AMREX_HOME ?= $(HOME)/src/amrex

DEBUG        = FALSE
USE_MPI      = FALSE
USE_OMP      = FALSE
BL_NO_FORT   = TRUE
COMP         = llvm  # Note: on Perlmutter, need llvm loaded
DIM          = 2

ENZYME       = FALSE
LLVM_VERSION = 23

include $(AMREX_HOME)/Tools/GNUMake/Make.defs
include ../defs.mk

CEXE_sources += main.cpp
CEXE_headers += myfunc.h

VPATH_LOCATIONS  += .
INCLUDE_LOCATIONS += .

ifeq ($(ENZYME), TRUE)
  CXXFLAGS += -DENZYME
  CXXFLAGS += $(ENZYME_CXXFLAGS)
  LDFLAGS += $(ENZYME_LDFLAGS)
endif

# Verbose linker output
#LDFLAGS += -Wl,-v

include $(AMREX_HOME)/Src/Base/Make.package

include $(AMREX_HOME)/Tools/GNUMake/Make.rules
