frac-parser: frac.tab.o lex.yy.o 
	gcc $^ -lfl -o $@ 

%.o: %.c
	gcc -c -o $@ $^

lex.yy.c: frac.l frac.tab.h 
	flex $<
	
frac.tab.h frac.tab.c: frac.y 
	bison -d frac.y

clean:
	find . -mindepth 1 \( -path './.git' -o -name "*.y" -o -name "*.l" -o -name "Makefile" -o -name "*.swp" \) -prune -o -print | xargs rm -f
