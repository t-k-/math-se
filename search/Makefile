OUTPUT=$(shell basename `pwd`)
CFLAGS=-I ../inc -L .. -I /usr/include/hiredis
DEP_LIB=-lparser -ll -lhiredis -lbdb_wraper -ltokyocabinet

.PHONY: all clean

all: $(OUTPUT)

test: test-rlv_tr

test-rlv_tr: rlv_tr.o test-rlv_tr.o
	gcc $(CFLAGS) $^ -lhiredis -o $@ -lmcheck

$(OUTPUT): main.o rlv_tr.o ../libparser.a ../libbdb_wraper.a
	gcc $(CFLAGS) $(filter %.o, $^) -o $(OUTPUT) $(DEP_LIB) -lmcheck

-include $(wildcard *.d)

%.o : %.c
	gcc $(CFLAGS) $*.c -c 
	gcc $(CFLAGS) -MM $*.c -o $*.d

clean:
	rm -f *.o *.d test-rlv_tr $(OUTPUT)
