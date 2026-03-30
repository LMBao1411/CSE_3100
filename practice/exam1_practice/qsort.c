#include <stdio.h>
#include <stdlib.h>
// comparison function
// take elements a and b
// if a<b return -1
// if a!=b return 0
// if a>b return 1

typedef struct {
    int id;
    int points;
} Exam1;

int gradesCompare(const void *a, const void *b) {
    Exam1 *pa = (Exam1 *)a;
    Exam1 *pb = (Exam1 *)b;
    return pa->points - pb->points;
}

int integerCompare(const void *a, const void *b) {
    int *ia = (int*)a;
    int *ib = (int*)b;

    // if (*ia < *ib)
    //     return -1;
    // else if (*ia == *ib) 
    //     return 0;
    // else
    //     return 1;
    return *ia - *ib;
//     return *(int*)a - *(int*)b;  // or just type cast them down here instead
}

int main() {
    int a[10] = {4,2,27,8,9,10,1,13,-67,100};
    // n = 10;
    // size = sizeof(int);
    // base = a;
    printf("Elements before sorting:\n");
    for (int i=0; i<10; i++) {
        printf("%d, ",a[i]);
    }
    printf("\n");

    qsort(a, 10, sizeof(int), integerCompare);

    printf("Elements after sorting:\n");
    for (int i=0; i<10; i++) {
        printf("%d, ",a[i]);
    }

    Exam1 grades[5];
    grades[0] = (Exam1){1, 99};
    grades[1] = (Exam1){2, 67};
    grades[2] = (Exam1){3, 78};
    grades[3] = (Exam1){4, 23};
    grades[4] = (Exam1){5, 90};

    printf("Points before sorting:\n");
    for (int i=0; i<5; i++) {
        printf("ID: %d, Points: %d\n", grades[i].id, grades[i].points);
    }
    printf("\n");

    qsort(grades, 5, sizeof(Exam1), gradesCompare);

    printf("Points after sorting:\n");
    for (int i=0; i<5; i++) {
        printf("ID: %d, Points: %d\n", grades[i].id, grades[i].points);
    }
        return 0;
}