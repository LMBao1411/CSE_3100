#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

long count = 0;
pthread_mutex_t mtx;

void *increase(void *arg) {
    long inc = *(long *) arg;
    for (long i = 0; i<inc;i++) {
        pthread_mutex_lock(&mtx);
        count++;
        pthread_mutex_unlock(&mtx);
    }
    pthread_exit(NULL);
}



int main (int argc, char *argv[]) {
    long inc = atol(argv[1]);
    pthread_mutex_init(&mtx, NULL);
    pthread_t t1, t2;
    pthread_create(&t1, NULL, increase, &inc);
    pthread_create(&t2, NULL, increase, &inc);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("counter is %ld \n", count);
    pthread_mutex_destroy(&mtx);
    return 0;
}