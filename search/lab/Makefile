.PHONY: all clean

all: test

test: posmerge.c clean
	gcc $< -o $@
	
clean:
	rm -f *.bin test
