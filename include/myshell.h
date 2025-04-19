#ifndef MYSHELL_H
#define MYSHELL_H

#define MAX_COMMAND_LEN 1024
#define MAX_ARGS 64

void myshell_loop();
void execute_command(char **args);
int handle_builtin(char **args);

#endif