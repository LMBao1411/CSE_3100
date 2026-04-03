#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "matrix.h"

#define NUM_THREADS 2

typedef struct {
    unsigned int id;
    TMatrix *m;
    TMatrix *n;
    TMatrix *t;
} thread_arg_t;

/* Main thread:
    create thread 0
    create thread 1

Thread 0:          Thread 1:
    rows 0–2           rows 3–5
    compute            compute

Main thread:
    waits (join)
    waits (join)
    return result
*/
static void *thread_main(void *p_arg)
{
    thread_arg_t *arg = (thread_arg_t *)p_arg;
    TMatrix *m = arg->m;
    TMatrix *n = arg->n;
    TMatrix *t = arg->t;

    int start, end;
    if (arg->id == 0) {
        start = 0;
        end = m->nrows / 2;
    } else {
        start = m->nrows / 2;
        end = m->nrows;
    }

    // compute assigned rows
    for (int i = start; i < end; i++) {
        for (int j = 0; j < m->ncols; j++) {
            t->data[i][j] = m->data[i][j] + n->data[i][j];
        }
    }
    return NULL;
}

/* threaded matrix addition */
TMatrix *addMatrix_thread(TMatrix *m, TMatrix *n)
{
    if (m == NULL || n == NULL)
        return NULL;

    if (m->nrows != n->nrows || m->ncols != n->ncols)
        return NULL;

    TMatrix *t = newMatrix(m->nrows, m->ncols);
    if (t == NULL)
        return NULL;

    pthread_t threads[NUM_THREADS];
    thread_arg_t args[NUM_THREADS];

    // create threads
    for (int i = 0; i < NUM_THREADS; i++) {
        args[i].id = i;
        args[i].m = m;
        args[i].n = n;
        args[i].t = t;

        if (pthread_create(&threads[i], NULL, thread_main, &args[i]) != 0) {
            perror("pthread_create failed");
            exit(1);
        }
    }

    // wait for threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join failed");
            exit(1);
        }
    }

    return t;
}