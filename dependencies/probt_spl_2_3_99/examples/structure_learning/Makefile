# Sample Makefile to compile ProBT structure-learning examples.
# Probayes, 2008
# 
# Targets:

PROBT_INCLUDE=../../../include
SL_INCLUDE=$(PROBT_INCLUDE)/structure_learning
PROBT_LIB=../../../lib

CXX=g++
CXXFLAGS=-I$(PROBT_INCLUDE) -I$(SL_INCLUDE) 
LDFLAGS=-L$(PROBT_LIB) -lspl 
CXXFLAGS+=-I/opt/local/include

# Environment variable containing the path to the dynamically loaded libraries
ifeq ($(shell uname -s),Darwin)
	# libraries for Mac OS X
	DLL_PATH_VAR := DYLD_LIBRARY_PATH
else
	# libraries for Linux
	DLL_PATH_VAR := LD_LIBRARY_PATH
endif

all: run_test

# Compiling a C++ source file: add the path to the ProBT includes, the path to the ProBT libraries,
# and link with the ProBT libraries.
structure_learning: structure_learning.cpp common.cpp asia.cpp
	$(DLL_PATH_VAR)=$(PROBT_LIB):${$(DLL_PATH_VAR)} $(CXX) $(CXXFLAGS) structure_learning.cpp common.cpp asia.cpp $(LDFLAGS)  -o structure_learning

# Running an example: adjust the LD_LIBRARY_PATH environment variable to point to the directory
# containing the ProBT libraries.
run_test: structure_learning
	@echo ">>> Running the structure-learning example. This will create some files:"
	@echo ">>>   bayesian_network.fig: the original bayesian network"
	@echo ">>>   model_asia.csv: 10000 points generated from the bayesian network"
	@echo ">>>   model_asia.xml: the bayesian network saved as a ProBT serialization archive"
	@echo ">>>   dmst_bic.xml: result of structure learning with the DMST algorithm"
	@echo ">>>   dmst-bs_bic.fig.: result of structure learning with DMST + GS"
	$(DLL_PATH_VAR)=$(PROBT_LIB):${$(DLL_PATH_VAR)} ./structure_learning

.PHONY: run_test

