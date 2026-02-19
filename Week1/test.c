#include <stdio.h>

int findCount(int n) {
    if (n==0) {
        return 1;
    }
    int count = 0;
    while (n != 0) {
        count++;
        n /= 10;
    }
    return count;
}

int reverseNum(int num) {
    int rev_num = 0;
    while (num > 0) {
        rev_num = rev_num * 10 + num % 10;
        num /= 10;
    }
    return rev_num;
}

int checkPrime(int n) {
    int count = 0;
    if (n <= 1) {
        printf("Not a prime number\n");
    }
    else {
        for (int i=2.; i*i<=n; i++) {
            if (n%i==0) {
                count++;
            }
        }
        if (count>0) {
        printf("Not a prime number\n");
        }
        else {
            printf("This is a prime number\n");
        }
    }
    return 0;
}


int main() {
    int n = 10;
    for (int i = 1; i <= n; i++) {
        printf("%d ",i);
    }
    printf("\n");
    int N = 100;
    for (int i=1; i<=N; i++) {
        if (i %2 == 0) {
            printf("%d ",i);
        }
    }
    printf("\n");

    int m = 100; int sum = 0;
    for (int i=1; i<=m; i++) {
        sum += i;
    }
    printf("sum from 1 to 100 is %d\n", sum);

    int M = 10; int product = 1;
    for (int i=1; i<=M; i++) {
        product *= i;
    }
    printf("product from 1 to 10 is %d\n", product);

    int p = 1;
    for (int i=50; i>=p; i--) {
        printf("%d ", i);
    }
    printf("\n");

    int P = 1234567;
    printf("Number of digitis in P is: %d\n", findCount(P));

    int t = 987654;
    printf("The reverse number of t is: %d\n", reverseNum(t));

    int T = 29;
    checkPrime(T);
    return 0;

    for (int i=0; i<5;i++) {
        for (int j=0;j<=i;j++){
            printf("+ ");
        }
        printf("\n");
    }
    return 0;
}