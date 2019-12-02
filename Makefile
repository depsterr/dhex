CC=gcc
PROJECTDIR=build/dhex.out
INSTALLDIR=/bin/dhex
CFLAGS= -g
INSTALLFLAGS=

default: src/dhex.c
	$(CC) -o $(PROJECTDIR) $^ $(CFLAGS)

install: src/dhex.c
	$(CC) -o $(INSTALLDIR) $^ $(INSTALLFLAGS)
	make clean

clean:
	rm -f *.o

run:
	make default
	./$(PROJECTDIR)

rc:
	make run clean
