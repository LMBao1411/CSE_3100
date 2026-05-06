#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "executor.h"
#include "parser.h"
#include "output.h"

// ----- HELPER -----
void print_json_value(FILE *f, const char *str) {
    if (str == NULL || strlen(str) == 0 || strcmp(str, "NONE") == 0) {
        fprintf(f, "null");
    } 
    else {
        fprintf(f, "\"%s\"", str);
    }
}

void generate_json_file(Test tests[], Test_result test_results[], int count) {
    FILE *filename = fopen("results.json", "w");
    if (!filename) {
        perror("results.json failed to open");
        return;
    }

    fprintf(filename, "{\n    \"tests\": [\n");

    for (int i = 0; i < count; i++) {
        char expected[4096];
        char actual[4096];
        strncpy(expected, tests[i].expected_output, sizeof(expected));
        strncpy(actual, test_results[i].test_output, sizeof(actual));

        if (strcmp(expected, "NONE") == 0) {
            expected[0] = '\0';
        }

        int passed = 0;
        if (test_results[i].crashed == 1 || test_results[i].timed_out == 1) {
            passed = 0;
        }
        else if (strcmp(expected, actual) == 0) {
            passed = 1;
        }

        int final_score;
        if (passed) {final_score = tests[i].score;} 
        else {final_score = 0;}

        const char* status_str;
        if (passed) {status_str = "passed";} 
        else {status_str = "failed";}

        fprintf(filename, "     {\n");
        fprintf(filename, "         \"name\": \"%s\",\n", tests[i].name);
        fprintf(filename, "            \"file_output\": ");
        print_json_value(filename, actual);
        fprintf(filename, ",\n");
        fprintf(filename, "            \"expected_output\": ");
        print_json_value(filename, tests[i].expected_output);
        fprintf(filename, ",\n");
        fprintf(filename, "            \"status\": \"%s\",\n", status_str);
        fprintf(filename, "            \"score\": %d\n", final_score);

        // Add comma to all but the last object
        if (i < count - 1) {
            fprintf(filename, "        },\n");
        } else {
            fprintf(filename, "        }\n");
        }
    }
    fprintf(filename, "    ]\n}\n");
    fclose(filename);
}

//DEBUGGING
#ifdef DEBUG_OUTPUT
int main() {
    Test tests[100];
    Test_result results[100];
    int count = 0;
    parse_test("test_calc_c", tests, &count);
    for (int i = 0; i < count; i++) {results[i] = execute_test(&tests[i]);}
    generate_json_file(tests, results, count);
    return 0;
}
#endif