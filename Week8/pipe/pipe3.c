#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {

    int pipe1[2], pipe2[2];
    pipe(pipe1);
    pipe(pipe2);

    if (fork() == 0) { // cmd1
        dup2(pipe1[1], 1);

        close(pipe1[0]);
        close(pipe2[0]);
        close(pipe2[1]);

        execlp("cat", "cat", "pipe.txt", NULL);
        perror("exec1");
        exit(1);
    }

    if (fork() == 0) { // cmd2
        dup2(pipe1[0], 0);
        dup2(pipe2[1], 1);

        close(pipe1[1]);
        close(pipe2[0]);

        execlp("sort", "sort", NULL);
        perror("exec2");
        exit(1);
    }

    // cmd3
    dup2(pipe2[0], 0);

    close(pipe1[0]);
    close(pipe1[1]);
    close(pipe2[1]);

    execlp("uniq", "uniq", NULL);
    perror("exec3");

    return 0;
}