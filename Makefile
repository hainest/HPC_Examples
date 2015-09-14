.PHONY: all clean release
.DEFAULT_GOAL = all

release: Makefile
	@ $(MAKE) --no-print-directory -C loopUnrolling release
	@ $(MAKE) --no-print-directory -C memoryLayout release

all: Makefile
	@ $(MAKE) --no-print-directory -C loopUnrolling all
	@ $(MAKE) --no-print-directory -C memoryLayout all
	
clean:
	@ $(MAKE) --no-print-directory -C loopUnrolling clean
	@ $(MAKE) --no-print-directory -C memoryLayout clean
