# CodeRunner Overview

## How to Run CodeRunner

1. Prepare the files you want to test
2. Fill out your test input file
3. Compile all source files
4. Run: ./main
5. A results.json file will be generated
---

## File Descriptions

### `parser.c`

* Parses the input test file
* Extracts all required test information
* Stores test data into structs
* Keeps input formatting simple

---

### `executor.c`

* Builds parameters for execvp
* Sets up file descriptors
* Redirects input/output
* Uses pipes to capture test output
* Creates child processes to run execvp and write output to parent process
* Parent process read the output and deal with child process status
* Handles: infinite loops, segmentation faults, runtime crashes, memory issues

---

### `output.c`

* Compares actual output with expected output
* Processes Test_result data
* Generates results.json

---

### `main.c`

* Runs parser
* Executes all test cases
* Calls output generator

---

### Debugging Commands

gcc -DDEBUG_PARSER parser.c -o parser
gcc -DDEBUG_EXECUTOR executor.c parser.c -o executor
gcc -DDEBUG_OUTPUT output.c executor.c parser.c -o output
