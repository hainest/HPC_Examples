CC = gcc
CCFLAGS = -Wall -Wextra -std=c99 -O3 -march=native -DNDEBUG

CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++14 -O3 -march=native -DNDEBUG

EXEC = sort_c sort_cpp sortD_cpp gen_files
.PHONY: all clean
.DEFAULT_GOAL = all

all: $(EXEC)

sort_c: sort.c Makefile
	@ echo Compiling $@...
	@ $(CC) $(CCFLAGS) -o $@ $<

sortD_cpp: sort.cpp Makefile
	@ echo Compiling $@...
	@ $(CXX) $(CXXFLAGS) -UNDEBUG -o $@ $<
	
sort_cpp: sort.cpp Makefile
	@ echo Compiling $@...
	@ $(CXX) $(CXXFLAGS) -o $@ $<

gen_files: gen_files.cpp Makefile
	@ echo Compiling $@...
	@ $(CXX) $(CXXFLAGS) -o $@ $<

clean:
	@ rm -rf $(EXEC)

dist-clean: clean
	@ rm -rf *.out *.ps numbers*
