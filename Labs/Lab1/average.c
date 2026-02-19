// Only this line of comment is provided 
#include <stdio.h>

int main() {
    double x, total, average;
    int count = 0;
    while (scanf("%lf", &x) == 1) {         // pay attention to %lf
        count++;
        total += x;
        average = total/count;
        printf("Total=%f Average=%f\n", total, average);        // pay attention to %f
    }
    printf("\nEOF received. Terminating loop.\n");
    return 0;
}