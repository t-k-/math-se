PARSER=ma-pa
SEARCHER=ma-se
CFLAGS=-I . -I ./inc -L . 
CC=gcc $(CFLAGS)

.PHONY: all clean submake

all: submake $(PARSER) $(SEARCHER) libbdb_wraper.so install tags

libbdb_wraper.so: bdb_wraper.c
	gcc -shared -fPIC -o $@ $^ -ltokyocabinet

tags: $(shell find . -name "*.[hcly]" -print)
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

PARSER_DEP=$(addprefix parser/, lex.yy.o y.tab.o)

$(PARSER): $(PARSER_DEP) parser/libmathtree.a
	$(CC) $(PARSER_DEP) -L parser -lmathtree -lmcheck -ll -lreadline -lncurses \
	-o $@ 

$(SEARCHER): $(SEARCHER).c list.o
	$(CC) $^ -o $@

clean:
	rm -f *.a *.o *.d $(PARSER) test-tree query candy score tags rank rand $(SEARCHER) libbdb_wraper.so
	make clean -C ./front-end
	make clean -C ./parser
