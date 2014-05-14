search_page = result.bin

$(search_page): httpd.c search.html template.html 
	gcc $< -lcurl -L . -I ./include -o $@
	chmod +x $@
	cp $@ /var/www/cgi/
	cp ../ma-pa /var/www/cgi/
	cp ../ma-se /var/www/cgi/
	cp ./*.cat /var/www/cgi/
	cp ./*.html /var/www/html/
	cp ./*.png /var/www/html/
	cp ./*.png /var/www/cgi/

clean:
	rm -f *.bin
