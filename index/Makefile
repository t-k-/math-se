libbdb_wraper.so: bdb_wraper.c
	gcc -shared -fPIC -o $@ $^ -ltokyocabinet
clean:
	rm -rf collection *.so
