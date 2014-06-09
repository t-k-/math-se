OUTPUT=$(shell basename `pwd`)

define cl_insert 
@ echo $1 >> clean.list
endef

.PHONY: all clean

all: $(OUTPUT)

$(OUTPUT): y.tab.h y.tab.c lex.yy.c
	gcc $(filter %.c, $^) -ll -o $@
	$(call cl_insert, $@)

y.tab.h y.tab.c: *.y
	yacc -d $^
	$(call cl_insert, y.tab.h)
	$(call cl_insert, y.tab.c)

lex.yy.c: *.l
	lex $^
	$(call cl_insert, $@)

clean:
	cat clean.list | xargs rm -f
	rm -f clean.list
