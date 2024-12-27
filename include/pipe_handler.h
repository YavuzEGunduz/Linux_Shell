//B221210080 ÇAĞRI TÜKENMEZ
//B221210038 DENİZ UMUR
//B231210350 MELİH DEDEOĞLU
//B221210071 YAVUZ EMRE GÜNDÜZ
//B211210015 BUĞRA BASTABAN



#ifndef PIPE_H
#define PIPE_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_ARGS 100




// Pipe ile birden fazla komutu çalıştır
void handle_pipe(char *command);

#endif // PIPE_H
