apa_conf_path:=/etc/apache2/apache2.conf
web_base_name:=math-se
web_path:=/var/www/$(web_base_name)
cgi_path:=/var/www/$(web_base_name)/cgi
output=rank.cgi
CFLAGS=-I ../inc -I /usr/include/hiredis -L ..
LIBS=-lcurl -lsearch -lparser -ll -lbdb_wraper -lhiredis -ltokyocabinet
COL_DIR=../search/collection

$(output): main.o
	gcc $^ $(CFLAGS) $(LIBS) -o $@

install: permit-cgi $(output)
	sudo cp -u *.png $(web_path)
	sudo cp -u *.html $(web_path)
	sudo cp -u *.cat $(cgi_path)
	sudo cp -u $(output) $(cgi_path)
	@ if [ -e $(COL_DIR) ]; then \
		echo "install collection..."; \
		sudo cp -ru $(COL_DIR) $(cgi_path); \
	else \
		echo "no collection installed"; \
	fi;
	sudo find $(web_path) -print0 | xargs -0 sudo chown www-data
	sudo find $(web_path) -print0 | xargs -0 sudo chgrp www-data

hello-world.cgi: helloworld.o
	gcc helloworld.o -o $@

permit-cgi: hello-world.cgi
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
		sudo tee -a $(apa_conf_path); \
		sudo /etc/init.d/apache2 restart; \
	);
	sudo mkdir -p $(cgi_path)
	sudo cp -u hello-world.cgi $(cgi_path)

-include $(wildcard *.d)

%.o : %.c
	gcc $(CFLAGS) $*.c -c 
	gcc $(CFLAGS) -MM $*.c -o $*.d

clean:
	rm -rf *.d *.o *.cgi
	rm -f $(search_cgi) install

distclean: clean
	sudo rm -rf $(web_path)
