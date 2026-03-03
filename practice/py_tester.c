#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <assert.h>

// The function we are testing
int add(int a, int b) {
    return a + b;
}

int main() {
    int pipefd[2];              // pipefd[0] is read, pipefd[1] is write
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(EXIT_FAILURE);
    }

    // CHILD PROCESS
    if (pid == 0) {
        // 1. Close the read end (we only write)
        close(pipefd[0]);

       // 2. Redirect the STDOUT to the pipe not the terminal
       dup2(pipefd[1], STDOUT_FILENO);
       close(pipefd[1]);

       execlp("python3", "python3", "py_quadratic.py", NULL);
       perror("execlp");
       exit(1);
    }
    // PARENT PROCESS
    else {
        // 1. Close the write end (we only read)
        close(pipefd[1]);

        char buffer[100];
        int r1, r2;
        int expected1 = 6; int expected2 = 7;
        int expected_sum = 13;
        FILE *stream = fdopen(pipefd[0], "r");
        if (fgets(buffer, sizeof(buffer), stream) != NULL) {
            if (sscanf(buffer, "%d %d", &r1, &r2) == 2) {
                printf("Testing Python Output: %d and %d\n", r1, r2);
                int sum = add(r1, r2); 
                printf("Testing SUM from python output: %d \n", sum);
                // Logic check: order of the roots does not matter
                if (((r1 == expected1) && (r2 == expected2) && (sum == expected_sum)) || 
                ((r1 == expected2) && (r2 == expected1) && (sum == expected_sum))) {

                    printf("[RESULT] PASS: Roots and sum of roots match expected values.\n");
                } 
                else {
                    printf("[RESULT] FAIL: Expected %d and %d, but got %d and %d.\n", 
                            expected1, expected2, r1, r2);
                }
            }
        }
        fclose(stream);
        wait(NULL);     // Cleanup the child process
    }
    return 0;
}