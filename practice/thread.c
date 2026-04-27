# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# define NUM_THREADS 5
pthread_mutex_t mutex;
pthread_cond_t cond_var;
int count = 0;
void *worker_thread(void *arg) {
int id = *(int *)arg;
pthread_mutex_lock(&mutex);
// Wait until all threads are ready
while (count < NUM_THREADS) {
printf("Thread %d waiting...\n", id);
pthread_cond_wait(&cond_var, &mutex);
}
printf("Thread %d starting work\n", id);
pthread_mutex_unlock(&mutex);
return NULL;
}
int main() {
pthread_t threads[NUM_THREADS];
int thread_args[NUM_THREADS];
pthread_mutex_init(&mutex, NULL);
pthread_cond_init(&cond_var, NULL);
// Create threads
for (int i = 0; i < NUM_THREADS; i++) {
thread_args[i] = i;
pthread_create(&threads[i], NULL, worker_thread, &thread_args[i]);
}
// Wait for all threads to be ready
pthread_mutex_lock(&mutex);
count = NUM_THREADS;
pthread_mutex_unlock(&mutex);
printf("All threads ready\n");
// FILL IN ONE LINE OF CODE BELOW
pthread_cond_broadcast(&cond_var);
// Wait for threads to finish
for (int i = 0; i < NUM_THREADS; i++) {
pthread_join(threads[i], NULL);
}
pthread_mutex_destroy(&mutex);
pthread_cond_destroy(&cond_var);
return 0;
}