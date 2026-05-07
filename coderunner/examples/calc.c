#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: %s n1 n2", argv[0]);
		return -1;
    }

    int a = atoi(argv[1]);
    int b = atoi(argv[2]);
    int sum = a + b;
    printf("%d", sum);
    return 0;
}