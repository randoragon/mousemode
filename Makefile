CC=gcc
CFLAGS=-std=c99
LDFLAGS=-lX11 -lXtst
OBJS=mousemode.o
OUT=mousemode

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
	cp -- $(OUT) /usr/local/bin
