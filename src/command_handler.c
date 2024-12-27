//B221210080 ÇAĞRI TÜKENMEZ
//B221210038 DENİZ UMUR
//B231210350 MELİH DEDEOĞLU
//B221210071 YAVUZ EMRE GÜNDÜZ
//B211210015 BUĞRA BASTABAN


#include "command_handler.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>


void execute_single_command(char *command, int background) {
    char *args[MAX_ARGS];
    char *input_file = NULL, *output_file = NULL;
    int arg_count = 0;
    int increment = 0;
    char *token = strtok(command, " ");
    
    while (token != NULL) {
        if(strcmp(token, "increment") == 0){
            increment = 1;
        }else if (strcmp(token, "<") == 0) {
            token = strtok(NULL, " ");
            if (token) input_file = token;
        } else if (strcmp(token, ">") == 0) {
            token = strtok(NULL, " ");
            if (token) output_file = token;
        } else {
            args[arg_count++] = token;
        }
        token = strtok(NULL, " ");
    }
    args[arg_count] = NULL;

    pid_t pid = fork();
    if (pid == 0) { // Child process
        if (input_file) {
            //dosyadan veri okuma
            int fd = open(input_file, O_RDONLY);
            if (fd < 0) {
                printf("%s giriş dosyasi bulunamadi.\n",input_file);
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);

            if(increment == 1){
                char buffer[16];//yazılan değerin int olduğunu varsayarak 32 bit verdim.
                if (fgets(buffer, sizeof(buffer), stdin) == NULL) {//dosyadaki veriyi genel inputa yönlendirir.
                    printf("Dosyadan okuma hatası.\n");
                    exit(EXIT_FAILURE);
                }
                int number = atoi(buffer); // Sayıya çevir
                number++; // Değeri artır
                printf("%d \n",number);
                fflush(stdout);
            }
        }
        if (output_file) {
            int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                printf("%s giriş dosyasi bulunamadi.\n",output_file);
                exit(EXIT_FAILURE);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        execvp(args[0], args);
        perror("Command execution error");
        exit(EXIT_FAILURE);
    } else if (pid > 0) { // Parent process 
        if (background) {
            //arka planda çalışma kısmı
        } else {
            waitpid(pid, NULL, 0);
        }
    } else {
        perror("Fork failed");
    }
}
