.PHONY: clean

CC=gcc
CFLAGS=-O3 -Wall -Werror -pedantic

primes: primes.c

clean:
	rm -f primes
