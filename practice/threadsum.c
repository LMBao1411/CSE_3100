#include <ctype.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>

// calculating summation of (3n^2 + 6n + 9)
long int total = 0;
pthread_mutex_t mutex;

typedef struct {
    int thread_num;
    int i, j;
} thread_data;

void *thread_calculator (void *threadarg) {
thread_data *my_data = (thread_data *)threadarg;
  for (int k = my_data->i; k <= my_data->j; k++) {
    int local = 3*k*k + 6*k + 9;

    pthread_mutex_lock(&mutex);
    total += local;
    pthread_mutex_unlock(&mutex);
  }
  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    int NUM_THREADS = atoi(argv[1]);    // number of threads
    int n = atoi(argv[2]);              // n terms
    assert(n >= 1);
    pthread_mutex_init(&mutex, NULL);

    pthread_t thread_array[NUM_THREADS];
    thread_data thread_parameters[NUM_THREADS];

    for (int t = 0; t < NUM_THREADS; t++) {
        thread_parameters[t].thread_num = t;
        thread_parameters[t].i = t * n / NUM_THREADS + 1;
        thread_parameters[t].j = (t + 1) * n / NUM_THREADS;
        pthread_create(&thread_array[t], NULL, thread_calculator, &thread_parameters[t]);
    }

    for (int t = 0; t < NUM_THREADS; t++) {
        pthread_join(thread_array[t], NULL);
    }

    pthread_mutex_destroy(&mutex);

    printf("total = %ld\n", total);
    return 0;
}
