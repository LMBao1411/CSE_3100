#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <unistd.h>
#define MAX 1000

int n;
unsigned long sum = 0;
pthread_mutex_t mutex;

void *calculate(void *threadarg) {
    pthread_mutex_lock(&mutex);
    if (n > 0) {
        sum += n*n;
        sleep(0);
        n--;
    }
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("Usage: %s n (1-500)\n", argv[0]);
        return -1;
    }

    n = atoi(argv[1]);
    assert(n >= 1 && n <= 500);

    int k = n;
    int m = 2*n;

    pthread_mutex_init(&mutex, NULL);
    pthread_t threads[MAX];
    int rc, t;

    for (t=0; t<MAX; t++) {
        rc = pthread_create(&threads[t], NULL, calculate, NULL);
        if (rc) {
            printf("ERROR");
            exit(-1);
        }
    }

    for (t=0; t<MAX; t++) {
        rc = pthread_join(threads[t], NULL);
        if (rc) {
            printf("ERROR");
            exit(-1);
        }
    }

    pthread_mutex_destroy(&mutex);
    unsigned long correct_sum = k*(k+1)*(2*k+1)/6;
    printf("thread sum : %ld || correct sum : %ld\n", sum, correct_sum);
    return 0;
}