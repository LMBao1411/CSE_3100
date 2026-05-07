#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void bubble_sort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

void selection_sort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (arr[j] < arr[min_idx]) min_idx = j;
        }
        int tmp = arr[min_idx];
        arr[min_idx] = arr[i];
        arr[i] = tmp;
    }
}

void merge(int arr[], int l, int m, int r) {
    int n1 = m - l + 1, n2 = r - m;
    int L[n1], R[n2];
    for (int i = 0; i < n1; i++) L[i] = arr[l + i];
    for (int j = 0; j < n2; j++) R[j] = arr[m + 1 + j];
    int i = 0, j = 0, k = l;
    while (i < n1 && j < n2) arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void merge_sort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        merge_sort(arr, l, m);
        merge_sort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

void print_array(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i < n - 1) printf(" ");
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage: %s <bubble|selection|merge> <n1> <n2> ...\n", argv[0]);
        return 1;
    }

    char *algo = argv[1];
    int n = argc - 2;
    int arr[64];
    for (int i = 0; i < n; i++) arr[i] = atoi(argv[i + 2]);

    if (strcmp(algo, "bubble") == 0) {
        bubble_sort(arr, n);
    } else if (strcmp(algo, "selection") == 0) {
        selection_sort(arr, n);
    } else if (strcmp(algo, "merge") == 0) {
        merge_sort(arr, 0, n - 1);
    } else {
        printf("Unknown algorithm: %s\n", algo);
        return 1;
    }

    print_array(arr, n);
    return 0;
}