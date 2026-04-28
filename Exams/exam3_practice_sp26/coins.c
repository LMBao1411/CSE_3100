#include <assert.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_COUNTERS 16

typedef struct {
  int n;                 
  int data;              
  int ready;
  // ready = 0 means no data available counter should wait, tosser needs to produce
  // ready = 1 means data is available tosser should wait, counter needs to consume
  pthread_mutex_t mutex;
  pthread_cond_t cond[NUM_COUNTERS]; 
  pthread_cond_t cond_toss;          
} data_t;

typedef struct {
  int id;        
  int *p_count;  
  data_t *pdata; 
} thread_arg_t;

void *counter(void *t) {
  thread_arg_t *arg = t;
  int id = arg->id;
  int *p_count = arg->p_count;
  data_t *pdata = arg->pdata;

  while (1) {
    pthread_mutex_lock(&pdata->mutex);

    // Wait while there is no data for counter to use AND the tosser is still active
    while((pdata->ready == 0 || pdata->data != id) && pdata->n > 0) {
      pthread_cond_wait(&pdata->cond[id], &pdata->mutex);
    }

    // Exit condition: No more coins to toss and no pending data for this thread
    if (pdata->ready == 0 && pdata->n == 0) {
      pthread_mutex_unlock(&pdata->mutex);
      break;
    }

    // If data is ready and it matches this counter's ID
    if (pdata->ready && pdata->data == id) {
      (*p_count)++;
      pdata->ready = 0;
      pthread_cond_signal(&pdata->cond_toss);
    }

    pthread_mutex_unlock(&pdata->mutex);
  }
  pthread_exit(NULL);
}

void *tosser(void *t) {
  thread_arg_t *arg = t;
  int id = arg->id;
  data_t *pdata = arg->pdata;
  unsigned s = id;
  
  while (1) {
    pthread_mutex_lock(&pdata->mutex);

    // Wait for the previous coin to be consumed
    while (pdata->ready == 1) {
      pthread_cond_wait(&pdata->cond_toss, &pdata->mutex);
    }

    // Check if we have completed the requested number of tosses
    if (pdata->n <= 0) {
      pthread_mutex_unlock(&pdata->mutex);
      break;
    }

    int c1 = rand_r(&s) & 1;
    int c2 = rand_r(&s) & 1;
    int c3 = rand_r(&s) & 1;
    int c4 = rand_r(&s) & 1;
    int c = (c1 << 3) | (c2 << 2) | (c3 << 1) | c4;

    pdata->data = c;
    pdata->ready = 1;
    pdata->n = pdata->n - 1;

    // Wake up the specific counter for this result
    pthread_cond_signal(&pdata->cond[c]);

    // If this was the last coin, wake up all counters so they can exit
    if (pdata->n == 0) {
      for (int i = 0; i < NUM_COUNTERS; i++) {
        pthread_cond_broadcast(&pdata->cond[i]);
      }
    }

    pthread_mutex_unlock(&pdata->mutex);
  }
  pthread_exit(NULL);
}

// do not change main function
int main(int argc, char *argv[]) {

  if (argc != 2) {
    printf("Usage: %s n\n", argv[0]);
    exit(0);
  }
  int n = atoi(argv[1]);

  int i, rv;
  int count[NUM_COUNTERS + 1] = {0};

  data_t data;

  pthread_t threads[NUM_COUNTERS + 1];
  thread_arg_t thread_args[NUM_COUNTERS + 1];

  pthread_mutex_init(&data.mutex, NULL);
  pthread_cond_init(&data.cond_toss, NULL);
  for (i = 0; i < NUM_COUNTERS + 1; i++) {
    pthread_cond_init(&data.cond[i], NULL);
  }

  data.ready = 0;
  data.n = n;

  thread_args[0].id = 0;
  thread_args[0].pdata = &data;
  rv = pthread_create(&threads[0], NULL, tosser, &thread_args[0]);
  assert(rv == 0);

  for (i = 0; i < NUM_COUNTERS; i++) {
    thread_args[i].id = i;
    thread_args[i].p_count = &count[i];
    thread_args[i].pdata = &data;

    rv = pthread_create(&threads[i], NULL, counter, &thread_args[i]);
    assert(rv == 0);
  }

  for (i = 0; i < NUM_COUNTERS; i++) {
    pthread_join(threads[i], NULL);
  }

  pthread_mutex_destroy(&data.mutex);
  pthread_cond_destroy(&data.cond_toss);
  for (i = 0; i < NUM_COUNTERS + 1; i++)
    pthread_cond_destroy(&data.cond[i]);

  for (int i = 0; i < NUM_COUNTERS; i++) {
    printf("%2d: %d\n", i, count[i]);
  }
  return 0;
}
