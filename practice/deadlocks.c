#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#define MAX 1000000
int main() {
    int pd[2];
    assert(pipe(pd) != -1);
    pid_t pid = fork();
    if (pid == 0) {
        for (int i = 0; i <MAX; i++){
            write(pd[1], &i, sizeof(i));
        }
        close(pd[1]);
        return 0;
    }
    close(pd[1]);
    waitpid(pid, NULL, 0);
    int count = 0, k;
    for (int i=0; i<MAX; i++) {
        if (read(pd[0], &k, sizeof(int))) count++;
    }
    close(pd[0]);
    printf("count = %d\n", count);
    //waitpid(pid, NULL, 0) NEEDS TO BE HERE
    return 0;
    /*
    The program deadlocks because the parent calls waitpid() before reading from the pipe. 
    The child fills the pipe buffer and blocks on write(), 
    while the parent waits for the child to terminate, so neither can proceed.
    */
}