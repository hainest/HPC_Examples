OBJS = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
EXEC = memory_layout

include ../Makefile.include

.PHONY: all clean release dist-clean

release : CXXFLAGS = $(WFLAGS) -O3
release : $(EXEC)

all: $(EXEC)

clean:
	@ echo Cleaning memoryLayout...
	@ rm -f $(OBJS) $(EXEC)

dist-clean: clean