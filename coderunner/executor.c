/* executes the files using execvp
get the parameters needed for execvp to run

fork() a child process
Set up pipes for stdin/stdout capture
Use execvp() to run the command
Need to handle infinite loops + seg faults + memory overflow + etc.

*/

/*
OUTLINE OF EXECUTOR

function execute_test(test, result_output[], *exit_status) {
    // create two pipes: stdin and stdout 
    int pipe_stdin[2];
    int pipe_stdout[2];
    pipe(pipe_stdin);
    pipe(pipe_stdout);

    // make child process
    pid = fork();

    if pid == 0 {   // CHILD
        close unused pipes
    }

}

*/
void execute_test() {
    return;
}