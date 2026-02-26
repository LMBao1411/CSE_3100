// Do not modify starter code
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MAX_LEN 100

void commonChars(char arr[][MAX_LEN], int n) {
  int common[26];
  for (int i = 0; i < 26; i++) {
    common[i] = true;
  }

  for (int i = 0; i < n; i++) {
    // file in code here
    int present[26] = {false};  // intialize temporary record of each string
    for (int j=0; arr[i][j] != '\0'; j++) { // loop through each character of that string
      char c = arr[i][j];
      present[c - 'a'] = true;  // mark the letter of that string true in the present record
    }

    for (int i=0; i<26; i++) {
      common[i] = common[i] && present[i];
    }
  }

  printf("Common characters: ");

  // fill code here
  int found = true;
  for (int i=0; i<26; i++) {
    if (common[i]==true) {
      printf("%c ", i + 'a');
      found = true;
    }
  }
  
  if (!found) {
    printf("None");
  }
  printf("\n");
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Usage: %s string1 string2 ...\n", argv[0]);
    return 1;
  }

  int n = argc - 1;
  char arr[n][MAX_LEN];

  for (int i = 0; i < n; i++) {
    strcpy(arr[i], argv[i + 1]);
  }

  commonChars(arr, n);

  return 0;
}