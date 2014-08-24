CFLAGS=-I . -I ./inc -L . 
CC=gcc $(CFLAGS)

.PHONY: all clean submake

all: submake install tags

tags: $(shell find . -name "*.[hcly]" -print)
	@echo dep: $^
	ctags --langmap=c:.c.y -R ./*

submake: 
	make -C ./index
	make -C ./parser
	make -C ./search
	make -C ./web

install:
	make install -C ./web

clean:
	rm -f *.a tags inc/*.gch
	make clean -C ./index
	make clean -C ./parser
	make clean -C ./search
	make clean -C ./web
