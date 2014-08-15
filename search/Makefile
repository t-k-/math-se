OUTPUT=$(shell basename `pwd`)
CFLAGS=-I ../inc -L ..

.PHONY: all clean

all: $(OUTPUT)

$(OUTPUT): main.o ../libparser.a
	gcc $(CFLAGS) $(filter %.o, $^) -o $(OUTPUT) -lparser -ll

-include $(wildcard *.d)

%.o : %.c
	gcc $(CFLAGS) $*.c -c 
	gcc $(CFLAGS) -MM $*.c -o $*.d

clean:
	rm -f *.o *.d $(OUTPUT)
