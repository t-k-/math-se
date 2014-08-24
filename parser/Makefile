OUTPUT=$(shell basename `pwd`)
CFLAGS=-I .. -I ../inc -L .
CC=gcc
LIB_NAME=parser

define cl_insert 
@ echo $1 >> toclean.list
endef

.PHONY: all clean

all: $(LIB_NAME) ../lib$(LIB_NAME).a

$(LIB_NAME): main.o ../lib$(LIB_NAME).a
	gcc $< -L .. -l$(LIB_NAME) -lmcheck -ll -lreadline -lncurses -o $@

lex.yy.o y.tab.o: y.tab.h y.tab.c lex.yy.c
	gcc -c $(filter %.c, $^) -include $< -I . -I ../inc
	$(call cl_insert, *.o)

y.tab.h y.tab.c: *.y
	yacc -d $^ --verbose --report=itemset
	$(call cl_insert, y.output)
	$(call cl_insert, y.tab.h)
	$(call cl_insert, y.tab.c)

lex.yy.c: *.l
	lex $^
	$(call cl_insert, $@)

../lib$(LIB_NAME).a: tree.o mathtree.o parser.o lex.yy.o y.tab.o
	ar rcs $@ $^

test: test.c ../lib$(LIB_NAME).a
	$(CC) $< $(CFLAGS) -L .. -l $(LIB_NAME) -o $@

-include $(wildcard *.d)

tree.o: ../tree.c
	$(CC) $(CFLAGS) $^ -c
	$(CC) $(CFLAGS) -MM $^ -o tree.d

%.o : %.c
	$(CC) $(CFLAGS) $*.c -c 
	$(CC) $(CFLAGS) -MM $*.c -o $*.d

clean:
	cat toclean.list | xargs rm -f
	rm -f toclean.list
	rm -f *.o *.d *.a $(LIB_NAME)
