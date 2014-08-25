all: libbdb_wraper.so ../libbdb_wraper.a
bdb_wraper.o: bdb_wraper.c
	gcc -c -o $@ $^

libbdb_wraper.so: bdb_wraper.c
	gcc -shared -fPIC -o $@ $^ -ltokyocabinet

../libbdb_wraper.a: bdb_wraper.o
	ar rcs $@ $^ 
clean:
	rm -rf *.o collection *.so
