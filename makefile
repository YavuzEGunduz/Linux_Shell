.PHONY: all clean

all: bin/shell.out

bin/shell.out: lib/shell.o 
	@mkdir -p bin
	gcc -Iinclude -o bin/shell.out 

lib/shell.o: src/shell.c
	@mkdir -p lib
	gcc -Iinclude -c src/shell.c -o lib/shell.o

clean:
	rm -f lib/*.o bin/shell.out