SEARCHER=search
CFLAGS=-I . -I ./inc -L . 
CC=gcc $(CFLAGS)

.PHONY: all clean submake

all: submake $(SEARCHER) install tags

tags: $(shell find . -name "*.[hcly]" -print)
	@echo dep: $^
	ctags --langmap=c:.c.y -R ./*

submake: 
	make -C ./index
	make -C ./parser
	make -C ./web

install:
	make install -C ./web

-include $(wildcard *.d)

%.o : %.c
	$(CC) $*.c -c 
	$(CC) -MM $*.c -o $*.d

$(SEARCHER): $(SEARCHER).o
	$(CC) $^ -o $@

clean:
	rm -f *.o *.d query candy score tags rank rand $(SEARCHER)
	make clean -C ./web
	make clean -C ./parser
	make clean -C ./index
