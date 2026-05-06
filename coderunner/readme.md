# Overview
How to run coderunner
1. prepare files to run
2. fill out the test_input file
3. compile all files
4. run ./main
5. results.json will be produced

DEBUGGING:
gcc -DDEBUG_PARSER parser.c -o parser
gcc -DDEBUG_EXECUTOR executor.c parser.c -o executor
gcc -DDEBUG_OUTPUT output.c executor.c parser.c -o output

Description of each function: 

parser.c 
parse the input file containing all the information needed to run the target file
the goal is to extract the strings from the input file and store it in an array or a struct
to not increase the complexity of the input files, 
I will just have the target file to run and its corresponding commands + parameters + stdins to run
(not having a target file that will need another file to run)

executor.c
- get the parameters needed for execvp to run
- set up file descriptors to redirect input / output to retrieve the data
- redirect file descriptors and create communicating pipe channel to retrive output of the test cases into something that we can stored and process later
- set up child process to run execvp
- child run execvp() and parent read the output
-Need to handle infinite loops + seg faults + memory overflow + etc.

output.c 
- take the test_output received from executor from the Test_result struct then compare with the expected output
- generate the JSON file

main.c
- call parser.c to run and retrieve data into array of structs
- call executor.c to run all the test cases for the file
- deal with edge cases: infinity loops, seg fault, memory overflow via having exit status, error flags, runtime flags, etc.
- call output.c to run and print results from test cases into a JSON file (results.json)