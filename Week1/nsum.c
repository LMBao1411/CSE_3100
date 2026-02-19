#include <stdio.h>
int main(){
    int n;
    printf("enter an integer: ");
    scanf("%d",&n);
    int i = 1;
    int sum = 0;
    while (i <= n) {
        sum += i;
        i += 1;
    }
    printf("your sum from 1 to n is %d", sum);
    return 0;
}