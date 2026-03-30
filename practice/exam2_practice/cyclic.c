#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/*
Parent
    writes to p_to_c1[1]
    reads from c2_to_p[0]
Child 1
    reads from p_to_c1[0]
    writes to c1_to_c2[1]
Child 2
    reads from c1_to_c2[0]
    writes to c2_to_p[1]
*/

// [Parent] → [C1] → [C2] → back to [Parent]

int main() {
    int p_to_c1[2], c1_to_c2[2], c2_to_p[2];

    pipe(p_to_c1);
    pipe(c1_to_c2);
    pipe(c2_to_p);

    if (fork() == 0) {
        // Child 1
        char ch;

        close(p_to_c1[1]);
        close(c1_to_c2[0]);

        while (read(p_to_c1[0], &ch, 1) > 0) {
            printf("child 1 read: %c\n", ch);
            ch++;
            printf("child 1 wrote: %c\n", ch);
            write(c1_to_c2[1], &ch, 1);
        }
        exit(0);
    }

    if (fork() == 0) {
        // Child 2
        char ch;

        close(c1_to_c2[1]);
        close(c2_to_p[0]);

        while (read(c1_to_c2[0], &ch, 1) > 0) {
            printf("child 2 read: %c\n", ch);
            ch++;
            printf("child 2 wrote: %c\n", ch);
            write(c2_to_p[1], &ch, 1);
        }
        exit(0);
    }

    // Parent
    char ch = 'a';

    close(p_to_c1[0]);
    close(c2_to_p[1]);

    while (1) {
        printf("parent wrote: %c\n", ch);
        write(p_to_c1[1], &ch, 1);

        read(c2_to_p[0], &ch, 1);
        printf("parent read: %c\n", ch);

        ch++;  // continue progression

        sleep(1); // slow it down so you can see output
    }

    return 0;
}