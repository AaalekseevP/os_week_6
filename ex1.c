#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

void execute_task(const char *process_name) {
    clock_t start = clock();

    int sum = 0;
    for (int i = 0; i < 1000000; i++) {
        sum += i;
    }

    clock_t end = clock();
    double exec_time = ((double) (end - start)) / CLOCKS_PER_SEC * 1000.0;

    printf("%s:\n ID: %d, Parent ID: %d, Execution time: %.3f ms\n\n",
           process_name, getpid(), getppid(), exec_time);
}

int main() {
    pid_t pid1, pid2;

    pid1 = fork();
    if (pid1 == 0) {
        execute_task("Child 1");
        exit(0);
    }

    pid2 = fork();
    if (pid2 == 0) {
        execute_task("Child 2");
        exit(0);
    }

    wait(NULL);
    wait(NULL);

    execute_task("Main Process");

    return 0;
}