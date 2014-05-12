search_page = search.bin

$(search_page): httpd.c search.html
	gcc $< -lcurl -L . -I ./include -o $@
	chmod +x $@
	cp $@ /var/www/cgi/
	cp $^ /var/www/html/
	cp ../ma-pa /var/www/cgi/
	cp ../ma-se /var/www/cgi/

clean:
	rm -f $(search_page)
