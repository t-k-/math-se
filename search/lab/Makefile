.PHONY: all clean

all: posmerge dirmerge

posmerge: posmerge.c clean
	gcc $< -o $@

dirmerge: dirmerge.c clean
	gcc $< -o $@
	
clean:
	rm -f *.bin posmerge dirmerge 
