#include <stdio.h>
#include <stdlib.h>

void free_rows(int **rows, int r) {
    for (int i = 0; i < r; i++) {
        free(rows[i]);
    }
    free(rows);
}

int main() {
    int r = 3;
    int c = 5;

    // Allocate the array of pointers
    int **rows = malloc(r * sizeof(int*));
    if (rows == NULL) return 1; // Always good to check if malloc failed

    // Allocate each row
    for (int i = 0; i < r; i++) {
        rows[i] = malloc(c * sizeof(int));
        for (int j = 0; j < c; j++) { // Initialize with some values so it's not random garbage
            rows[i][j] = i + j; 
        }
    }

    // Print the whole grid
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            printf("%d ", rows[i][j]);
        }
        printf("\n"); // This newline ensures the buffer flushes every row
    }

    // This allocates the entire grid as one contiguous chunk
    // matrix is a single pointer. It points to a whole block of c integers (a row).
    int (*matrix)[c] = malloc(r * sizeof(*matrix));
    // When you write matrix[i][j], the compiler doesn't have to look up a second pointer address. 
    // It performs a simple linear calculation to find the element in that single block of memory:
    // Address = BaseAddress + (i x c + j) x sizeof(int)
    matrix[2][3] = 1000;
    printf("%d\n", matrix[2][3]);

    free(matrix);
    free_rows(rows, r);
    return 0;
}