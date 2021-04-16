CC=gcc
CFLAGS=-std=c99
LDFLAGS=-lX11 -lXtst
OBJS=mousemode.o
OUT=mousemode
DESTDIR=
PREFIX=/usr/local
MANPREFIX=$(PREFIX)/share/man

VERSION=1.0

all: main

main: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $(OUT)

%.o: %.c
	$(CC) -c $(CFLAGS) $^ -o $@

clean:
	rm -f *.o

debug: CFLAGS += -g -Og
debug: clean $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o a.out

install: CFLAGS += -O3
install: clean all
	mkdir -p -- $(DESTDIR)$(PREFIX)
	cp -- $(OUT) $(DESTDIR)$(PREFIX)/bin
	chmod 755 -- $(DESTDIR)$(PREFIX)/bin/$(OUT)
	mkdir -p -- $(DESTDIR)$(MANPREFIX)
	sed "s/VERSION/$(VERSION)/g" < mousemode.1 > $(DESTDIR)$(MANPREFIX)/man1/mousemode.1
	chmod 755 -- $(DESTDIR)$(MANPREFIX)/man1/mousemode.1

uninstall:
	rm -f -- $(DESTDIR)$(PREFIX)/bin/$(OUT)
	rm -f -- $(DESTDIR)$(MANPREFIX)/man1/mousemode.1
