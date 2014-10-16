BNAME=$(shell basename `pwd`)
CFLAGS=-I . -I ../inc -L .. -I /usr/include/hiredis 

.PHONY: all clean

all: $(BNAME) $(BNAME)-time ../lib$(BNAME).a

test: test-rlv_tr

test-rlv_tr: rlv_tr.o test-rlv_tr.o
	gcc $(CFLAGS) $^ -lhiredis -o $@ -lmcheck

DEP_LIB:=-lparser -ll -lbdb_wraper -lhiredis -ltokyocabinet -lmcheck

$(BNAME): main.o elapsed.o rank_verb.o rlv_tr.o ../libparser.a ../libbdb_wraper.a
	gcc $(CFLAGS) $(filter %.o, $^) -o $@ $(DEP_LIB)

$(BNAME)-time: main.o rank_time.o elapsed.o rlv_tr.o ../libparser.a ../libbdb_wraper.a
	gcc $(CFLAGS) $(filter %.o, $^) -o $@ $(DEP_LIB)

../lib$(BNAME).a: rank.o rlv_tr.o
	ar rcs $@ $^

rank_verb.o : rank.c
	gcc -DRK_VERBOSE $(CFLAGS) -c $^ -o $@
	gcc $(CFLAGS) -MM $^ -o rank_verb.d

rank_time.o : rank.c
	gcc -DRK_TIME_TEST $(CFLAGS) -c $^ -o $@
	gcc $(CFLAGS) -MM $^ -o rank_time.d

-include $(wildcard *.d)

%.o : %.c
	gcc $(CFLAGS) $*.c -c 
	gcc $(CFLAGS) -MM $*.c -o $*.d

clean:
	rm -f *.o *.d test-rlv_tr $(BNAME) $(BNAME)-time
