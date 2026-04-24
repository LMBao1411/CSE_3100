/*parse the input file containing all the information needed to run the target file
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

typedef struct {
    NAME
    COMMANDS
    STDIN
    EXPECTED_OUTPUT
} Test;

*/ 
