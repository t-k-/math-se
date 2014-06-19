PARSER=ma-pa
SEARCHER=ma-se
CFLAGS=-I ./inc -L . 
CC=gcc $(CFLAGS)

.PHONY: all clean submake

all: submake $(PARSER) $(SEARCHER) install tags

tags: $(shell find . -name "*.[hyc]" -print)
	@echo dep: $^
	ctags --langmap=c:.c.y -R ./*

submake: 
	make -C ./parser
	make -C ./front-end

install:
	make install -C ./front-end

-include $(wildcard *.d)

%.o : %.c
	$(CC) $*.c -c 
	$(CC) -MM $*.c -o $*.d

libmathtree.a: tree.o list.o mathtree.o
	ar rcs libmathtree.a $^

test-tree: test-tree.c libmathtree.a 
	$(CC) $< -lmathtree -o $@

PARSER_DEP=$(addprefix parser/, lex.yy.o y.tab.o)
$(PARSER): $(PARSER_DEP) libmathtree.a
	$(CC) $(PARSER_DEP) -lmathtree -lmcheck -ll -o $@ 

$(SEARCHER): $(SEARCHER).c libmathtree.a
	$(CC) $< -lmathtree -o $@

clean:
	rm -f *.a *.o *.d $(PARSER) test-tree query candy score tags rank $(SEARCHER)
	make clean -C ./front-end
	make clean -C ./parser
