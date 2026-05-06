#ifndef OUTPUT_H
#define OUTPUT_H
#include "parser.h"
#include "executor.h"

typedef struct {
        char name[256];
        char actual_output[4096];
        char expected_output[4096];
        char status[128];
        int score;
} Test_score;

void print_json_value(FILE *f, const char *str);
void generate_json_file(Test tests[], Test_result test_results[], int count);

#endif