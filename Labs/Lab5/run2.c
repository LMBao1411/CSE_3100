#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char ** argv)
{
    // at least, there should be 3 arguments
    // 2 for the first command, and the rest for the second command

    // first command is argv[1] and its argument is argv[2]
    // second command is argv[3] and the rest of arguments if any are arguments to argv[3]
    if (argc < 4) {
        fprintf(stderr, "Usage: %s cmd1 cmd1_arg cmd2 [cmd2_args ..]\n", argv[0]);
        return 1;
    }

    // TODO
    // You should use execlp to run the first command, and execvp to run the second command
    pid_t child;
    int exitStatus;

    // first command, first fork for the first child
    child = fork();
    if (child < 0) {
        perror("fork() failed");
        exit(1);
    }
    if (child == 0) {
        execlp(argv[1], argv[1], argv[2], NULL);
        perror("execlp() failed");
    }
    else {
        child = waitpid(child, &exitStatus, 0);
        printf("exited=%d exitstatus=%d\n", WIFEXITED(exitStatus), WEXITSTATUS(exitStatus));
    }
    
    // second command, second fork for the second child
    child = fork();
    if (child < 0) {
        perror("fork() failed");
        exit(1);
    }
    if (child == 0) {
        execvp(argv[3], &argv[3]);
        perror("execvp() failed");
    }
    else {
        waitpid(child,NULL,0);
        printf("exited=%d exitstatus=%d\n", WIFEXITED(exitStatus), WEXITSTATUS(exitStatus));
    }
    
    return 0;
}
