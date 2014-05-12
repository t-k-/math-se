search_page = search.bin

$(search_page): httpd.c search.html
	gcc $< -lcurl -L . -I ./include -o $@
	chmod +x $@
	cp $@ /var/www/cgi/
	cp $^ /var/www/html/

clean:
	rm -f $(search_page)
