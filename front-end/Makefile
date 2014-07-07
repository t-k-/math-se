search_cgi = result.bin

$(search_cgi): front-end.c search.html template.html 
	gcc $< -lcurl -L . -I ./include -o $@
	chmod +x $@

install: $(search_cgi) ../ma-pa ../ma-se *.png *.jpg *.cat *.html
	mkdir -p /var/www/html/ /var/www/cgi/
	for f in $^; do cp $$f /var/www/cgi/; done
	cp /var/www/cgi/*.jpg /var/www/html/
	cp /var/www/cgi/*.png /var/www/html/
	cp /var/www/cgi/*.html /var/www/html/
	touch install

clean:
	rm -f $(search_cgi) install
