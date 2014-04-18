PARSER=ma-pa

all: $(PARSER) test-tree

%.o: %.c
	gcc $^ -I ./inc -c 

mathtree.a: tree.o list.o mathtree.o
	ar rcs libmathtree.a $^

test-tree: test-tree.c mathtree.a 
	gcc $< -lmathtree -L . -I ./inc -o $@

$(PARSER): $(PARSER).tab.o $(PARSER).yy.o
	gcc $^ -lfl -mcheck -o $@ 

%.tab.o: %.tab.c
	gcc -c -o $@ $^

%.yy.o: lex.yy.c %.tab.h
	gcc -c -o $@ $< -include $(word 2, $^)

lex.yy.c: $(PARSER).l
	flex $<

parse = bison --verbose --report=itemset -d $^
%.tab.h %.tab.c: %.y 
	$(parse) 2>&1 | grep --color conflicts || $(parse) 

clean:
	rm -f lex.yy.c *.output *.tab.h *.tab.c *.a *.o $(PARSER) test-tree
