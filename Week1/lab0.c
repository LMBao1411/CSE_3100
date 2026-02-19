#include <stdio.h>
int main(void)
{
    printf("Hello, World!\n");

    int N = 200;
    int i = 1;
    int sum = 0;
    while (i < N) {
        if (i % 2 == 0) {
            sum += i;
        } 
        i += 1;
    }
    printf("%d\n", sum);
    return 0;
}
