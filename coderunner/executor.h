#ifndef EXECUTOR_H
#define EXECUTOR_H

typedef struct {
    char test_output[4096];
    int exit_status;
    int crashed;
    int timed_out;
    int signal;
} Test_result;

void error_handler(int status, Test_result *result);
Test_result execute_test(Test *test);

#endif