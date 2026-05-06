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
#include "executor.h"

// ----- HELPER ----- //

// Error handling function: using signal.h (SIGEGV, SIGABRT, SIGFPE, SGKILL)
// If it is not a timed_out signal from the child process, error_handler will be called
void error_handler(int status, Test_result *test_result) {
    if (test_result->timed_out) return;

    if (WIFEXITED(status)) {
        test_result->exit_status = WEXITSTATUS(status);
        if (test_result->exit_status != 0) {
            test_result->crashed = 1;
        }
    }
    else if (WIFSIGNALED(status)) {
        test_result->crashed = 1;
        test_result->signal = WTERMSIG(status);
        
        char error_msg[128];
        if (test_result->signal == SIGSEGV) {
            snprintf(error_msg, sizeof(error_msg), "seg fault/stack overflow\n");
        }
        else if (test_result->signal == SIGABRT) {
            snprintf(error_msg, sizeof(error_msg), "process aborted\n");
        }
        else if (test_result->signal == SIGFPE) {
            snprintf(error_msg, sizeof(error_msg), "floating point error\n");
        }
        else {
            snprintf(error_msg, sizeof(error_msg), "killed by signal %d\n", test_result->signal);
        }
        strncat(test_result->test_output, error_msg, sizeof(test_result->test_output) - strlen(test_result->test_output) - 1);
    }
}

// MAIN EXECUTOR FUNCTION
// parameters: test struct
// return a Test_result struct
Test_result execute_test(Test *test) {
    Test_result test_result;
    memset(&test_result, 0, sizeof(Test_result));

    int pipe_stdin[2];
    int pipe_stdout[2];
    if (pipe(pipe_stdin) < 0 || pipe(pipe_stdout) < 0) {
        perror("pipe() failed");
        test_result.crashed = 1;
        return test_result;
    }  

    // Stdin Pipe: Parent writes -> Child reads (Parent will use pipe_stdin[1] to write and child will use pipe_stdin[0])
    // Stdout Pipe: Child writes -> Parent reads (Child will use pipe_stdout[1] to write and parent will use pipe_std[0] to read)
    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork Failed");
        test_result.crashed = 1;
        return test_result;
    }
    if (pid == 0) {                 // CHILD    
        dup2(pipe_stdin[0], 0);
        dup2(pipe_stdout[1], 1);
        dup2(pipe_stdout[1], 2);    // capture error

        close(pipe_stdin[0]);       // close all unused pipes
        close(pipe_stdin[1]);
        close(pipe_stdout[0]);
        close(pipe_stdout[1]);

        // tokenizing arguments to pass into execvp (using a local temp_args to modify)
        char *args[64];
        int arg_count = 0;
        char temp_args[1024];
        strncpy(temp_args, test->command_line, sizeof(temp_args));
        temp_args[sizeof(temp_args) - 1] = '\0';

        char *token = strtok(temp_args, " ");
        while (token != NULL && arg_count < 63) {
            args[arg_count++] = token;
            token = strtok(NULL, " ");
        }
        args[arg_count] = NULL;
        execvp(args[0], args);
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
        int timeout_secs = 5;
        int status;
        pid_t wpid;

        /* WNOHANG prevents wait()/waitpid() from blocking so that the process can go on with other tasks. 
        WNOHANG returns 0 if the child process is still actively running */
        while ((wpid = waitpid(pid, &status, WNOHANG)) == 0) {
            if (time(NULL) - start_time >= 5) {     // timeout checker
                kill(pid, SIGKILL);                 // kill the child
                waitpid(pid, &status, 0);           // collect zombie child
                test_result.timed_out = 1;
                test_result.crashed = 1;
                strncat(test_result.test_output, "\nprocess killed (infinite loop)\n", sizeof(test_result.test_output) - strlen(test_result.test_output) - 1);
                break;
            }

            bytes_read = read(pipe_stdout[0], buffer, sizeof(buffer));          // read whatever output is currently available
            if (bytes_read > 0 && total_read + bytes_read < sizeof(test_result.test_output) - 1) {
                memcpy(test_result.test_output + total_read, buffer, bytes_read);
                total_read += bytes_read;
                test_result.test_output[total_read] = '\0';
            }
        }

        if (!test_result.timed_out) {        // if child finished naturally or produce a different status:
            int bytes_read;
            while ((bytes_read = read(pipe_stdout[0], buffer, sizeof(buffer))) > 0) {       // either read remaining output from the pipe
                if (total_read + bytes_read < sizeof(test_result.test_output) - 1) {
                    memcpy(test_result.test_output + total_read, buffer, bytes_read);
                    total_read += bytes_read;
                    test_result.test_output[total_read] = '\0';
                }
            }
            error_handler(status, &test_result);         // or call error_handler to deal with the status
        } 
        close(pipe_stdout[0]);
    }
    return test_result;
}

// DEBUGGING
#ifdef DEBUG_EXECUTOR
int main() {
    Test tests[10];
    int count = 0;
    parse_test("test_calc_c", tests, &count);
    for (int i = 0; i < count; i++) {
        printf("---Running %s---\n", tests[i].name);
        Test_result r = execute_test(&tests[i]);
        printf("Output: %s\n", r.test_output);
        printf("Crashed: %d, Timeout: %d, Exit Code: %d\n\n", r.crashed, r.timed_out, r.exit_status);
    }
    return 0;
}
#endif