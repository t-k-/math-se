CFLAGS=-I . -I ./inc -L . 
CC=gcc $(CFLAGS)
FIND=find . -type d \( -path './*col' -o -path './.git' \) -prune -o 

.PHONY: all clean submake mklink tags

all: submake tags mklink

tags:
	$(FIND) -name '*.[hcly]' -print > tags.list 
	ctags --langmap=c:.c.y -L tags.list

mklink: submake
	@ echo making symbolic links...
	@ [ -e search.ln ] || ln -s `pwd`/search/search `pwd`/search.ln
	@ [ -e search-time.ln ] || ln -s `pwd`/search/search-time `pwd`/search-time.ln
	@ [ -e bdb_probe.ln ] || ln -s `pwd`/index/bdb_probe `pwd`/bdb_probe.ln
	@ [ -e test/parse/rand-cmd.sh.ln ] || \
	ln -s `pwd`/crawler/rand-cmd.sh `pwd`/test/parse/rand-cmd.sh.ln
	@ [ -e test/parse/parser.ln ] || \
	ln -s `pwd`/parser/parser `pwd`/test/parse/parser.ln
	@ [ -e test/eval/parser.ln ] || \
	ln -s `pwd`/parser/parser `pwd`/test/eval/parser.ln
	@ [ -e test/eval/index.py.ln ] || \
	ln -s `pwd`/index/index.py `pwd`/test/eval/index.py.ln
	@ [ -e test/eval/libbdb_wraper.so ] || \
	ln -s `pwd`/index/libbdb_wraper.so `pwd`/test/eval/libbdb_wraper.so
	@ [ -e test/eval/where.sh.ln ] || \
	ln -s `pwd`/test/parse/1-where.sh `pwd`/test/eval/where.sh.ln

submake: 
	make -C ./index
	make -C ./parser
	make -C ./search
	make -C ./web

install: submake mklink
	make install -C ./web

clean:
	rm -f *.a tags inc/*.gch tags.list
	$(FIND) -name '*.pyc' -print | xargs rm -f
	$(FIND) -name '*.ln' -print | xargs rm -f
	$(FIND) -name '*.so' -print | xargs rm -f
	make clean -C ./index
	make clean -C ./parser
	make clean -C ./search
	make clean -C ./web

distclean: clean
	make distclean -C ./web
