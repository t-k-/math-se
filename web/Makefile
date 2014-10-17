apa_conf_path:=/etc/apache2/apache2.conf
web_base_name:=math-se
web_path:=/var/www/$(web_base_name)
cgi_path:=/var/www/$(web_base_name)/cgi
output=rank.cgi
CFLAGS=-I ../inc -I /usr/include/hiredis -L ..
LIBS=-lcurl -lsearch -lparser -ll -lbdb_wraper -lhiredis -ltokyocabinet
COL_DIR=../col
check_perm=@ touch /root/test 2>/dev/null || \
           ($(call color, "* ROOT REQUIRED. *") && exit 1)
COLOR_RED=$(shell tput setaf 1)
COLOR_NIL=$(shell tput sgr0)

color=echo $(COLOR_RED) $1 $(COLOR_NIL)

$(output): main.o
	gcc $^ $(CFLAGS) $(LIBS) -o $@

install: permit-cgi $(output)
	$(check_perm)
	cp -u *.png $(web_path)
	cp -u *.html $(web_path)
	cp -u *.cat $(cgi_path)
	cp -u $(output) $(cgi_path)
	@ if [ -e $(COL_DIR) ]; then \
		echo -e $(C_RED) "install collection..." $(C_END); \
		cp -ru $(COL_DIR) $(cgi_path); \
	else \
		$(call color, "* NO COLLECTION INSTALLED *"); \
	fi;
	find $(web_path) -print0 | xargs -0 chown www-data
	find $(web_path) -print0 | xargs -0 chgrp www-data
	chmod +x $(cgi_path)/*.cgi

hello-world.cgi: helloworld.o
	gcc helloworld.o -o $@

permit-cgi: hello-world.cgi
	$(check_perm)
	@ if [ ! -e $(apa_conf_path) ]; then \
		echo '[ apache2 not installed ]'; \
		exit; \
	fi; \
	cat $(apa_conf_path) | grep '$(web_base_name)' || \
	( \
		echo 'config the apache2...'; \
		echo "\n# $(web_base_name)\nScriptAlias" \
		"/$(web_base_name)-bin/ $(cgi_path)/\nAlias" \
		"/$(web_base_name)/ $(web_path)/\n" | \
		tee -a $(apa_conf_path); \
		/etc/init.d/apache2 restart; \
	);
	mkdir -p $(cgi_path)
	cp -u hello-world.cgi $(cgi_path)

-include $(wildcard *.d)

%.o : %.c
	gcc $(CFLAGS) $*.c -c 
	gcc $(CFLAGS) -MM $*.c -o $*.d

clean:
	rm -rf *.d *.o *.cgi
	rm -f $(search_cgi) install

distclean: clean
	$(check_perm)
	rm -rf $(web_path)
