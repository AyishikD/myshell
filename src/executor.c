#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#define MAX_ARGS 100

typedef struct Job {
    pid_t pid;
    char *command;
    struct Job *next;
} Job;


Job *job_list = NULL;
void handle_jobs(char **args); 

void execute_command(char **args) {
    int background = 0;
    FILE *input_file = NULL;
    FILE *output_file = NULL;

    if (args[0] == NULL) return;
    if (strcmp(args[0], "jobs") == 0 || strcmp(args[0], "fg") == 0 || strcmp(args[0], "kill") == 0) {
        handle_jobs(args);
        return;
    }

    int i = 0;
    while (args[i] != NULL) i++;
    if (i > 0 && strcmp(args[i - 1], "&") == 0) {
        background = 1;
        args[i - 1] = NULL;
    }

    char *commands[20][MAX_ARGS];
    int num_cmds = 0;
    int arg_idx = 0;
    for (i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], "|") == 0) {
            commands[num_cmds][arg_idx] = NULL;
            num_cmds++;
            arg_idx = 0;
        } else {
            commands[num_cmds][arg_idx++] = args[i];
        }
    }
    commands[num_cmds][arg_idx] = NULL;
    num_cmds++;

    for (i = 0; commands[num_cmds - 1][i] != NULL; i++) {
        if (strcmp(commands[num_cmds - 1][i], "<") == 0 && commands[num_cmds - 1][i + 1]) {
            input_file = fopen(commands[num_cmds - 1][i + 1], "r");
            if (!input_file) {
                perror("Input file error");
                return;
            }
            commands[num_cmds - 1][i] = NULL;
        } else if (strcmp(commands[num_cmds - 1][i], ">") == 0 && commands[num_cmds - 1][i + 1]) {
            output_file = fopen(commands[num_cmds - 1][i + 1], "w");
            if (!output_file) {
                perror("Output file error");
                return;
            }
            commands[num_cmds - 1][i] = NULL;
        } else if (strcmp(commands[num_cmds - 1][i], ">>") == 0 && commands[num_cmds - 1][i + 1]) {
            output_file = fopen(commands[num_cmds - 1][i + 1], "a");
            if (!output_file) {
                perror("Append file error");
                return;
            }
            commands[num_cmds - 1][i] = NULL;
        }
    }

    int pipe_fd[2 * (num_cmds - 1)];
    for (i = 0; i < num_cmds - 1; i++) {
        if (pipe(pipe_fd + i * 2) < 0) {
            perror("pipe");
            exit(1);
        }
    }

    for (i = 0; i < num_cmds; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            if (i > 0) {
                dup2(pipe_fd[(i - 1) * 2], 0);
            }
            if (i < num_cmds - 1) {
                dup2(pipe_fd[i * 2 + 1], 1);
            }

            if (i == 0 && input_file) {
                dup2(fileno(input_file), STDIN_FILENO);
            }

            if (i == num_cmds - 1 && output_file) {
                dup2(fileno(output_file), STDOUT_FILENO);
            }

            for (int j = 0; j < 2 * (num_cmds - 1); j++) {
                close(pipe_fd[j]);
            }

            execvp(commands[i][0], commands[i]);
            perror("execvp failed");
            exit(1);
        } else if (pid < 0) {
            perror("fork failed");
            return;
        } else {
            if (background && i == num_cmds - 1) {
                printf("[Running in background] PID: %d\n", pid);
                Job *new_job = malloc(sizeof(Job));
                new_job->pid = pid;
                new_job->command = strdup(commands[i][0]);
                new_job->next = job_list;
                job_list = new_job;
            }
        }
    }

    for (i = 0; i < 2 * (num_cmds - 1); i++) {
        close(pipe_fd[i]);
    }

    if (!background) {
        for (i = 0; i < num_cmds; i++) {
            wait(NULL);
        }
    }

    if (input_file) fclose(input_file);
    if (output_file) fclose(output_file);
}

void handle_jobs(char **args) {
    if (args[0] != NULL && strcmp(args[0], "jobs") == 0) {
        Job *curr = job_list;
        while (curr) {
            printf("[%d] %s\n", curr->pid, curr->command);
            curr = curr->next;
        }
    }
    if (args[0] != NULL && strcmp(args[0], "fg") == 0 && args[1] != NULL) {
        pid_t pid = atoi(args[1]);
        kill(pid, SIGCONT);  
        waitpid(pid, NULL, 0);  
        Job **prev = &job_list;
        while (*prev) {
            if ((*prev)->pid == pid) {
                Job *temp = *prev;
                *prev = (*prev)->next;
                free(temp->command);
                free(temp);
                break;
            }
            prev = &(*prev)->next;
        }
    }
    if (args[0] != NULL && strcmp(args[0], "kill") == 0 && args[1] != NULL) {
        pid_t pid = atoi(args[1]);
        kill(pid, SIGTERM);
        printf("Deleted job");  
    }
}