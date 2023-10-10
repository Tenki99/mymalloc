CC=gcc
CFLAGS=-O3 -fPIC
MALLOC_VERSION=BF

small_range_rand_allocs: small_range_rand_allocs.c my_malloc.c my_malloc.h 
	$(CC) $(CFLAGS) -D$(MALLOC_VERSION) -o $@ small_range_rand_allocs.c my_malloc.c my_malloc.h -g

clean:
	rm -f *~ *.o  small_range_rand_allocs 

clobber:
	rm -f *~ *.o
