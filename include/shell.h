#ifndef SHELL_H
#define SHELL_H

#define MAX_INPUT_SIZE 1024
#define MAX_ARGS 100
#define MAX_CMDS 10
#define MAX_BG_PROCESSES 100

void execute_pipe(char *commands[], int cmd_count, int background);

#endif
