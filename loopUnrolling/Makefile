OBJS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
EXEC = loop_unrolling

include ../Makefile.include

all: $(EXEC)

.PHONY: all clean release dist-clean

release : CXXFLAGS = $(WFLAGS) -O3
release : $(EXEC)

rand_fill.o : rand_fill.cpp
	@ echo Compiling $<...
	@ $(CXX) $(INC) $(CXXFLAGS) -c -O3 $< -o $@
	
clean:
	@ echo Cleaning loopUnrolling...
	@ rm -f $(OBJS) $(EXEC)

dist-clean: clean