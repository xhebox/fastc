CC ?= gcc
PREFIX ?= /
MAJVER ?= 0
SUBVER ?= 1
VERSION ?= $(MAJVER).$(SUBVER)

CFLAGS = -O3 -std=c11 -Wall

all: clean
	$(CC) $(CFLAGS) -fPIC -c parser.c
	$(CC) -shared -Wl,-soname,libfcparser.so.0 -o libfcparser.so.0.1 parser.o
	ln -s libfcparser.so.$(VERSION) libfcparser.so.$(MAJVER)
	ln -s libfcparser.so.$(MAJVER) libfcparser.so

install: all
	cp -f libfcparser.so* $(PREFIX)/lib/
	cp -f parser.h $(PREFIX)/include/fcparser.h

uninstall:
	rm -f $(PREFIX)/lib/libfcpaser.so*
	rm -f $(PREFIX)/include/fcparser.h

test: all
	$(CC) $(CFLAGS) test.c -L. -lfcparser -o test
	LD_LIBRARY_PATH=. ./test

clean:
	rm -fr *.o *.so* test

.PHONY: test clean install uninstall
