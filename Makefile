CC ?= gcc
PREFIX ?= /
DESTDIR ?= /
MAJVER ?= 0
SUBVER ?= 1
VERSION ?= $(MAJVER).$(SUBVER)

CFLAGS = -O3 -std=c11 -Wall

all: lib
	$(CC) $(CFLAGS) main.c -o fcparser -luv -L. -lfcparser

lib: clean
	$(CC) $(CFLAGS) -fPIC -c parser.c
	$(CC) -shared -Wl,-soname,libfcparser.so.0 -o libfcparser.so.0.1 parser.o
	ln -s libfcparser.so.$(VERSION) libfcparser.so.$(MAJVER)
	ln -s libfcparser.so.$(MAJVER) libfcparser.so

install-libs: all
	mkdir -p $(DESTDIR)/$(PREFIX)/lib
	mkdir -p $(DESTDIR)/$(PREFIX)/include
	cp -f libfcparser.so* $(DESTDIR)/$(PREFIX)/lib/
	cp -f parser.h $(DESTDIR)/$(PREFIX)/include/fcparser.h

install: install-libs
	mkdir -p $(DESTDIR)/$(PREFIX)/bin
	cp -f fcparser $(DESTDIR)/$(PREFIX)/bin/

uninstall:
	rm -f $(DESTDIR)/$(PREFIX)/lib/libfcpaser.so*
	rm -f $(DESTDIR)/$(PREFIX)/include/fcparser.h
	rm -f $(DESTDIR)/$(PREFIX)/bin/fcparser

test: lib
	$(CC) $(CFLAGS) test.c -L. -lfcparser -o test
	LD_LIBRARY_PATH=. ./test

clean:
	rm -fr *.o *.so* test fcparser

.PHONY: test clean install uninstall
