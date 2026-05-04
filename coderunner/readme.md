Overview
How to run coderunner
1. prepare files to run
2. fill out the test_input file
3. compile all files
4. run ./main
5. results.json will be produced



Description of each function: 

parser.c 
parse the input file containing all the information needed to run the target file
the goal is to extract the strings from the input file and store it in an array or a struct
to not increase the complexity of the input files, 
I will just have the target file to run and its corresponding commands + parameters + stdins to run
(not having a target file that will need another file to run)

executor.c
get the parameters needed for execvp to run
set up file descriptors to redirect input / output to retrieve the data
set up child process to run execvp
redirect file descriptors and create communicating pipe channel to retrive output of the test cases into something that we can stored and process later
        fork() a child process
        Set up pipes for stdin/stdout capture
Use execvp() to run the command
Need to handle infinite loops + seg faults + memory overflow + etc.

output.c 
take the output from the input file and compare with the expected output
then generate the JSON file which the format could be referenced from the pyhthon implementation

main.c
call parser.c to run and retrieve data into array of structs
call executor.c to run all the test cases for the file
deal with edge cases: infinity loops, seg fault, memory overflow
        via having exit status, error flags, runtime flags, etc.
call output.c to run and print results from test cases into a JSON file (reults.json)