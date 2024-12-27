//B221210080 ÇAĞRI TÜKENMEZ
//B221210038 DENİZ UMUR
//B231210350 MELİH DEDEOĞLU
//B221210071 YAVUZ EMRE GÜNDÜZ
//B211210015 BUĞRA BASTABAN
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include "shell.h"

void execute_pipe(char *commands[], int cmd_count, int background) {
    int i;
    int pipe_fd[2 * (cmd_count - 1)]; // Array to hold file descriptors for pipes


    



    // Create pipes
    for (i = 0; i < cmd_count - 1; i++) {
        if (pipe(pipe_fd + 2 * i) < 0) {
            perror("pipe failed");
            exit(EXIT_FAILURE);
        }
    }

    // Fork processes for each command
    for (i = 0; i < cmd_count; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            // Child process
            char *args[MAX_ARGS];
            char *token = strtok(commands[i], " ");
            int arg_count = 0;

            // Tokenize the current command into arguments
            while (token != NULL && arg_count < MAX_ARGS) {
                args[arg_count++] = token;
                token = strtok(NULL, " ");
            }
            args[arg_count] = NULL;

            // Handle piping between commands
            if (i > 0) { // Not the first command
                if (dup2(pipe_fd[2 * (i - 1)], STDIN_FILENO) < 0) {
                    perror("dup2 failed for input");
                    exit(EXIT_FAILURE);
                }
            }

            if (i < cmd_count - 1) { // Not the last command
                if (dup2(pipe_fd[2 * i + 1], STDOUT_FILENO) < 0) {
                    perror("dup2 failed for output");
                    exit(EXIT_FAILURE);
                }
            }

            // Close all pipe file descriptors in the child process
            for (int j = 0; j < 2 * (cmd_count - 1); j++) {
                close(pipe_fd[j]);
            }

            // Execute the command
            if (execvp(args[0], args) == -1) {
                fprintf(stderr, "Failed to execute command: %s\n", args[0]);
                perror("execvp failed");
                exit(EXIT_FAILURE);
            }
        } else if (pid < 0) {
            // Fork failed
            perror("fork failed");
            exit(EXIT_FAILURE);
        }
    }

    // Parent process
    // Close all pipe file descriptors
    for (i = 0; i < 2 * (cmd_count - 1); i++) {
        close(pipe_fd[i]);
    }

    // Wait for all child processes to finish if not in the background
    if (!background) {
        for (i = 0; i < cmd_count; i++) {
            wait(NULL);
        }
    }
}