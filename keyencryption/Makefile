CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic -g $(shell pkg-config --cflags gmp)
MAPLE =  randstate.o numtheory.o rsa.o
DAISY = --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose
LFLAGS = $(shell pkg-config --libs gmp)
.PHONY: all scan-build valgrind

all: keygen encrypt decrypt

keygen: $(MAPLE) keygen.o
	$(CC) -o keygen $(MAPLE) keygen.o $(LFLAGS)

keygen.o: keygen.c
	$(CC) $(CFLAGS) -c keygen.c

encrypt: $(MAPLE) encrypt.o
	$(CC) -o encrypt $(MAPLE) encrypt.o $(LFLAGS)

encrypt.o: encrypt.c
	$(CC) $(CFLAGS) -c encrypt.c

decrypt: $(MAPLE) decrypt.o
	 $(CC) -o decrypt $(MAPLE) decrypt.o $(LFLAGS)

decrypt.o: decrypt.c
	$(CC) $(CFLAGS) -c decrypt.c

clean:
	rm -f *.o keygen encrypt decrypt

scan-build: clean
	scan-build --use-cc=$(CC) make

valgrind: decrypt
	valgrind $(DAISY) ./decrypt

format:
	clang-format -i -style=file *.[ch]
