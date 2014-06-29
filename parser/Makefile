OUTPUT=$(shell basename `pwd`)

define cl_insert 
@ echo $1 >> toclean.list
endef

.PHONY: all clean

all: lex.yy.o y.tab.o

lex.yy.o y.tab.o: y.tab.h y.tab.c lex.yy.c
	gcc -c $(filter %.c, $^) -include $< -I .. -I ../inc
	$(call cl_insert, *.o)

y.tab.h y.tab.c: *.y
	yacc -d $^ --verbose --report=itemset
	$(call cl_insert, y.output)
	$(call cl_insert, y.tab.h)
	$(call cl_insert, y.tab.c)

lex.yy.c: *.l
	lex $^
	$(call cl_insert, $@)

clean:
	cat toclean.list | xargs rm -f
	rm -f toclean.list
