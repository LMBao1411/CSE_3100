#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* print out an error message and exit */
void my_error(char *s)
{
    perror(s);
    exit(1);
}

/* Concatnate two strings.
 * Dynamically allocate space for the result.
 * Return the address of the result.
 */
char *my_strcat(char *s1, char *s2)
{
    // TODO
    char *result;
    int len_s1 = 0, len_s2 = 0;
    if (!s1 || !s2) {return NULL;}

    len_s1 = strlen(s1);
    len_s2 = strlen(s2);
    int total_len = len_s1 + len_s2 + 1;

    result = malloc(total_len);
    if (result == NULL) {
        my_error("malloc failed");
    }

    if (s1) {strcpy(result, s1);}
    else result[0] = '\0';
    if (s2) {strcat(result, s2);}
    return result;
}

int main(int argc, char *argv[])
{
    if (argc < 1) {return 0;}

    char *s;
    s = my_strcat("", argv[0]);

    for (int i = 1; i < argc; i ++) {
        char *temp = s;
        s = my_strcat(temp, argv[i]);
        free(temp);     // ALWAYS free old memory after copying
    }

    printf("%s\n", s);
    free(s);

    return 0;
}
