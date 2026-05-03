#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct {
    char name[256];
    char commands[512];
    char stdin_input[4096];
    char expected_output[4096];
} Test;

// HERLPER FUNCTION: Extracts content between { and } into 'dest'
void extract_value(const char *line, char *dest) {
    char *start = strchr(line, '{');
    char *end = strrchr(line, '}');
    
    if (start && end && end > start) {
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

/*MAIN PARSER FUNCTION - parameters: filename, array to store parsed test cases, ptr keeping track of how many tests are loaded*/ 
void parse_test(char* filename, Test tests[], int *count) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return;
    }

    char line[5000];
    *count = -1;        // Start at -1 so the first '-' bumps it to 0

    while (fgets(line, sizeof(line), file)) {
        if (line[0] == "*") {       // stopping point
            break;
        }
        if (line[0] == "-") {       // detected a new test case block
            (*count)++;
            memset(&tests[*count], 0, sizeof(Test));        // memset prevents garbage values
            continue;
        }

        if (*count >= 0) {          // only read if a test block exists
            if (strstr(line, "name: ")) {
                extract_value(line, tests[*count].name);
            }
            else if (strstr(line, "commads: ")) {
                extract_value(line, tests[*count].commands);
            }
            else if (strstr(line, "parameters:")) {
                extract_value(line, tests[*count].stdin_input);
            } 
            else if (strstr(line, "expected_output:")) {
                extract_value(line, tests[*count].expected_output);
            }
        }
    }
    (*count)++;
    fclose(file);
}

