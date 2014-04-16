all: cal 

%: %.tab.o %.yy.o
	gcc $^ -lfl -mcheck -o $@ 

%.tab.o: %.tab.c
	gcc -c -o $@ $^

%.yy.o: lex.yy.c %.tab.h
	gcc -c -o $@ $< -include $(word 2, $^)

lex.yy.c: cal.l 
	flex $<
	
parse = bison --verbose --report=itemset -d $^
%.tab.h %.tab.c: %.y 
	$(parse) 2>&1 | grep --color conflicts || $(parse) 

clean:
	find . -mindepth 1 \( -path './.git' -o -name "*.[yl]" -o -name "list*" -o -name "README.md" -o -name "Makefile" -o -name "*.swp" \) -prune -o -print | xargs rm -f
