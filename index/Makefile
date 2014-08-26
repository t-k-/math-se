all: libbdb_wraper.so ../libbdb_wraper.a
bdb_wraper.o: bdb_wraper.c
	gcc -c -o $@ $^

libbdb_wraper.so: bdb_wraper.c
	gcc -shared -fPIC -o $@ $^ -ltokyocabinet

../libbdb_wraper.a: bdb_wraper.o
	ar rcs $@ $^ 

test: test.c bdb_wraper.c
	gcc -I ../inc $^ -ltokyocabinet -o $@

clean:
	rm -rf *.o collection *.so test *.bdb
