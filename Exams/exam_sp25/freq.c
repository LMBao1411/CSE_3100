#include <stdio.h>
#include <stdlib.h>

// TODO: Implement this function
unsigned firstDigit(unsigned k) {
    // Fill in code below to return the first digit of the number k
    // Hint: For positive numbers, you can continuously divide by 10

    return 0; // Replace this with your logic
}

// TODO: Implement this function
void FirstDigitCount(unsigned *a, unsigned n, unsigned freq[10]) {
    // Fill in your code below
    // 1. Initialize all values in freq to 0 so there are no garbage values
    // 2. Iterate through array 'a', find the first digit of each number
    // 3. Increment the corresponding index in the freq array

}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: ./freq [list of positive integers]\n");
        exit(0);
    }

    unsigned n = argc - 1;
    unsigned *arr = (unsigned *)malloc(n * sizeof(unsigned));

    for (int i = 0; i < n; i++) {
        arr[i] = (unsigned)atoi(argv[i+1]);
    }

    unsigned freq[10];

    FirstDigitCount(arr, n, freq);

    for (int i = 0; i < 10; i++) {
        printf("%d\t%u\n", i, freq[i]);
    }

    free(arr);
    return 0;
}
