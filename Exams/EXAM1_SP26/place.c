#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_elements(char **elems, int count) {
  for (int i = 0; i < count; i++) {
    printf("%s\n", elems[i]);
  }
}

// HELPER: comparator function
int stringCompare(const void *s1, const void *s2) {
  int count1 = 0;
  int count2 = 0;
  char *sa = *(char **) s1;
  char *sb = *(char **) s2;

  for (int i = 0; sa[i] != '\0'; i++) {
    char c = sa[i];
    if ((c >= 'a') && (c <= 'z')) {
      count1++;
    }
  }
  for (int i = 0; sb[i] != '\0'; i++) {
    char d = sb[i];
    if ((d >= 'a') && (d <= 'z')) {
      count2++;
    }
  }
  return count1 - count2;
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    perror("Usage : ./place <strings>");
    exit(1);
  }

  // params: address of the array of strings, n strings, size of the each string, comparator function
  qsort(&argv[1], argc - 1, sizeof(char *), stringCompare);
  print_elements(&argv[1], argc - 1);
  return 0;
}
