#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NUM_THREADS 20

long double pi[NUM_THREADS];

struct thread_data {
  int thread_num;
};

// A thread to calcuate one term in the PI calculation
void *hex_pi(void *threadarg) {

  struct thread_data *my_data = (struct thread_data *)threadarg;
  int n = my_data->thread_num;

  // note how the thread writes to the array pi[]
  pi[n] = (long double)4 / (8 * n + 1) - (long double)2 / (8 * n + 4) -
          (long double)1 / (8 * n + 5) - (long double)1 / (8 * n + 6);
  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

  pthread_t threads[NUM_THREADS];
  struct thread_data thread_data_array[NUM_THREADS];
  int rc, t;

  for (t = 0; t < NUM_THREADS; t++) {
    thread_data_array[t].thread_num = t;
    rc = pthread_create(&threads[t], NULL, hex_pi, &thread_data_array[t]);
    if (rc) {
      printf("ERROR; return code from pthread_create() is %d\n", rc);
      exit(-1);
    }
  }

  long double sum = 0.;
  long double scale = 1.;
  for (t = 0; t < NUM_THREADS; t++) {
    rc = pthread_join(threads[t], NULL);
    if (rc) {
      printf("ERROR; return code from pthread_join() is %d\n", rc);
      exit(-1);
    }
    sum += pi[t] * scale;
    scale /= (long double)16.;
  }
  printf("PI = %.18Lf\n", sum);
  return 0;
}

// 1. Create a global array pi[20] to store results
// 2. Create 20 threads
// 3. Each thread gets a unique index n (0–19) via thread_data
// 4. Each thread computes one term and writes it to pi[n]
// 5. Main thread waits for all threads using pthread_join
// 6. After all threads finish, main computes the final sum using pi[n] and scaling