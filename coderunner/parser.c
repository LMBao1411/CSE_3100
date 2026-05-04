#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

// typedef struct {
//     char name[256];
//     char command[256];
//     char args_str[1024];      
//     char stdin_input[1024];
//     char expected_output[4096];
// } Test;

// -----HERLPER FUNCTIONS----- 

// Extracts content between { and } into 'dest'
void extract_value(const char *line, char *dest) {
    char *start = strchr(line, '{');
    char *end = strrchr(line, '}');
    
    if (start && end && (end > start)) {        // validate a string is inside {} AND is in the correct order
        int len = end - start - 1;
        if (len > 0) {
            strncpy(dest, start + 1, len);
            dest[len] = '\0';
        } 
        else {
            dest[0] = '\0';
        }
    }
}

// Replace commas with space in parameters
void comma_replace(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == ',') {
            str[i] = ' ';
        }
    }
}

// MAIN PARSER FUNCTION
// parameters: filename, array to store parsed test cases, ptr keeping track of how many tests are loaded
void parse_test(char* filename, Test tests[], int *count) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    char line[5000];
    *count = -1;        // Start at -1 so the first '-' bumps it to 0

    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '*') {       // stopping point
            break;
        }
        if (line[0] == '-') {       // detected a new test case block
            (*count)++;
            memset(&tests[*count], 0, sizeof(Test));        // memset prevents garbage values
            continue;
        }

        if (*count >= 0) {
            if (strstr(line, "name:")) {
                extract_value(line, tests[*count].name);
            } 
            else if (strstr(line, "commands:")) {
                extract_value(line, tests[*count].command);
            } 
            else if (strstr(line, "parameters:")) {
                extract_value(line, tests[*count].args_str);        // Map parameters to args_str
                if (strcmp(tests[*count].args_str, "NONE") == 0) {
                    tests[*count].args_str[0] = '\0';               // Clear if NONE
                } 
                else {
                    comma_replace(tests[*count].args_str);
                }
            } 
            else if (strstr(line, "expected_output:")) {
                extract_value(line, tests[*count].expected_output);
            }
        }
    }
    (*count)++;
    fclose(file);
}

// DEBUGGING
#ifdef DEBUG_PARSER
int main() {
    Test tests[100];
    int count = 0;
    parse_test("test_input", tests, &count);
    for (int i = 0; i < count; i++) {
        printf("Test #%d\n", i + 1);
        printf("Name: %s\n", tests[i].name);
        printf("Commands: %s\n", tests[i].command);
        printf("Args: %s\n", tests[i].args_str);
        printf("Expected Output: %s\n", tests[i].expected_output);
        printf("\n");
    }
    return 0;
}
#endif