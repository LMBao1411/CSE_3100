#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#define MAX 1024
int main() {
    int pd[2];
    assert(pipe(pd)!= -1);
    pid_t pid = fork();
    if (pid ==0) {
        close(pd[1]);
        dup2(pd[0], 0);
        close(pd[0]);
        char s[MAX];
        while (fgets(s, sizeof(s), stdin)) printf("%s", s);
        return 0;
    }
    close(pd[0]);
    char *msg = "hello world\ntoday is a great day\n";
    write(pd[1], msg, strlen(msg));
    close(pd[1]);
    waitpid(pid, NULL, 0);
    return 0;
}