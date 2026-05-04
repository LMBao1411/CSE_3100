#ifndef PARSER_H
#define PARSER_H

typedef struct {
    char name[256];
    char command[512];
    char parameters[2058];
    char expected_output[4096];
} Test;

void extract_value(const char *line, char *dest);
void comma_replace(char *str);
void parse_test(char* filename, Test tests[], int *count);

#endif