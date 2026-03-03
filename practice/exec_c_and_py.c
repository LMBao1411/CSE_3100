#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// COMPILE exec.c ex1.c ex2.c before running ./exec

int main() {
    int pipefd[2];
    pid_t pid;

    // 1. Create a pipe
    if (pipe(pipefd) == -1) {
        perror("pipe failed");
        exit(1);
    }

    // 2. Fork
    pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }

    // 3. CHILD PROCESS
    if (pid == 0) {
        close(pipefd[0]);       // close read end
        
        //Redirect stdout to pipe
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        // Run python script
        execlp("python3", "python3", "py_quadratic.py", (char *)NULL);
        perror("exec failed");
        exit(1);
    }
    
    // 4. PARENT PROCESS
    else {
        close(pipefd[1]);
        
        char buffer[100];
        // Read the output and null-terminate it
        ssize_t bytesRead = read(pipefd[0], buffer, sizeof(buffer) - 1);
        if (bytesRead > 0) {
            buffer[bytesRead] = '\0'; 
        }
        close(pipefd[0]);
        
        wait(NULL); 

        int value1, value2;
        // Parse the two integers from the Python output
        if (sscanf(buffer, "%d %d", &value1, &value2) == 2) {
            printf("Value 1: %d, Value 2: %d\n", value1, value2);

            // Corrected loops: printf requires a format string
            printf("Loop 1:\n");
            for (int i = 1; i <= value1; i++) {
                printf("%d ", i);
                // i++ inside the loop makes it skip numbers; 
                // remove it if you want to count every number.
            }
            
            printf("\nLoop 2:\n");
            for (int i = 1; i <= value2; i++) {
                printf("%d ", i);
            }
            printf("\n");
        } else {
            printf("Failed to parse values from Python script.\n");
        }
    }
    return 0;
}
