#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_COUNTERS 8

typedef struct {
    int n;      // number of rolls remaining
    int data;
    int ready;
    pthread_mutex_t mutex;
    pthread_cond_t cond_toss;
    pthread_cond_t cond[NUM_COUNTERS];
} data_t;

typedef struct {
    int id;
    int *p_count;
    data_t *pdata;
} thread_arg_t;

void *counter (void *threadarg) {
    thread_arg_t *arg = threadarg;
    data_t *pdata = arg->pdata;
    int *p_count = arg->p_count;
    int id = arg->id;

    while (1) {
        pthread_mutex_lock(&pdata->mutex);

        while ( (pdata->n > 0) && (pdata->ready == 0 || pdata->data != id)) {
            pthread_cond_wait(&pdata->cond[id], &pdata->mutex);
        }

        if (pdata->ready == 0 && pdata->n == 0) {
            pthread_mutex_unlock(&pdata->mutex);
            break;
        }

        if (pdata->ready == 1 && pdata->data == id) {
            (*p_count)++;
            pdata->ready = 0;
            pthread_cond_signal(&pdata->cond_toss);
        }

        pthread_mutex_unlock(&pdata->mutex);
    }
    pthread_exit(NULL);
}

void *tosser (void *threadarg) {
    thread_arg_t *arg = threadarg;
    int id = arg->id;
    data_t *pdata = arg->pdata;
    unsigned s = id;

    while (1) {
        pthread_mutex_lock(&pdata->mutex);

        while (pdata->ready == 1) {
            pthread_cond_wait(&pdata->cond_toss, &pdata->mutex);
        }

        if (pdata->n == 0) {
            pthread_mutex_unlock(&pdata->mutex);
            break;
        }

        int c1 = rand_r(&s) & 1;
        int c2 = rand_r(&s) & 1;
        int c3 = rand_r(&s) & 1;
        int c = (c1 << 2) | (c2 << 1) | c3;  // 7 outcomes

        pdata->data = c;
        pdata->ready = 1;
        pdata->n = pdata->n - 1;

        pthread_cond_signal(&pdata->cond[c]);

        if (pdata->n == 0) {
            for (int i = 0; i<NUM_COUNTERS; i++) {
                pthread_cond_broadcast(&pdata->cond[i]);
            }
        }
        pthread_mutex_unlock(&pdata->mutex);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    int n = atol(argv[1]);
    int count[NUM_COUNTERS + 1] = {0};
    data_t data;

    pthread_t threads[NUM_COUNTERS + 1];
    thread_arg_t thread_args[NUM_COUNTERS + 1];

    pthread_mutex_init(&data.mutex, NULL);
    pthread_cond_init(&data.cond_toss, NULL);
    
    for (int i = 0; i < NUM_COUNTERS; i++) {
        pthread_cond_init(&data.cond[i], NULL);
    }

    data.ready = 0;
    data.n = n;

    thread_args[0].id = 0;
    thread_args[0].pdata = &data;
    pthread_create(&threads[0], NULL, tosser, &thread_args[0]);
    
    for (int i = 0; i < NUM_COUNTERS; i++) {
        thread_args[i].id = i;
        thread_args[i].p_count = &count[i];
        thread_args[i].pdata = &data;
        pthread_create(&threads[i], NULL, counter, &thread_args[i]);
    }

    for (int i = 0; i<NUM_COUNTERS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&data.mutex);
    pthread_cond_destroy(&data.cond_toss);
    for (int i = 0; i<NUM_COUNTERS; i++) {
        pthread_cond_destroy(&data.cond[i]);
    }

    for (int i = 0; i < NUM_COUNTERS; i++) {
        printf("%2d: %d\n", i, count[i]);
    }
    
    return 0;
}