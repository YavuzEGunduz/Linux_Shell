#B221210080 ÇAĞRI TÜKENMEZ
#B221210038 DENİZ UMUR
#B231210350 MELİH DEDEOĞLU
#B221210071 YAVUZ EMRE GÜNDÜZ
#B211210015 BUĞRA BASTABAN


.PHONY: all clean

all: bin/shell.out

bin/shell.out: lib/main.o lib/shell.o lib/signal_handler.o lib/prompt.o lib/command_handler.o lib/pipe_handler.o
	@mkdir -p bin
	gcc -Iinclude -o bin/shell.out lib/main.o lib/shell.o lib/signal_handler.o lib/prompt.o lib/command_handler.o lib/pipe_handler.o

lib/main.o: src/main.c
	@mkdir -p lib
	gcc -Iinclude -c src/main.c -o lib/main.o

lib/shell.o: src/shell.c
	@mkdir -p lib
	gcc -Iinclude -c src/shell.c -o lib/shell.o

lib/signal_handler.o: src/signal_handler.c
	@mkdir -p lib
	gcc -Iinclude -c src/signal_handler.c -o lib/signal_handler.o

lib/prompt.o: src/prompt.c
	@mkdir -p lib
	gcc -Iinclude -c src/prompt.c -o lib/prompt.o

lib/command_handler.o: src/command_handler.c
	@mkdir -p lib
	gcc -Iinclude -c src/command_handler.c -o lib/command_handler.o

lib/pipe_handler.o: src/pipe_handler.c
	@mkdir -p lib
	gcc -Iinclude -c src/pipe_handler.c -o lib/pipe_handler.o

clean:
	rm -f lib/*.o bin/shell.out