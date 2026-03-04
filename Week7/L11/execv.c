#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    int a = fork();
    if (a == 0) {
        char *args[] = {"./calc", "sum", "1", "2", "3", "4", NULL};
        execv("./calc", args);
        //path of file, the args
    }
    else {
        wait(NULL);
        printf("Done with that file\n");
    }
    return 0;
}