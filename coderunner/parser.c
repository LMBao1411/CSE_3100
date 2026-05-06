#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

// -----HELPER----- 

// Extracts content between { and } into 'dest'
void extract_value(const char *line, char *dest) {
    char *start = strchr(line, '{');
    char *end = strrchr(line, '}');
    
    if (start && end && (end > start)) {
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
            else if (strstr(line, "command:")) {
                extract_value(line, tests[*count].command_line);
            } 
            else if (strstr(line, "expected_output:")) {
                extract_value(line, tests[*count].expected_output);
            }
            else if (strstr(line, "score:")) {
                char temp_score[32];
                extract_value(line, temp_score);
                tests[*count].score = atoi(temp_score);
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
    parse_test("test_calc_c", tests, &count);
    for (int i = 0; i < count; i++) {
        printf("Test #%d\n", i + 1);
        printf("Name: %s\n", tests[i].name);
        printf("Command: %s\n", tests[i].command_line);
        printf("Expected Output: %s\n", tests[i].expected_output);
        printf("Score if test passed: %d\n", tests[i].score);
        printf("\n");
    }
    return 0;
}
#endif