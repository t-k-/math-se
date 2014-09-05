BNAME=$(shell basename `pwd`)
CFLAGS=-I ../inc -L .. -I /usr/include/hiredis

.PHONY: all clean

all: $(BNAME) ../lib$(BNAME).a

test: test-rlv_tr

test-rlv_tr: rlv_tr.o test-rlv_tr.o
	gcc $(CFLAGS) $^ -lhiredis -o $@ -lmcheck

DEP_LIB:=-l$(BNAME) -lparser -ll -lbdb_wraper 
DEP_LIB:=$(DEP_LIB) -lhiredis -ltokyocabinet

$(BNAME): main.o ../lib$(BNAME).a ../libparser.a ../libbdb_wraper.a
	@echo dep: $(DEP_LIB)
	gcc $(CFLAGS) $< -o $(BNAME) $(DEP_LIB) -lmcheck

../lib$(BNAME).a: rank.o rlv_tr.o
	ar rcs $@ $^

-include $(wildcard *.d)

%.o : %.c
	gcc $(CFLAGS) $*.c -c 
	gcc $(CFLAGS) -MM $*.c -o $*.d

clean:
	rm -f *.o *.d test-rlv_tr $(BNAME)
