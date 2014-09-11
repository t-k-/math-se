CFLAGS=-I . -I ./inc -L . 
CC=gcc $(CFLAGS)

.PHONY: all clean submake mklink

all: submake tags mklink

tags: $(shell find . -name "*.[hcly]" -print)
	@echo dep: $^
	ctags --langmap=c:.c.y -R ./*

mklink: submake
	@ echo making symbolic links...
	@ [ -e search.ln ] || ln -s `pwd`/search/search `pwd`/search.ln

submake: 
	make -C ./index
	make -C ./parser
	make -C ./search
	make -C ./web

install: submake
	make install -C ./web

clean:
	rm -f *.a tags inc/*.gch *.ln
	find . -name "*.pyc" | xargs rm -f
	make clean -C ./index
	make clean -C ./parser
	make clean -C ./search
	make clean -C ./web

distclean: clean
	find . -name "collection" | xargs rm -rf
	make distclean -C ./web
