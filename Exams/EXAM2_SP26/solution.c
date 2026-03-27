#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
    int num = atoi(argv[1]);
    int p_c1[2];
    int c1_p[2];

    int p_c2[2];
    int c2_p[2];

    int c1_c2[2];
    int c2_c1[2];

    pipe(p_c1);
    pipe(c1_p);

    pipe(p_c2);
    pipe(c2_p);

    pipe(c1_c2);
    pipe(c2_c1);

    // Child 1
    pid_t child1 = fork();
    if (child1 == 0) {
        // step 1: receive from parent
        close(p_c1[1]);
        read(p_c1[0], &num, sizeof(int));
        printf("Child 1 read: %d\n", num);

        // step 2: write to child 2
        num = num * 2;
        close(c1_c2[0]);
        write(c1_c2[1], &num, sizeof(int));
        printf("Child 1 wrote: %d\n", num);

        // step 5: read from child 2
        close(c2_c1[1]);
        read(c2_c1[0], &num, sizeof(int));
        printf("Child 1 read: %d\n", num);

        // step 6: write to parent
        num = num * 2;
        close(c1_p[0]);
        write(c1_p[1], &num, sizeof(int));
        printf("Child 1 wrote: %d\n", num);

        // close after using
        close(p_c1[0]); close(c1_c2[1]); close(c2_c1[0]); close(c1_p[1]);

        exit(0);
    }

    pid_t child2 = fork();
    if (child2 == 0) {
        // step 2: receive from child 1
        close(c1_c2[1]);
        read(c1_c2[0], &num, sizeof(int));
        printf("Child 2 read: %d\n", num);
        
        // step 3: write to parent
        close(c2_p[0]);
        num = num * 4;
        write(c2_p[1], &num, sizeof(int));
        printf("Child 2 wrote: %d\n", num);

        // step 4: read from parent
        close(p_c2[1]);
        read(p_c2[0], &num, sizeof(int));
        printf("Child 2 read: %d\n", num);

        // step 5: write to child 1
        close(c2_c1[0]);
        num = num * 4;
        write(c2_c1[1], &num, sizeof(int));
        printf("Child 2 wrote: %d\n", num);

        // close after finish using
        close(c1_c2[0]); close(c2_p[1]); close(p_c2[0]); close(c2_c1[1]);

        // close off all open pipe ends when forked again
        close(p_c1[1]); close(c1_p[0]); close(c1_c2[0]); close(c2_c1[1]);

        exit(0);
    }

    // Parent
    // step 1: send to child 1
    close(p_c1[0]);
    write(p_c1[1], &num, sizeof(int));
    printf("Parent wrote: %d\n", num);

    // step 3: read from child 2
    close(c2_p[1]);
    read(c2_p[0], &num, sizeof(int));
    printf("Parent read: %d\n", num);

    // step 4: write to child 2
    close(p_c2[0]);
    num = num * 8;
    write(p_c2[1], &num, sizeof(int));
    printf("Parent wrote: %d\n", num);

    // step 6: read from child 1
    close(c1_p[1]);
    read(c1_p[0], &num, sizeof(int));
    printf("Parent read: %d\n", num);

    // close after finish using
    close(p_c1[1]); close(c2_p[0]); close(p_c2[1]); close(c1_p[0]);

    wait(NULL);
    wait(NULL);

    // close everything after finishing
    close(p_c1[0]); close(c1_p[1]); close(c1_c2[1]); close(c2_c1[0]);
    close(c1_c2[0]); close(c2_p[1]); close(p_c2[0]); close(c2_c1[1]);

    

    return 0;
}