#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <signal.h>
#include <time.h>
#include <parser.h>

// executor.c needs to return the output from the test + did the test exited successfully/crashed/timedout/seg fault/recursion/etc.

typedef struct {
    char test_output[4096];
    int exit_status;
    int crashed;
    int timed_out;
    int signal;
} Test_result;


// ----- HELPER FUNCTION ----- //

// Error handling function
void error_handler(int status, Test_result *result) {
    if (WIFEXITED(status)) {                        // Process exited normally via exit()
        result->exit_status = WEXITSTATUS(status);

        if (result->exit_status != 0) {             // Exited with non-zero = error
            result->crashed = 1;
        }
    }

    else if (WIFSIGNALED(status)) {                 // Process was killed by a signal (crash)
        result->crashed = 1;
        result->signal = WTERMSIG(status);          // Store which signal that crashed the code

        if (result->signal == SIGSEGV) {
            snprintf(result->test_output, sizeof(result->test_output), "seg fault/stack overflow\n");
        } 
        else if (result->signal == SIGABRT) {
            snprintf(result->test_output, sizeof(result->test_output), "process aborted\n");
        } 
        else if (result->signal == SIGFPE) {
            snprintf(result->test_output, sizeof(result->test_output), "floating point error\n");
        } 
        else if (result->signal == SIGKILL) {
            result->timed_out = 1;
            snprintf(result->test_output, sizeof(result->test_output), "process killed via timed out\n");
        } 
        else {
            snprintf(result->test_output, sizeof(result->test_output), "killed by signal %d\n", result->signal);
        }
    }
}

// MAIN EXECUTOR FUNCTION
// parameters: test struct & return a result struct
Test_result execute_test(Test *test) {
    Test_result result;
    memset(&result, 0, sizeof(Test_result));

    int pipe_stdin[2];
    int pipe_stdout[2];
    pipe(pipe_stdin);
    pipe(pipe_stdout);
    if (pipe(pipe_stdin) < 0 || pipe(pipe_stdout) < 0) {
        perror("pipe() failed");
        result.crashed = 1;
        return result;
    }  

    // Stdin Pipe: Parent writes -> Child reads (Parent will use pipe_stdin[1] to write and child will use pipe_stdin[0])
    // Stdout Pipe: Child writes -> Parent reads (Child will use pipe_stdout[1] to write and parent will use pipe_std[0] to read)

    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork Failed");
        result.crashed = 1;
        return result;
    }

    if (pid == 0) {        // CHILD    
        dup2(pipe_stdin[0], 0);
        dup2(pipe_stdout[1], 1);
        dup2(pipe_stdout[1], 2);    // capture error

        close(pipe_stdin[0]);
        close(pipe_stdin[1]);
        close(pipe_stdout[0]);
        close(pipe_stdout[1]);
        
        struct rlimit mem_limit;                    // Memory limit = 256MB
        mem_limit.rlim_cur = 256 * 1024 * 1024;
        mem_limit.rlim_max = 256 * 1024 * 1024;
        setrlimit(RLIMIT_AS, &mem_limit);

        char *args[] = {test->command, test->parameters, NULL};
        execvp(test->command, args);
        perror("exevcp failed");
        exit(EXIT_FAILURE);
    }

    else if (pid > 0) {             // PARENT
        close(pipe_stdin[0]);
        close(pipe_stdout[1]);

        if (strlen(test->parameters) > 0) {
            write(pipe_stdin[1], test->parameters, strlen(test->parameters));
        }
        close(pipe_stdin[1]);

        // Read child's output
        int bytes_read;
        int total_read = 0;
        char buffer[1024];
        while ((bytes_read = read(pipe_stdout[0], buffer, sizeof(buffer) - 1)) > 0) {
            if (total_read + bytes_read < sizeof(result.test_output)) {
                strncat(result.test_output, buffer, bytes_read);
                total_read += bytes_read;
            }
        }
        close(pipe_stdout[0]);

        // Wait for child process and deal with status edge cases
        int status;
        waitpid(pid, &status, 0);
        error_handler(status, &result);

    }
    return result;
}

//DEBUGGING
// at the bottom of executor.c

#ifdef TEST_EXECUTOR
int main() {
    Test t;
    Test_result r;

    // ---- TEST 1: ls (no stdin) ----
    printf("=== TEST 1: ls ===\n");
    memset(&t, 0, sizeof(Test));
    strncpy(t.command, "ls", sizeof(t.command) - 1);
    strncpy(t.parameters, "", sizeof(t.parameters) - 1);
    r = execute_test(&t);
    printf("  crashed: %d | exit: %d | output:\n%s\n", r.crashed, r.exit_status, r.test_output);

    // ---- TEST 2: echo with args ----
    printf("=== TEST 2: echo ===\n");
    memset(&t, 0, sizeof(Test));
    strncpy(t.command, "echo", sizeof(t.command) - 1);
    strncpy(t.parameters, "hello world", sizeof(t.parameters) - 1);
    r = execute_test(&t);
    printf("  crashed: %d | exit: %d | output:\n%s\n", r.crashed, r.exit_status, r.test_output);

    // ---- TEST 3: segfault ----
    // compile first: echo 'int main(){int*p=0;*p=1;}' > crash.c && gcc crash.c -o crash
    printf("=== TEST 3: segfault ===\n");
    memset(&t, 0, sizeof(Test));
    strncpy(t.command, "./crash", sizeof(t.command) - 1);
    strncpy(t.parameters, "", sizeof(t.parameters) - 1);
    r = execute_test(&t);
    printf("  crashed: %d | signal: %d | output:\n%s\n", r.crashed, r.signal, r.test_output);

    return 0;
}
#endif