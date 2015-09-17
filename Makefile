.PHONY: all clean release
.DEFAULT_GOAL = all

release: Makefile
	@ $(MAKE) --no-print-directory -C loopUnrolling release
	@ $(MAKE) --no-print-directory -C memoryLayout release
	@ $(MAKE) --no-print-directory -C CvsC++ release

all: Makefile
	@ $(MAKE) --no-print-directory -C loopUnrolling all
	@ $(MAKE) --no-print-directory -C memoryLayout all
	@ $(MAKE) --no-print-directory -C CvsC++ all
	
clean:
	@ $(MAKE) --no-print-directory -C loopUnrolling clean
	@ $(MAKE) --no-print-directory -C memoryLayout clean
	@ $(MAKE) --no-print-directory -C CvsC++ clean

dist-clean:
	@ $(MAKE) --no-print-directory -C loopUnrolling dist-clean
	@ $(MAKE) --no-print-directory -C memoryLayout dist-clean
	@ $(MAKE) --no-print-directory -C CvsC++ dist-clean