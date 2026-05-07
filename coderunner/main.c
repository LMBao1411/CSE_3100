#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "parser.h"
#include "executor.h"
#include "output.h"
#define MAX_TESTS 10

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <test_input_file>\n", argv[0]);
		return EXIT_FAILURE;
    }

    Test tests[MAX_TESTS];
    Test_result test_results[MAX_TESTS];
    int count = 0;

    parse_test(argv[1], tests, &count);

    for (int i = 0; i < count; i++) {
        printf("Running test %d: %s\n", i + 1, tests[i].name);
        test_results[i] = execute_test(&tests[i]);
    }

    generate_json_file(tests, test_results, count);

    printf("Testing complete. Results saved to results.json\n");

    return 0;
}