OUTPUT=$(shell basename `pwd`)
CFLAGS=-I .. -I ../inc -L .
CC=gcc

define cl_insert 
@ echo $1 >> toclean.list
endef

.PHONY: all clean

all: lex.yy.o y.tab.o libmathtree.a

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

libmathtree.a: tree.o list.o mathtree.o
	ar rcs libmathtree.a $^

test: test.c libmathtree.a 
	$(CC) $< $(CFLAGS) -lmathtree -o $@

list.o : 
	$(CC) $(CFLAGS) ../list.c -c 
	$(CC) $(CFLAGS) -MM ../list.c -o list.d

%.o : %.c
	$(CC) $(CFLAGS) $*.c -c 
	$(CC) $(CFLAGS) -MM $*.c -o $*.d

clean:
	cat toclean.list | xargs rm -f
	rm -f toclean.list
