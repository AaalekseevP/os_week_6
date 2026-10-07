#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_LINE 1024
#define MAX_ARGS 64

int main() {
    char line[MAX_LINE];
    char *args[MAX_ARGS];

    while (1) {
        printf("shell> ");
        fflush(stdout);

        if (fgets(line, sizeof(line), stdin) == NULL)
            break;

        line[strcspn(line, "\n")] = '\0';

        if (strlen(line) == 0)
            continue;

        if (strcmp(line, "exit") == 0)
            break;

        // Divide into arguments
        int arg_count = 0;
        char *token = strtok(line, " ");
        while (token != NULL && arg_count < MAX_ARGS - 1) {
            args[arg_count++] = token;
            token = strtok(NULL, " ");
        }
        args[arg_count] = NULL;

        // Background process creation
        pid_t pid = fork();

        if (pid < 0) {
            perror("Fork failed");
        } else if (pid == 0) {
            // Child process
            char path[512];
            if (args[0][0] == '/' || args[0][0] == '.') {
                strcpy(path, args[0]);
            } else {
                snprintf(path, sizeof(path), "/bin/%s", args[0]);
            }

            char *envp[] = { NULL };
            if (execve(path, args, envp) == -1) {
                snprintf(path, sizeof(path), "/usr/bin/%s", args[0]);
                if (execve(path, args, envp) == -1) {
                    perror("Execution failed");
                    exit(EXIT_FAILURE);
                }
            }
        } else {
            // Parent process waiting to finish
            wait(NULL);
        }
    }

    return 0;
}
