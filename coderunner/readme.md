# CodeRunner Overview

## How to Run CodeRunner

1. Prepare the files you want to test
2. Fill out your test input file
3. Compile target file and run **make all**
4. Run: ./main <test_case_input for the target file>
5. A results.json file will be generated

---

### Debugging

gcc -DDEBUG_PARSER parser.c -o parser

gcc -DDEBUG_EXECUTOR executor.c parser.c -o executor

gcc -DDEBUG_OUTPUT output.c executor.c parser.c -o output