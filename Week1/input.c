#include <stdio.h>
int main() {
    int n;
    printf("enter an integer: ");
    scanf("%d", &n);
    printf("Your input is %d\n", n);

    int result = n*2;
    printf("your input multiplied by two is equal to %d\n", result);
    printf("your input is %d, and double of your input is %d", n, result);
    return 0;
}