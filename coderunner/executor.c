#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <signal.h>
#include <time.h>
#include "parser.h"

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
// using signal.h (SIGEGV, SIGABRT, SIGFPE, SGKILL)
void error_handler(int status, Test_result *result) {
    if (result->timed_out) return; // Handled in the main loop

    if (WIFEXITED(status)) {
        result->exit_status = WEXITSTATUS(status);
        if (result->exit_status != 0) {
            result->crashed = 1;
        }
    }
    else if (WIFSIGNALED(status)) {
        result->crashed = 1;
        result->signal = WTERMSIG(status);
        
        // Append error to any existing partial output and not overwrite it
        char error_msg[128];
        if (result->signal == SIGSEGV) {
            snprintf(error_msg, sizeof(error_msg), "seg fault/stack overflow\n");
        }
        else if (result->signal == SIGABRT) {
            snprintf(error_msg, sizeof(error_msg), "process aborted\n");
        }
        else if (result->signal == SIGFPE) {
            snprintf(error_msg, sizeof(error_msg), "floating point error\n");
        }
        else {
            snprintf(error_msg, sizeof(error_msg), "killed by signal %d\n", result->signal);
        }
        strncat(result->test_output, error_msg, sizeof(result->test_output) - strlen(result->test_output) - 1);
    }
}

// MAIN EXECUTOR FUNCTION
// parameters: test struct & return a result struct
Test_result execute_test(Test *test) {
    Test_result result;
    memset(&result, 0, sizeof(Test_result));

    int pipe_stdin[2];
    int pipe_stdout[2];
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

    if (pid == 0) {                 // CHILD    
        dup2(pipe_stdin[0], 0);
        dup2(pipe_stdout[1], 1);
        dup2(pipe_stdout[1], 2);    // capture error

        close(pipe_stdin[0]);       // close all unused pipes
        close(pipe_stdin[1]);
        close(pipe_stdout[0]);
        close(pipe_stdout[1]);
        
        struct rlimit mem_limit;    // memory limit = 256MB
        mem_limit.rlim_cur = 256 * 1024 * 1024;
        mem_limit.rlim_max = 256 * 1024 * 1024;
        setrlimit(RLIMIT_AS, &mem_limit);

        // tokenizing arguments to pass into execvp 
        char *args[64];
        int arg_count = 0;
        args[arg_count++] = test->command;

        char temp_args[1024];
        strncpy(temp_args, test->args_str, sizeof(temp_args));
        temp_args[sizeof(temp_args) - 1] = '\0';

        char *token = strtok(temp_args, " ");
        while (token != NULL && arg_count < 63) {
            args[arg_count++] = token;
            token = strtok(NULL, " ");
        }
        args[arg_count] = NULL;

        execvp(test->command, args);
        perror("execvp failed");
        exit(EXIT_FAILURE);
    }

    else if (pid > 0) {         // PARENT
        close(pipe_stdin[0]);
        close(pipe_stdout[1]);

        if (strlen(test->stdin_input) > 0) {
            write(pipe_stdin[1], test->stdin_input, strlen(test->stdin_input));
        }
        close(pipe_stdin[1]);
  
        int bytes_read;
        int total_read = 0;
        char buffer[1024];
        time_t start_time = time(NULL);
        int timeout_secs = 5; // 5 second timeout
        int status;
        pid_t wpid;
        int process_alive = 1;  // child is still running?

        // Implement active polling to prevent infinite waiting
        /* WNOHANG prevents wait()/waitpid() from blocking so that the process can go on with other tasks. 
        If a child died, its pid will be returned by wait()/waitpid() and the process can act on that. If nothing died, then the returned pid is 0 */
        while (1) {
            if (process_alive) {
                wpid = waitpid(pid, &status, WNOHANG);
                if (wpid != 0) {
                    process_alive = 0;  // exited naturally
                } 
                else if (time(NULL) - start_time > timeout_secs) {
                    kill(pid, SIGKILL);
                    result.timed_out = 1;
                    result.crashed = 1;
                    strncat(result.test_output, "\n[TIMEOUT] process killed (infinite loop)\n", 
                            sizeof(result.test_output) - strlen(result.test_output) - 1);
                    waitpid(pid, &status, 0);      // collect the zombie child
                    process_alive = 0;
                }
            }

            // read output from child
            bytes_read = read(pipe_stdout[0], buffer, sizeof(buffer));
            if (bytes_read > 0) {
                if (total_read + bytes_read < sizeof(result.test_output) - 1) {
                    memcpy(result.test_output + total_read, buffer, bytes_read);
                    total_read += bytes_read;
                    result.test_output[total_read] = '\0';
                }
            } 
            else if (!process_alive) {
                break; 
            }
        }
        close(pipe_stdout[0]);

        if (!result.timed_out) {
            error_handler(status, &result);
        }
    }
    return result;
}

// DEBUGGING
#ifdef DEBUG_EXECUTOR
int main() {
    Test tests[10];
    int count = 0;
    
    parse_test("test_input", tests, &count);
    for (int i = 0; i < count; i++) {
        printf("---Running %s---\n", tests[i].name);
        Test_result r = execute_test(&tests[i]);
        printf("Output: %s\n", r.test_output);
        printf("Crashed: %d, Timeout: %d, Exit Code: %d\n", r.crashed, r.timed_out, r.exit_status);
    }
    return 0;
}
#endif