parser.c
parse the input file containing all the information needed to run the target file
the goal is to extract the strings from the input file and store it in an array or a struct
to not increase the complexity of the input files, 
I will just have the target file to run and its corresponding commands + parameters + stdins to run
(not having a target file that will need another file to run)

- (indicates there will be a test after "-")
test1
    name: {}
    commands: {}
    parameters: {}
    expected_output: {}
-
test2
    name: {}
    commands: {}
    parameters: {}
    expected_output: {}
-
test3
    name: {}
    commands: {}
    parameters: {}
    expected_output: {}
* ("*" indicates the end of the input file, no more tests to run)

The parser will try to encode the string into a struct

OUTLINE OF PARSER.C

typedef struct {
    char name[256];
    char commands[512];
    char stdin_input[4096];
    char expected_output[4096];
    int score;
} Test

function parser(filename, tests[], *count) {
    open file
    *count = 0

    WHILE not end of filename {
        read line

        if line starts with "*" {
            break
        }
        
        if line starts with "-" {
            t = empty Test struct

            WHILE reading the line {
                IF line starts with '-' or '*': {BREAK}  // end of this test

                IF line contains "name:": {extract value -> t.name}

                IF line contains "command:": {extract value -> t.command}

                IF line contains "stdin:": {extract value -> t.stdin_input}

                IF line contains "expected_output:": {extract value -> t.expected_output}
            }
        }
        tests[*count] = t
    (*count)++
    }
}