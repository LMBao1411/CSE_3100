#include <stdio.h>
#include <stdlib.h>

int main() {
    int r = 3;
    int c = 5;

    // Allocate the array of pointers
    int **rows = malloc(r * sizeof(int*));
    if (rows == NULL) return 1; // Always good to check if malloc failed

    // Allocate each row
    for (int i = 0; i < r; i++) {
        rows[i] = malloc(c * sizeof(int));
    }

    // Clean up
    for (int i = 0; i < r; i++) {
        free(rows[i]);
    }
    free(rows);
    return 0;
}