#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int agrc, char **argv) {
    if (agrc < 2) {
        fprintf(stderr, "Error\n");
    }

    int a = fork();
    if (a == 0) {
        // char *argv[] = {"./calc", "sum", "1", "2", "3", NULL};
        execv("./calc", &argv[1]);
        //path of file, the argv array
    }
    else {
        wait(NULL);
        printf("Done with that file\n");
    }
    return 0;
}