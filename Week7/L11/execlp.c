#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    int a = fork();
    if (a == 0) {
        execlp("ls","ls", NULL);
        //name of file, how to run the file, NULL
    }
    else {
        wait(NULL);
        printf("Done with that file\n");
    }
    return 0;
}