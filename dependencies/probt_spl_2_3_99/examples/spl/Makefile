# Sample Makefile to compile ProBT examples with serialization support.
# Probayes, 2007
# 
# Targets:

PROBT_INCLUDE=../../include 
PROBT_LIB=../../lib

CXX=g++

# Environment variable containing the path to the dynamically loaded libraries
ifeq ($(shell uname -s),Darwin)
	# libraries for Mac OS X
	DLL_PATH_VAR := DYLD_LIBRARY_PATH
else
	# libraries for Linux
	DLL_PATH_VAR := LD_LIBRARY_PATH
endif

all: run

# Compiling a C++ source file: add the path to the ProBT includes, the path to the ProBT libraries,
# and link with the ProBT libraries.
serialiseReader: serialiseReader.cpp
	$(DLL_PATH_VAR)=$(PROBT_LIB):${$(DLL_PATH_VAR)} $(CXX) -I$(PROBT_INCLUDE) serialiseReader.cpp -L$(PROBT_LIB) -lspl -o serialiseReader

serialiseWriter: serialiseWriter.cpp
	$(DLL_PATH_VAR)=$(PROBT_LIB):${$(DLL_PATH_VAR)} $(CXX) -I$(PROBT_INCLUDE) serialiseWriter.cpp -L$(PROBT_LIB) -lspl -o serialiseWriter

# Running an example: adjust the LD_LIBRARY_PATH environment variable to point to the directory
# containing the ProBT libraries.
run_writer: serialiseWriter
	@echo ">>> Running the serialization writer. This will read the data file my_data.data. This will create my_learn.gnu (gnuplot file) and my_objects.xml (serialization file)."
	$(DLL_PATH_VAR)=$(PROBT_LIB):${$(DLL_PATH_VAR)} ./serialiseWriter my_data.data

run_reader: serialiseReader
	@echo ">>> Running the serialization reader. This will read my_objects.xml and the data file my_data2.data. This will create my_learn2.gnu (gnuplot file)."
	$(DLL_PATH_VAR)=$(PROBT_LIB):${$(DLL_PATH_VAR)} ./serialiseReader my_objects.xml my_data2.data

show_distributions:
	gnuplot my_learn.gnu my_learn2.gnu

run: run_writer run_reader show_distributions

.PHONY: run run_writer run_reader show_distributions

