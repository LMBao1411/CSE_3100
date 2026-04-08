#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "matrix.h"

// Search TODO to find the locations where code needs to be completed

#define     NUM_THREADS     2

typedef struct {
    unsigned int id;
    TMatrix *m, *n, *t;
} thread_arg_t;

static void * thread_main(void * p_arg)
{
    // TODO
    thread_arg_t *arg = (thread_arg_t *) p_arg;
    TMatrix *m = arg->m;
    TMatrix *n = arg->n;
    TMatrix *t = arg->t;

    for (unsigned int row = arg->id; row < t->nrows; row += NUM_THREADS) {
        for (unsigned int col = 0; col < t->ncols; ++col) {
            TElement dot_product = 0;
            for (unsigned int i = 0; i < m->ncols; ++i) {
                dot_product += m->data[row][i] * n->data[i][col];
            }
            t->data[row][col] = dot_product;
        }
    }
    return NULL;
}

/* Return the sum of two matrices.
 *
 * If any pthread function fails, report error and exit. 
 * Return NULL if anything else is wrong.
 *
 * Similar to mulMatrix, but with multi-threading.
 */
TMatrix * mulMatrix_thread(TMatrix *m, TMatrix *n)
{
    if (    m == NULL || n == NULL
         || m->ncols != n->nrows )
        return NULL;

    TMatrix * t = newMatrix(m->nrows, n->ncols);
    if (t == NULL)
        return t;

    // TODO
    pthread_t threads[NUM_THREADS];
    thread_arg_t args[NUM_THREADS];
    for (int i=0; i<NUM_THREADS; i++) {
        args[i].id = i;
        args[i].m = m;
        args[i].n = n;
        args[i].t = t;
        if (pthread_create(&threads[i], NULL, thread_main, &args[i]) != 0) {
            perror("pthread_create failed");
            exit(1);
        }
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join failed");
            exit(1);
        }
    }

    return t;
}
