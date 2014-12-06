include rules.mk

MODULES := helloworld helloworld2 

all: tags $(MODULES:=.sub)

clean: tags-clean $(MODULES:=.clean) 
