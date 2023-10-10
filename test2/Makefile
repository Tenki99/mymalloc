CC=gcc
CFLAGS=-O3 -fPIC
MALLOC_VERSION=BF

all: mymalloc_test

mymalloc_test: mymalloc_test.c my_malloc.c my_malloc.h
	$(CC) $(CFLAGS) -D$(MALLOC_VERSION) -o $@ mymalloc_test.c my_malloc.c my_malloc.h -g

clean:
	rm -f *~ *.o mymalloc_test

clobber:
	rm -f *~ *.o
