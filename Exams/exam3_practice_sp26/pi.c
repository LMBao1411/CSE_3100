#include <ctype.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define N_THREADS 2
#define SAMPLE_SIZE 10000000

typedef struct thread_arg_tag {
  int id;
  int count;
  int total;
  char padding[52];
} thread_arg_t;

double rand_double(unsigned int *s) { return (rand_r(s) / (double)RAND_MAX); }

int generate_points(int n, unsigned int seed) {
    int count = 0;
    double x, y;
    unsigned int s = seed;
    for (int i = 0; i < n; i++) {
        x = rand_double(&s);
        y = rand_double(&s);
        if (x * x + y * y <= 1)
            count++;
    }
    return count;
}

void *thread_main(void *thread_arg) {
  thread_arg_t *arg = (thread_arg_t*) thread_arg;
  arg->count = generate_points(arg->total, (unsigned int)arg->id);
  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

  int n = SAMPLE_SIZE;
  int n_threads = N_THREADS;

  int total_count = 0;

  if (argc > 1) {
    n = atoi(argv[1]);
  }
  if (argc > 2) {
    n_threads = atoi(argv[2]);
  }

  pthread_t thread_arr[n_threads];
  thread_arg_t thread_args[n_threads];
  int base = n / n_threads;         // Base samples per thread: n / n_threads.  
  int remainder = n % n_threads;    // Remainder: n % n_threads. You can give these extra points to the last thread.

  for (int i = 0; i < n_threads; i++) {
    thread_args[i].id = i;
    thread_args[i].count = 0;
    thread_args[i].total = base;
    if (i < remainder) {
      thread_args[i].total = base + 1;
    }
    else {
      thread_args[i].total = base;
    }
    pthread_create(&thread_arr[i], NULL, thread_main, &thread_args[i]);
  }

  for (int i = 0; i < n_threads; i++) {
    pthread_join(thread_arr[i], NULL);
    total_count += thread_args[i].count;
  }

  double pi = 4.0 * (double)total_count / (double)n;

  printf("pi = %.2f\n", pi);

  return 0;
}
