CC=gcc
CFLAGS=-O2 -v

decomment:
	$(CC) -o decomment decomment.c $(CFLAGS)

.PHONY: recursive_decomment.sh COPYRIGHT decomment.c

clean:
	rm -f *.o decomment
