
ASCIIDOC_FILTERS ?= /usr/share/asciidoc/filters


ifeq ($(USER),lvv)
	HOMEDIR := /home/lvv/p/volnitsky.com/
	INCLUDE := $(HOMEDIR)/include.mk 
else
	INCLUDE := /dev/null
endif


include $(INCLUDE)

index.txt: 
	head -n-1 README.asciidoc > /tmp/t.ad

CXXFLAGS=-std=gnu++0x -Wall  -I/home/lvv/p/ 
t-buf: CXXFLAGS+=-fno-default-inline  -O0 -ggdb 

#CLEAN_LIST += $(wildcard *.png)
#COPY_LIST += $(wildcard *.png)
