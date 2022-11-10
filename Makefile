CC=gcc
CFLAGS=-Wall -Werror -Wextra

all: clean test s21_sprintf
 
rebuild: clean s21_sprintf test

test:
	$(CC) $(CFLAGS) testprintf.c -o test

s21_sprintf: s21_sprintf.o  
	$(CC) $(CFLAGS) s21_sprintf.o   -o s21_sprintf

s21_sprintf.o:
	$(CC) $(CFLAGS) -c s21_sprintf.c -o s21_sprintf.o

clean:
	rm -rf *.o s21_sprintf test