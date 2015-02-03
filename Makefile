################################################################################
#
#	Makefile for namegen.c
#	Version 1.0, 2015-02-02
#	James Hendrie <hendrie.james@gmail.com>
#
################################################################################
CC=gcc
OPTFLAGS=-O3
#OPTFLAGS=-g -Wall
ROOTPATH=
PREFIX=$(ROOTPATH)/usr
SRC=src
SRCFILE=namegen.c
DESTFILE=namegen
DATA=data
PROGPATH=$(PREFIX)/share/namegen
DATAPATH=$(PROGPATH)/$(DATA)
NAMES1=firstnames_male.txt
NAMES2=firstnames_female.txt
NAMES3=surnames.txt

all:	$(SRC)/$(SRCFILE)
	$(CC) $(OPTFLAGS) -o $(DESTFILE) src/$(SRCFILE)

install:
	install -D -g 0 -o 0 -m 0755 $(DESTFILE) $(PREFIX)/bin/$(DESTFILE)
	install -v -D -g 0 -o 0 -m 0644 LICENSE $(PROGPATH)/LICENSE
	install -v -D -g 0 -o 0 -m 0644 README $(PROGPATH)/README
	install -v -D -g 0 -o 0 -m 0644 $(DATA)/$(NAMES1) $(DATAPATH)/$(NAMES1)
	install -v -D -g 0 -o 0 -m 0644 $(DATA)/$(NAMES2) $(DATAPATH)/$(NAMES2)
	install -v -D -g 0 -o 0 -m 0644 $(DATA)/$(NAMES3) $(DATAPATH)/$(NAMES3)

uninstall:
	rm -vf $(PREFIX)/bin/$(DESTFILE)
	rm -vf $(DATAPATH)/$(NAMES1)
	rm -vf $(DATAPATH)/$(NAMES2)
	rm -vf $(DATAPATH)/$(NAMES3)
	rmdir -v $(DATAPATH)
	rm -vf $(PROGPATH)/LICENSE
	rm -vf $(PROGPATH)/README
	rmdir -v $(PROGPATH)
