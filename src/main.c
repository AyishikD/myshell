#include <stdio.h>
#include <pwd.h>
#include <sys/types.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "myshell.h"
#define ORANGE "\033[38;5;208m"
#define RESET "\033[0m"

void myshell_loop() {
    char *input;
    char *args[MAX_ARGS];
    char cwd[1024];
    char prompt[2048];

    while (1) {
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            struct passwd *pw = getpwuid(getuid());
            char hostname[HOST_NAME_MAX + 1];
            gethostname(hostname, sizeof(hostname));
            snprintf(prompt, sizeof(prompt), ORANGE "\033[1;36m%s@%s\033[0m:\033[1;32m%s" RESET "$ ", pw->pw_name, hostname, cwd);
        } else {
            perror("getcwd() error");
            continue;
        }
        input = readline(prompt);

        if (input == NULL) {
            printf("\n");
            break;
        }
        if (*input != '\0') add_history(input);
        int i = 0;
        char *token = strtok(input, " ");
        while (token && i < MAX_ARGS - 1) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL;

        if (args[0] != NULL) {
            if (handle_builtin(args) == 0) {
                execute_command(args);
            }
        }

        free(input);
    }
}
int main() {
    myshell_loop();
    return 0;
}