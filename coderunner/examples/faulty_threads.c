#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 20

long double pi[NUM_THREADS];

struct thread_data {
    int thread_num;
};

// Faulty thread function
void *hex_pi(void *threadarg) {
    struct thread_data *my_data = (struct thread_data *)threadarg;
    int n = my_data->thread_num;

    // BUG 1: Wrong denominator values
    pi[n] = (long double)4 / (8 * n + 1)
          - (long double)2 / (8 * n + 3)   // should be +4
          - (long double)1 / (8 * n + 5)
          - (long double)1 / (8 * n + 7);  // should be +6

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    struct thread_data thread_data_array[NUM_THREADS];
    int rc, t;

    for (t = 0; t < NUM_THREADS; t++) {
        thread_data_array[t].thread_num = t;

        rc = pthread_create(&threads[t], NULL, hex_pi, &thread_data_array[t]);

        if (rc) {
            printf("ERROR; pthread_create() = %d\n", rc);
            exit(-1);
        }
    }

    long double sum = 0.0;
    long double scale = 1.0;

    for (t = 0; t < NUM_THREADS; t++) {
        rc = pthread_join(threads[t], NULL);

        if (rc) {
            printf("ERROR; pthread_join() = %d\n", rc);
            exit(-1);
        }

        // BUG 2: Incorrect scaling factor
        sum += pi[t] * scale;
        scale /= 8.0;   // should divide by 16
    }

    // BUG 3: Reduced precision output
    printf("PI = %.6Lf\n", sum);

    return 0;
}