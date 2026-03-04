#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {

    int a = fork();
    if (a == 0) {
        char *args[] = {"ls", "-l", NULL};
        execvp(args[0], args);
        //path of file, the argv array
    }
    else {
        wait(NULL);
        printf("Done with that file\n");
    }
    return 0;
}