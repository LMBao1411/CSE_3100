#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_CMDS 10

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s cmd1 cmd2 ...\n", argv[0]);
        exit(1);
    }

    int num_cmds = argc - 1;
    int pipes[MAX_CMDS][2];
    for (int i = 0; i < num_cmds - 1; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(1);
        }
    }
    for (int i = 0; i < num_cmds; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork");
            exit(1);
        }

        if (pid == 0) {
            // ================= CHILD PROCESS =================

            // TODO: Redirect input (if not first command)
            if (i > 0) {
                // connect pipes[i-1][0] to stdin
                dup2(pipes[i-1][0], 0);
            }

            // TODO: Redirect output (if not last command)
            if (i < num_cmds - 1) {
                // connect pipes[i][1] to stdout
                dup2(pipes[i][1], 1);
            }

            // TODO: Close ALL pipe fds (very important!)
            for (int j = 0; j < num_cmds - 1; j++) {
                // close both ends of every pipe
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            // TODO : Execute command
            char *cmd = argv[i+1];
            char *args[] = {cmd, NULL};
            execvp(cmd, args);

            // If exec fails
            perror("execvp");
            exit(1);
        }
        // ================= PARENT CONTINUES LOOP =================
    }

    // TODO : Parent closes all pipe fds
    for (int i = 0; i < num_cmds - 1; i++) {
        // close both ends
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Wait for all children
    for (int i = 0; i < num_cmds; i++) {
        wait(NULL);
    }

    return 0;
}