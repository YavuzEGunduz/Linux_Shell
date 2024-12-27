//B221210080 ÇAĞRI TÜKENMEZ
//B221210038 DENİZ UMUR
//B231210350 MELİH DEDEOĞLU
//B221210071 YAVUZ EMRE GÜNDÜZ
//B211210015 BUĞRA BASTABAN


#include "pipe_handler.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

void handle_pipe(char *cmd) {
    char *commands[2];
    int pipefd[2];

    commands[0] = strtok(cmd, "|");
    commands[1] = strtok(NULL, "|");

    if (commands[1] == NULL) {
        printf("Invalid pipe syntax\n");
        return;
    }

    pipe(pipefd);
    pid_t pid1 = fork();

    if (pid1 == 0) { // First child
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);
        char *args[100];
        int i = 0;
        args[i] = strtok(commands[0], " ");
        while (args[i] != NULL) {
            i++;
            args[i] = strtok(NULL, " ");
        }
        execvp(args[0], args);
        perror("execvp failed");
        exit(1);
    }

    pid_t pid2 = fork();

    if (pid2 == 0) { // Second child
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[1]);
        close(pipefd[0]);
        char *args[100];
        int i = 0;
        args[i] = strtok(commands[1], " ");
        while (args[i] != NULL) {
            i++;
            args[i] = strtok(NULL, " ");
        }
        execvp(args[0], args);
        perror("execvp failed");
        exit(1);
    }

    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}


void execute_piped_commands(char *commands[], int cmd_count, char *input_file, char *output_file) {
    int pipe_fd[2 * (cmd_count - 1)]; // Create file descriptors for pipes

    // Create pipes
    for (int i = 0; i < cmd_count - 1; i++) {
        if (pipe(pipe_fd + 2 * i) < 0) {
            perror("pipe failed");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < cmd_count; i++) {
        pid_t pid = fork();

        if (pid == 0) { // Child process
            // Redirect input from the previous pipe or input file
            if (i == 0 && input_file) {
                int fd = open(input_file, O_RDONLY);
                if (fd < 0) {
                    perror("Input file open failed");
                    exit(EXIT_FAILURE);
                }
                dup2(fd, STDIN_FILENO);
                close(fd);
            } else if (i > 0) {
                dup2(pipe_fd[2 * (i - 1)], STDIN_FILENO);
            }

            // Redirect output to the next pipe or output file
            if (i == cmd_count - 1 && output_file) {
                int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd < 0) {
                    perror("Output file open failed");
                    exit(EXIT_FAILURE);
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            } else if (i < cmd_count - 1) {
                dup2(pipe_fd[2 * i + 1], STDOUT_FILENO);
            }

            // Close all pipe file descriptors
            for (int j = 0; j < 2 * (cmd_count - 1); j++) {
                close(pipe_fd[j]);
            }

            // Parse command arguments
            char *args[MAX_ARGS];
            char *token = strtok(commands[i], " ");
            int arg_count = 0;

            while (token != NULL) {
                args[arg_count++] = token;
                token = strtok(NULL, " ");
            }
            args[arg_count] = NULL;

            // Execute the command
            execvp(args[0], args);
            perror("execvp failed");
            exit(EXIT_FAILURE);
        } else if (pid < 0) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }
    }

    // Parent process
    // Close all pipe file descriptors
    for (int i = 0; i < 2 * (cmd_count - 1); i++) {
        close(pipe_fd[i]);
    }

    // Wait for all child processes to finish
    for (int i = 0; i < cmd_count; i++) {
        wait(NULL);
    }
}

void execute_sequential_commands(char *commands[], int cmd_count) {
    for (int i = 0; i < cmd_count; i++) {
        pid_t pid = fork();

        if (pid == 0) { // Child process
            char *args[MAX_ARGS];
            char *token = strtok(commands[i], " ");
            int arg_count = 0;

            while (token != NULL) {
                args[arg_count++] = token;
                token = strtok(NULL, " ");
            }
            args[arg_count] = NULL;

            execvp(args[0], args);
            perror("execvp failed");
            exit(EXIT_FAILURE);
        } else if (pid > 0) {
            waitpid(pid, NULL, 0); // Wait for the current command to finish
        } else {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }
    }
}

void execute_background_command(char *command) {
    pid_t pid = fork();

    if (pid == 0) { // Child process
        char *args[MAX_ARGS];
        char *token = strtok(command, " ");
        int arg_count = 0;

        while (token != NULL) {
            args[arg_count++] = token;
            token = strtok(NULL, " ");
        }
        args[arg_count] = NULL;

        execvp(args[0], args);
        perror("execvp failed");
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
    // Parent process does not wait for the child process
}
