search_page = search.bin

$(search_page): httpd.c search.html
	gcc $< -lcurl -L . -I ./include -o $@
	chmod +x $@
	sudo cp $@ /var/www/cgi/
	sudo cp $^ /var/www/html/

clean:
	rm -f $(search_page)
