CC=gcc
CFLAGS=-Wall -Werror -Wextra

all: clean test
 
rebuild: clean sprintf

test:
	$(CC) $(CFLAGS) testprintf.c -o test

sprintf: sprintf.o  common_modules.o
	$(CC) $(CFLAGS) sprintf.o  common_modules.o -o sprintf

sprintf.o:
	$(CC) $(CFLAGS) -c sprintf.c -o sprintf.o

common_modules.o:
	$(CC) $(CFLAGS) -c common_modules.c -o common_modules.o

clean:
	rm -rf *.o sprintf test