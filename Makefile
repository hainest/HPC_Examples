CXX = g++ -std=c++11 -fopenmp -mfpmath=sse -march=native
WFLAGS = -Wall -Wextra -Werror
CXXFLAGS = $(WFLAGS) -O0 -g

OBJS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
EXEC = test

all: $(EXEC)

.PHONY: all clean release

release : CXXFLAGS = $(WFLAGS) -O3
release : $(EXEC)

$(EXEC) : $(OBJS)
	@ echo Building $@...
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

rand_fill.o : rand_fill.cpp
	@ echo Compiling $<...
	$(CXX) $(CXXFLAGS) -c -O3 $< -o $@
	
%.o : %.cpp
	@ echo Compiling $<...
	$(CXX) $(CXXFLAGS) -c $< -o $@
	
clean:
	@ rm -f $(OBJS) $(EXEC)
	@ $(MAKE) --no-print-directory -C loopUnrolling clean
	@ $(MAKE) --no-print-directory -C readingNumbers clean
