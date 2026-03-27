#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int p_to_c1[2], c1_to_c2[2], c2_to_p[2];

    pipe(p_to_c1);
    pipe(c1_to_c2);
    pipe(c2_to_p);

    // Child 1
    if (fork() == 0) {
        char ch;

        close(p_to_c1[1]);
        close(c1_to_c2[0]);
        close(c2_to_p[0]);
        close(c2_to_p[1]);

        while (read(p_to_c1[0], &ch, 1) > 0) {
            printf("child 1 read: %c\n", ch);

            if (ch == 'z') {
                write(c1_to_c2[1], &ch, 1); // pass it along
                break;
            }

            ch++;
            printf("child 1 wrote: %c\n", ch);
            write(c1_to_c2[1], &ch, 1);
        }

        close(p_to_c1[0]);
        close(c1_to_c2[1]);
        exit(0);
    }

    // Child 2
    if (fork() == 0) {
        char ch;

        close(c1_to_c2[1]);
        close(c2_to_p[0]);
        close(p_to_c1[0]);
        close(p_to_c1[1]);

        while (read(c1_to_c2[0], &ch, 1) > 0) {
            printf("child 2 read: %c\n", ch);

            if (ch == 'z') {
                write(c2_to_p[1], &ch, 1); // pass it along
                break;
            }

            ch++;
            printf("child 2 wrote: %c\n", ch);
            write(c2_to_p[1], &ch, 1);
        }

        close(c1_to_c2[0]);
        close(c2_to_p[1]);
        exit(0);
    }

    // Parent
    char ch = 'a';

    close(p_to_c1[0]);
    close(c1_to_c2[0]);
    close(c1_to_c2[1]);
    close(c2_to_p[1]);

    while (1) {
        printf("parent wrote: %c\n", ch);
        write(p_to_c1[1], &ch, 1);

        if (read(c2_to_p[0], &ch, 1) <= 0) break;
        printf("parent read: %c\n", ch);

        if (ch == 'z') {
            break;
        }

        ch++;
        sleep(1);
    }

    close(p_to_c1[1]);
    close(c2_to_p[0]);

    wait(NULL);
    wait(NULL);

    return 0;
}