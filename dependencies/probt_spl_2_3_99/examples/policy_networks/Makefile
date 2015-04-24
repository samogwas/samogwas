# Sample Makefile to compile ProBT examples.
# Probayes, 2009

PROBT_INCLUDE=../../include
PROBT_LIB=../../lib

CXX=g++ -Wall

# Environment variable containing the path to the dynamically loaded libraries
ifeq ($(shell uname -s),Darwin)
	# libraries for Mac OS X
	DLL_PATH_VAR := DYLD_LIBRARY_PATH
else
	# libraries for Linux
	DLL_PATH_VAR := LD_LIBRARY_PATH
endif

all: run

# Compiling C++ source files: add the path to the ProBT includes, the
# path to the ProBT libraries, and link with the ProBT libraries.
infected_milk: infected_milk.cpp
	$(DLL_PATH_VAR)=$(PROBT_LIB):${$(DLL_PATH_VAR)} $(CXX) -I$(PROBT_INCLUDE) infected_milk.cpp -L$(PROBT_LIB) -lspl -o infected_milk

oil_wildcatter: oil_wildcatter.cpp
	$(DLL_PATH_VAR)=$(PROBT_LIB):${$(DLL_PATH_VAR)} $(CXX) -I$(PROBT_INCLUDE) oil_wildcatter.cpp -L$(PROBT_LIB) -lspl -o oil_wildcatter

# Running the examples: adjust the LD_LIBRARY_PATH environment variable
# to point to the directory containing the ProBT libraries.
run: infected_milk oil_wildcatter
	@echo ">>> Running the examples. It will create the files infected_milk.dot and oil_wildcatter.dot (figures in in graphviz format)."
	$(DLL_PATH_VAR)=$(PROBT_LIB):${$(DLL_PATH_VAR)} ./infected_milk
	$(DLL_PATH_VAR)=$(PROBT_LIB):${$(DLL_PATH_VAR)} ./oil_wildcatter

show: run
	display infected_milk.dot
	display oil_wildcatter.dot

.PHONY: run show

