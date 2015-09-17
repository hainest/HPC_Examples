CC = gcc
CCFLAGS = -Wall -Wextra -std=c99 -O3 -march=native -DNDEBUG

CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++14 -O3 -march=native -DNDEBUG

EXEC = test_c test_cpp testD_cpp gen_files
.PHONY: all clean
.DEFAULT_GOAL = all

all: $(EXEC)

test_c: test.c Makefile
	@ echo Compiling $@...
	@ $(CC) $(CCFLAGS) -o $@ $<

testD_cpp: test.cpp Makefile
	@ echo Compiling $@...
	@ $(CXX) $(CXXFLAGS) -UNDEBUG -o $@ $<
	
test_cpp: test.cpp Makefile
	@ echo Compiling $@...
	@ $(CXX) $(CXXFLAGS) -o $@ $<

gen_files: gen_files.cpp Makefile
	@ echo Compiling $@...
	@ $(CXX) $(CXXFLAGS) -o $@ $<

clean:
	@ rm -rf $(EXEC)

dist-clean: clean
	@ rm -rf *.out *.ps numbers*
