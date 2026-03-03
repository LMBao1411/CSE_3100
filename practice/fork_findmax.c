#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int arr[] = {12, 45, 7, 89, 34, 65, 23, 90};
    int n = sizeof(arr) / sizeof(arr[0]);

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }

    int cur = 0;
    int highest = 0;
    int first_half = n / 2;
    for (int i = 0; i<n; i++) {
        // CHILD PROCESS
        if (pid == 0) {
            if (arr[cur] > highest && cur <= first_half) {
                highest = arr[cur];
            }
            cur++;
            printf("Child's PID = %d, its max value it found = %d\n", getpid(), highest);
        }
        // PARENT PROCESS
        else {
            waitpid(pid, NULL, 0);
            int backward = n-1;
            if (arr[backward] > highest && backward > first_half) {
                highest = arr[backward];
            }
            backward--;
            printf("Parent max value it found = %d\n", highest);
        }
    }
    return 0;
}