/*MAIN CODE RUNNER FILE*/

/*
call parser.c to run and retrieve data into array of structs

set up file descriptors to redirect input / output to retrieve the data

set up child process to run execvp

redirect file descriptors and create communicating pipe channel to retrive output of the test cases into something that we can stored and process later

call executor.c to run all the test cases for the file

deal with edge cases: infinity loops, seg fault, memory overflow
        via having exit status, error flags, runtime flags, etc.

call output.c to run and print results from test cases into a JSON file (reults.json) 

free up memory, close file descriptors end, make sure nothing is left open or leaking

check with valgrind
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <parser.h>
#include <executor.h>
#include <output.h>


int main() {
    return 0;
}