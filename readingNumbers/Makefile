CXX = g++ -std=c++11
OPTIMIZE = -O3 -m64 -march=native -mavx
CXXWARN = -Wall -Wextra -Werror

CXXFLAGS = -fdiagnostics-color=auto $(CXXWARN) $(OPTIMIZE)

.PHONY: all clean

all:
	@ echo Use either 'sum' or 'stats'
	@ false

sum: sum.o
	@ echo Building $@...
	$(CXX) $(CXXFLAGS) $< -o $@
	
stats: stats.o
	@ echo Building $@...
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	@ rm -rf *.o sum stats
