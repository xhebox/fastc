CC ?= gcc
PREFIX ?= /

CFLAGS = -O3 -std=c11 -Wall

all:
	$(CC) $(CFLAGS) -fPIC -c parser.c
	$(CC) -shared -o libfcparser.so parser.o

install: all
	cp -f libfcparser.so $(PREFIX)/lib/libfcpaser.so
	cp -f parser.h $(PREFIX)/include/fcparser.h

uninstall:
	rm -f $(PREFIX)/lib/libfcpaser.so
	rm -f $(PREFIX)/include/fcparser.h

test:
	$(CC) $(CFLAGS) test.c -L. -lfcparser -o test
	LD_LIBRARY_PATH=. ./test

clean:
	rm -fr *.o *.so test

.PHONY: test clean install uninstall
