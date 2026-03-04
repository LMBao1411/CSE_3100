#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Usage: ./calculator <operation>\n");
    return 1;
  }

  int num1, num2, result = 0;
  printf("Enter num1:\n");
  scanf("%d", &num1);

  printf("Enter num2:\n");
  scanf("%d", &num2);

  if (strcmp(argv[1], "add") == 0) {
    result = num1 + num2;
    printf("%d + %d = %d\n", num1, num2, result);
  } else if (strcmp(argv[1], "mul") == 0) {
    result = num1 - num2;
    printf("%d * %d = %d\n", num1, num2, result);
  } else {
    printf("Invalid operation: %s\n", argv[1]);
    return 1;
  }

  return 0;
}

