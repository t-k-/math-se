PARSER=ma-pa
SEARCHER=ma-se
CFLAGS=-I . -I ./inc -L . 
CC=gcc $(CFLAGS)
READLN_ROOT := ./readline-6.3
CURSES_ROOT := ./ncurses-5.9
READLN_LIB := $(READLN_ROOT)/libreadline.a
CURSES_LIB := $(CURSES_ROOT)/lib/libncurses.a

.PHONY: all clean submake

all: submake $(PARSER) $(SEARCHER) install tags

tags: $(shell find . -type d \( -path $(READLN_ROOT) -o -path $(CURSES_ROOT) \) -prune -o -name "*.[hcly]" -print)
	@echo dep: $^
	ctags --langmap=c:.c.y -R ./*

parser/readline:
	rm -f $@
	ln -s `pwd`/readline-6.3/ $@ 

submake: parser/readline
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

$(READLN_LIB):
	cd $(READLN_ROOT) && ./configure
	make -C $(READLN_ROOT)

$(CURSES_LIB):
	cd $(CURSES_ROOT) && ./configure
	make -C $(CURSES_ROOT)

$(PARSER): $(PARSER_DEP) libmathtree.a $(READLN_LIB) $(CURSES_LIB)
	$(CC) $(PARSER_DEP) -lmathtree -lmcheck -ll \
	-L $(shell dirname $(READLN_LIB)) -lreadline \
	-L $(shell dirname $(CURSES_LIB)) -lncurses \
	-o $@ 

$(SEARCHER): $(SEARCHER).c libmathtree.a
	$(CC) $< -lmathtree -o $@

clean:
	rm -f *.a *.o *.d $(PARSER) test-tree query candy score tags rank rand $(SEARCHER)
	make clean -C ./front-end
	make clean -C ./parser
	make distclean -C $(READLN_ROOT)
	make distclean -C $(CURSES_ROOT)
	rm -f parser/readline
