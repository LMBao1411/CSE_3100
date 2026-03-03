#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// COMPILE exec.c ex1.c ex2.c before running ./exec

int main() {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }

    if (pid == 0) {
        // CHILD runs the program ex1
        execl("./ex1", "ex1", NULL);
        perror("exec child failed");
        exit(1);
    }
    else {
        // PARENT runs the program ex2
        execl("./ex2", "ex2", NULL);
        perror("exec parent failed");
        exit(1);
    }
    return 0;
}